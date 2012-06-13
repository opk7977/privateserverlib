#pragma once

#include "SServerStd.h"

class DataLeader;
class PlayerMgr;
class Network;
class DBMgr;

class DBMain
{
	//======================================
	// singleTon°´Ã¼µé
	//======================================
	DataLeader*			m_document;
	PlayerMgr*			m_playerMgr;
	Network*			m_network;
	DBMgr*				m_dbMgr;
	//======================================
public:
	DBMain(void);
	~DBMain(void);

	BOOL Init();

	void Release();
};
