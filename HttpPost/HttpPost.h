
// HttpPost.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHttpPostApp: 
// �йش����ʵ�֣������ HttpPost.cpp
//

class CHttpPostApp : public CWinApp
{
public:
	CHttpPostApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHttpPostApp theApp;