
// LogSrv.h : LogSrv ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CLogSrvApp:
// �� Ŭ������ ������ ���ؼ��� LogSrv.cpp�� �����Ͻʽÿ�.
//

class CLogSrvApp : public CWinApp
{
public:
	CLogSrvApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLogSrvApp theApp;
