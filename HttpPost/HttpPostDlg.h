
// HttpPostDlg.h : ͷ�ļ�
//

#pragma once

# include "resource.h"

// CHttpPostDlg �Ի���
class CHttpPostDlg : public CDialogEx
{
// ����
public:
	CHttpPostDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HTTPPOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogBtn();

	VOID InitStatusBar();
	VOID  SetStateInfo(LPCTSTR lpszText, UINT posIdx = 0);

	CString m_strName;
	CString m_strPass;
	CStatusBar m_Statusbar;
};
