
// SampleDemoGame.h : SampleDemoGame ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSampleDemoGameApp:
// �� Ŭ������ ������ ���ؼ��� SampleDemoGame.cpp�� �����Ͻʽÿ�.
//

class CSampleDemoGameApp : public CWinApp
{
public:
	CSampleDemoGameApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSampleDemoGameApp theApp;
