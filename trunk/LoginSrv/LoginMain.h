#pragma once

#include "SServerStd.h"

class Network;
class LoginDB;
class DataLeader;
class LogSrvNet;

class LoginMain
{
private:
	//======================================
	// singleTon��ü��
	//======================================
	Network*		m_network;
	LoginDB*		m_dbMgr;
	DataLeader*		m_document;
	LogSrvNet*		m_logSrv;
	//======================================

public:
	LoginMain(void);
	~LoginMain(void);

	// �ʱ�ȭ
	BOOL Init();

	void Relase();
};
