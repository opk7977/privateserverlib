#pragma once

#include "SServerStd.h"

#define LOGIN_SERVER_ID 8800

class Network;
class UDPSender;
class DataLeader;
class TmpSessionSpace;

class LogSrvMgr;
class DBSrvMgr;

class LoginMain
{
private:
	//======================================
	// singleTon��ü��
	//======================================
	Network*			m_network;
	UDPSender*			m_udpSender;
	DataLeader*			m_document;
	TmpSessionSpace*	m_tmpSpace;
	LogSrvMgr*			m_logSrv;
	DBSrvMgr*			m_dbSrvMgr;
	//======================================

public:
	LoginMain(void);
	~LoginMain(void);

	// �ʱ�ȭ
	BOOL Init();

	void Release();
};
