#pragma once

#include "SServerStd.h"

#define DB_SERVER_ID 9000

class DataLeader;
class PlayerMgr;
class Network;
class DBMgr;
class LogSrvMgr;

class DBMain
{
	//======================================
	// singleTon°´Ã¼µé
	//======================================
	DataLeader*			m_document;
	PlayerMgr*			m_playerMgr;
	Network*			m_network;
	DBMgr*				m_dbMgr;
	LogSrvMgr*			m_logSrv;
	//======================================

public:
	DBMain(void);
	~DBMain(void);

	BOOL Init();

	void Release();
};
