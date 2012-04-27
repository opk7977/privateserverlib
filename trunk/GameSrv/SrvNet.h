#pragma once

#include "SSocket.h"

class GameSession;
class SPacket;

class SrvNet : public SSingleton <SrvNet>
{
private:
	friend class SSingleton<SrvNet>;

private:
	SSocket			m_sock;
	SOCKADDR		m_sockAddr;

	GameSession*	m_LobbySrv;

private:
	SrvNet(void);
	~SrvNet(void);

public:
	//소켓 생성시 nonblock설정flag
	BOOL Init();

	//연결
	BOOL ConnectToSrv( char* ipAddr, int port );
	//void DisConnect();

	//보내기 함수
	BOOL SendToLobbyServer( SPacket &packet );

	//세션 설정
	inline void SetSession( GameSession* session ) { m_LobbySrv = session; }
	inline GameSession* GetSession() const { return m_LobbySrv; }
};

#define GetSrvNet SrvNet::GetInstance()
