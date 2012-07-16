#pragma once

#include "SServerStd.h"

#define GAME_SERVER_ID 7900

class DataLeader;

class GameMgr;
class CharMgr;
class ItemMgr;

class Network;
class LogSrvMgr;
class DBSrvMgr;

class GameSrvMain
{
private:
	//======================================
	// singleTon°´Ã¼µé
	//======================================
	DataLeader*	m_document;

	GameMgr*	m_gameMgr;
	CharMgr*	m_charMgr;
	ItemMgr*	m_itemMgr;

	Network*	m_network;
	LogSrvMgr*	m_logSrv;
	DBSrvMgr*	m_dbMgr;
	//======================================

public:
	GameSrvMain(void);
	~GameSrvMain(void);

	BOOL Init();
	void Release();
};
