
// HttpPostDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HttpPost.h"
#include "HttpPostDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHttpPostDlg 对话框



CHttpPostDlg::CHttpPostDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHttpPostDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strPass(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHttpPostDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NUM_EDT, m_strName);
	DDX_Text(pDX, IDC_PASS_EDT, m_strPass);
}

BEGIN_MESSAGE_MAP(CHttpPostDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOG_BTN, &CHttpPostDlg::OnBnClickedLogBtn)
END_MESSAGE_MAP()


// CHttpPostDlg 消息处理程序

BOOL CHttpPostDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	InitStatusBar();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHttpPostDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHttpPostDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHttpPostDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID  CHttpPostDlg::SetStateInfo(LPCTSTR lpszText, UINT posIdx)
{
	if (!lpszText || _tcslen(lpszText) <= 0) return;
	m_Statusbar.SetPaneText(posIdx, lpszText);
}
VOID CHttpPostDlg::InitStatusBar()
{
	CRect  rect;
	GetWindowRect(rect);
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height() + 15);

	UINT array[1] = { 12301 };
	m_Statusbar.Create(this);
	m_Statusbar.SetIndicators(array, sizeof(array) / sizeof(UINT));

	m_Statusbar.SetPaneInfo(0, array[0], 0, rect.Width());
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	SetStateInfo(_T("状态：启动成功！"));

}

#include "PostThread.h"

void CHttpPostDlg::OnBnClickedLogBtn()
{
	UpdateData(TRUE);
	if (m_strName.IsEmpty() || m_strPass.IsEmpty())
	{
		MessageBox(_T("用户名和密码不能为空"));
		return;
	}
	

	CWinThread * pThread = AfxBeginThread(LoginPostFunc, this);

	
}
