
// HttpPostDlg.h : 头文件
//

#pragma once

# include "resource.h"

// CHttpPostDlg 对话框
class CHttpPostDlg : public CDialogEx
{
// 构造
public:
	CHttpPostDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HTTPPOST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
