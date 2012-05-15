#pragma once

#include "SServerStd.h"

class Network;
class RoomMgr;
class CharMgr;
class DataLeader;
class CheckDB;

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
	CheckDB*	m_dbMgr;
	//======================================

public:
	LobbyMain(void);
	~LobbyMain(void);

	BOOL Init();
	void Release();
};
