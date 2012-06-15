#pragma once

#include "SServerStd.h"

#define LOBBY_SERVER_ID 8900

class Network;
class RoomMgr;
class CharMgr;
class DataLeader;
class LogSrvMgr;
class DBSrvMgr;

class LobbyMain
{
private:
	//======================================
	// singleTon°´Ã¼µé
	//======================================
	Network*	m_network;
	RoomMgr*	m_roomMgr;
	CharMgr*	m_charMgr;
	DataLeader*	m_document;
	LogSrvMgr*	m_logSrv;
	DBSrvMgr*	m_dbMgr;
	//======================================

public:
	LobbyMain(void);
	~LobbyMain(void);

	BOOL Init();
	void Release();
};
