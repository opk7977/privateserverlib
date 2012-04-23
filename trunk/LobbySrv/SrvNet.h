#pragma once

#include "SSocket.h"

class LobbySession;
class SPacket;

class SrvNet : public SSingleton <SrvNet>
{
private:
	friend class SSingleton<SrvNet>;

private:
	LobbySession*	m_GameSrv;

private:
	SrvNet(void);
	~SrvNet(void);

public:
	void SetSession( LobbySession* session );

	BOOL SendToGameServer( SPacket &packet );

};

#define GetSrvNet SrvNet::GetInstance()

