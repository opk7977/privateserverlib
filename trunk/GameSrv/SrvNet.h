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
	//���� ������ nonblock����flag
	BOOL Init();

	//����
	BOOL ConnectToSrv( char* ipAddr, int port );
	//void DisConnect();

	//������ �Լ�
	BOOL SendToLobbyServer( SPacket &packet );

	//���� ����
	inline void SetSession( GameSession* session ) { m_LobbySrv = session; }
	inline GameSession* GetSession() const { return m_LobbySrv; }
};

#define GetSrvNet SrvNet::GetInstance()
