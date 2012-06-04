#pragma once

#include "SSocket.h"

class LobbySession;
class SPacket;
class SLogger;

class SrvNet : public SSingleton <SrvNet>
{
private:
	friend class SSingleton<SrvNet>;

private:
	//======================================
	// singleTon°´Ã¼
	//======================================
	SLogger*		m_logger;

private:
	LobbySession*	m_GameSrv;

private:
	SrvNet(void);
	~SrvNet(void);

public:
	void SrvNetClear();
	inline void SetSession( LobbySession* session ) { m_GameSrv = session; }
	inline LobbySession* GetSession() const { return m_GameSrv; }
	inline BOOL IsConnect() { return (m_GameSrv != NULL); }

	BOOL SendToGameServer( SPacket &packet );

};

#define GetSrvNet SrvNet::GetInstance()

