#pragma once

#include "SServerStd.h"

class Network;
/*class LoginDB;*/
class DataLeader;
class LogSrvNet;
class TmpSessionSpace;
class DBSrvMgr;

class LoginMain
{
private:
	//======================================
	// singleTon객체들
	//======================================
	Network*			m_network;
/*	LoginDB*			m_dbMgr;*/
	DataLeader*			m_document;
	LogSrvNet*			m_logSrv;
	TmpSessionSpace*	m_tmpSpace;
	DBSrvMgr*			m_dbSrvMgr;
	//======================================

public:
	LoginMain(void);
	~LoginMain(void);

	// 초기화
	BOOL Init();

	void Relase();
};
