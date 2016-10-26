
#include "stdafx.h"
#include "PostThread.h"
#include "HttpPostDlg.h"


CPostItem *pPostItem = NULL;

CString GetMidStrByLAndR(CString& strSrc, CString strLeft, CString strRight, BOOL bIncludeStart, BOOL bIncludeEnd)
{
	CString strRet;
	int eIdxBegin = strSrc.Find(strLeft);
	if (eIdxBegin != -1)
	{
		if (!bIncludeStart) {
			eIdxBegin += strLeft.GetLength();
		}

		int eIdxEnd = strSrc.Find(strRight, eIdxBegin + 1);
		if (eIdxEnd != -1)
		{
			if (!bIncludeEnd) {
				strRet = strSrc.Mid(eIdxBegin, eIdxEnd - eIdxBegin);
			}
			else{
				eIdxEnd += strRight.GetLength();
				strRet = strSrc.Mid(eIdxBegin, eIdxEnd - eIdxBegin);
			}
		}
	}

	return strRet;
}

string UTF8ToGBK(const string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	wchar_t *wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, (len + 1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	string strTemp(szGBK);

	delete[]szGBK;
	delete[]wszGBK;

	return strTemp;
}

BOOL getLoginIFrameURL(CPostItem* pPItem)
{
	try{
		HRESULT hr = pPItem->m_pHttpReq->Open(_T("GET"), _T("http://i.qq.com/"));
		if (FAILED(hr)) return FALSE;

		pPItem->m_pHttpReq->SetRequestHeader(_T("Accept"), _T("text/html, application/xhtml+xml, */*"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Accept-Language"), _T("zh-CN"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("User-Agent"), _T("Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Host"), _T("i.qq.com"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Connection"), _T("Keep-Alive"));

		hr = pPItem->m_pHttpReq->Send();
		if (FAILED(hr)) return FALSE;

		_variant_t varRspBody = pPItem->m_pHttpReq->GetResponseBody();
		ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
		char *pContentBuffer = (char *)varRspBody.parray->pvData;

		string strGBK = UTF8ToGBK(pContentBuffer);
		CString strContent;
		
		strContent = strGBK.c_str();
		
		CString loginIFrameURL;

		loginIFrameURL = GetMidStrByLAndR(strContent, _T("var src = '"), _T("';"));
		if (loginIFrameURL.GetLength() <= 0) return FALSE;
		
		loginIFrameURL.Replace(_T("&amp;"), _T("&"));
		loginIFrameURL.Replace(_T("' + value + '"), _T("http%3A%2F%2Fqzs.qq.com%2Fqzone%2Fv5%2Floginsucc.html%3Fpara%3Dizone"));
		pPItem->m_appID = GetMidStrByLAndR(loginIFrameURL, _T("appid="), _T("&"));
		pPItem->m_loginIFrameURL = loginIFrameURL;
		
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL visitLoginIFrameURL(CPostItem* pPItem)
{
	try{
		HRESULT hr = pPItem->m_pHttpReq->Open(_T("GET"), (LPCTSTR)pPItem->m_loginIFrameURL);
		if (FAILED(hr)) return FALSE;

		pPItem->m_pHttpReq->SetRequestHeader(_T("Host"), _T("xui.ptlogin2.qq.com"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Referer"), _T("http://i.qq.com/"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Cookie"), _T("_qz_referrer=i.qq.com"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Accept"), _T("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Accept-Language"), _T("zh-CN,zh;q=0.8"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("User-Agent"), _T("Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Connection"), _T("Keep-Alive"));
		
		hr = pPItem->m_pHttpReq->Send();
		if (FAILED(hr)) return FALSE;

		
		_variant_t varRspBody = pPItem->m_pHttpReq->GetResponseBody();
		ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
		char *pContentBuffer = (char *)varRspBody.parray->pvData;
				
		CString strContent;
		strContent = pContentBuffer;


		//教程中取的是空 改成拿COOKIE中的字段
		_bstr_t bstrAllHeader = pPItem->m_pHttpReq->GetAllResponseHeaders();
		pPItem->m_loginSig=GetMidStrByLAndR(CString((LPCTSTR)bstrAllHeader), _T("pt_login_sig="), _T(";"));
		
		

		//pPItem->m_loginSig = GetMidStrByLAndR(strContent, _T("login_sig:\""), _T("\""));  //教程中取的是空
		pPItem->m_ptur_Ver = GetMidStrByLAndR(strContent, _T("pt_ver_md5:\""), _T("\""));
		pPItem->m_jsVer = GetMidStrByLAndR(strContent, _T("ptui_version:encodeURIComponent(\""), _T("\""));

		if (pPItem->m_loginSig.IsEmpty() || pPItem->m_ptur_Ver.IsEmpty() || pPItem->m_jsVer.IsEmpty()) return FALSE;
	}
	catch (...){
		return FALSE;
	}

	return TRUE;
}


#import "C:/windows/system32/msscript.ocx" no_namespace
BOOL visitVCodeURL(CPostItem* pPItem)
{
	try
	{
		//得到随机数字  模拟JS中Math.round（）；
		srand((unsigned)time(NULL));
		double randNum = (double)rand() / RAND_MAX;
		CString strNum;
		strNum.Format(_T("%.16lf"), randNum);

		CString strVCodeUrl;
		strVCodeUrl.Format(_T("http://check.ptlogin2.qq.com/check?regmaster=\
&pt_tea=1&uin=%s&appid=%s&js_ver=%s&js_type=1&login_sig=%s&u1=\
http%%3A%%2F%%2Fqzs.qq.com%%2Fqzone%%2Fv5%%2Floginsucc.html%%3Fpara%%3Dizone&r=%s"), 
			pPItem->m_strUseID, pPItem->m_appID, pPItem->m_jsVer, pPItem->m_loginSig, strNum);

		HRESULT hr = pPItem->m_pHttpReq->Open(_T("GET"), (LPCTSTR)strVCodeUrl);
		if (FAILED(hr)) return FALSE;


		IScriptControlPtr pScriptControl(__uuidof(ScriptControl));
		pScriptControl->Language = "JavaScript";


		LPCTSTR szPgv_pvid = _T("function jsfunc(){return (Math.round(Math.random() * 2147483647) * (new Date().getUTCMilliseconds())) % 10000000000;}");
		LPCTSTR szPgv_info = _T("function jsfunc(){return (Math.round(Math.random() * 2147483647) * (new Date().getUTCMilliseconds())) % 10000000000;}");
		LPCTSTR szPgv_pvi = _T("function jsfunc(){return Math.round(2147483647 * (Math.random() || .5)) * +new Date % 1E10;}");
		LPCTSTR szPgv_si = _T("function jsfunc(){return Math.round(2147483647 * (Math.random() || .5)) * +new Date % 1E10;}");
		
		pScriptControl->AddCode(szPgv_pvid);
		VARIANT A = pScriptControl->Eval("jsfunc();");
		int iPgv_pvid = A.intVal;
		pScriptControl->AddCode(szPgv_pvid);
		 A = pScriptControl->Eval("jsfunc();");
		int iPgv_info = A.intVal;
		pScriptControl->AddCode(szPgv_pvid);
		 A = pScriptControl->Eval("jsfunc();");
		int iPgv_pvi = A.intVal;
		pScriptControl->AddCode(szPgv_pvid);
		 A = pScriptControl->Eval("jsfunc();");
		int iPgv_si = A.intVal;

		CString pgv_cookie;
		pgv_cookie.Format(_T("pgv_pvid=%u; pgv_info=ssid=s%u; pgv_pvi=%u; pgv_si=s%u; _qz_referrer=i.qq.com"),
			iPgv_pvid, iPgv_info, iPgv_pvi, iPgv_si);		

		pPItem->m_pHttpReq->SetRequestHeader(_T("Host"), _T("check.ptlogin2.qq.com"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Referer"), (LPCTSTR)pPItem->m_loginIFrameURL);
		pPItem->m_pHttpReq->SetRequestHeader(_T("Accept"), _T("application/javascript, */*;q=0.8"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Accept-Language"), _T("zh-CN"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("User-Agent"), _T("Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Connection"), _T("Keep-Alive"));
		pPItem->m_pHttpReq->SetRequestHeader(_T("Cookie"), pgv_cookie.GetBuffer());

		hr = pPItem->m_pHttpReq->Send();
		if (FAILED(hr)) return FALSE;

		long statusCode = pPItem->m_pHttpReq->GetStatus();
		CString strContent;
		_variant_t varRspBody = pPItem->m_pHttpReq->GetResponseBody();
		char *pContentBuffer = (char *)varRspBody.parray->pvData;		
		strContent = pContentBuffer;

		_bstr_t allHeader = pPItem->m_pHttpReq->GetAllResponseHeaders();
		CString strAllHeader = allHeader;

		pPItem->m_strVcode = GetMidStrByLAndR(strContent, _T(",\'"), _T("\',"));
		pPItem->m_uHex = GetMidStrByLAndR(strContent, _T("\\x"), _T("\'"), TRUE);
		pPItem->m_ptvfSession = GetMidStrByLAndR(strAllHeader, _T("ptvfsession="), _T(";"));

	}
	catch (...)
	{
		return FALSE;
	}
	return 1;
}

UINT __cdecl LoginPostFunc(LPVOID pParam)
{
	CHttpPostDlg *pHttpPostDlg = (CHttpPostDlg*)pParam;
	if (!pHttpPostDlg)  return 0;

	CoInitialize(NULL);
	try
	{
		HRESULT hr = S_FALSE;
		pPostItem = new CPostItem();
		if (!pPostItem->m_pHttpReq)
		{
			hr=pPostItem->m_pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
			if (FAILED(hr)) return 0;
		}

		pPostItem->m_strUseID = pHttpPostDlg->m_strName;
		pPostItem->m_strPass = pHttpPostDlg->m_strPass;
		BOOL bRet = FALSE;
		bRet = getLoginIFrameURL(pPostItem);
		if (!bRet) return FALSE;


		bRet = visitLoginIFrameURL(pPostItem);
		if (!bRet) return FALSE;
		


		bRet = visitVCodeURL(pPostItem);
		if (!bRet) return FALSE;

	}
	catch (...)
	{
		return 0;
	}
	return 1;
}