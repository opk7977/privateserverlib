#pragma once

#include "SServerStd.h"

class Network;
class RoomMgr;
class CharMgr;
class DataLeader;
class DBSrvMgr;
//class CheckDB;

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
	DBSrvMgr*	m_dbMgr;
	//CheckDB*	m_dbMgr;
	//======================================

public:
	LobbyMain(void);
	~LobbyMain(void);

	BOOL Init();
	void Release();
};
