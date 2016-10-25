#pragma once 

#include <string>


using namespace std;
#import "C:\\Windows\\System32\\WinHttpCom.dll" no_namespace


class CPostItem
{
public:
	IWinHttpRequestPtr m_pHttpReq;
	CString m_strUseID;
	CString m_strPass;
	CString m_loginIFrameURL;
	CString m_loginSig;
	CString m_ptur_Ver;
	CString m_jsVer;
	CString m_strVcode;
	CString m_uHex;
	CString m_ptvfSession;
	CString m_appID;

	CPostItem()
	{
		m_pHttpReq = NULL;
	}
};



UINT __cdecl LoginPostFunc(LPVOID pParam);

inline CString GetMidStrByLAndR(CString& strSrc, CString strLeft, CString strRight, BOOL bIncludeStart = FALSE, BOOL bIncludeEnd = FALSE);

