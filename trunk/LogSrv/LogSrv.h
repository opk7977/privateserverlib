
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
#include "STime.h"

class PacketParser;

class CLogSrvApp : public CWinApp
{
public:
	CLogSrvApp();

public:
	STime			m_timer;

	//--------------------------------------
	// single ton ��ü
	//--------------------------------------
	PacketParser	*m_parser;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CLogSrvApp theApp;
