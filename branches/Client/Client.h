
// Client.h : Client ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "STime.h"

// CClientApp:
// �� Ŭ������ ������ ���ؼ��� Client.cpp�� �����Ͻʽÿ�.
//

class CClientApp : public CWinApp
{
public:
	CClientApp();

//
// 	STime time;
// 	float timePer;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CClientApp theApp;
