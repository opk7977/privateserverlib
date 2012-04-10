#pragma once

#include "SAcceptor.h"
#include "SSocket.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	//서버 자신의 어셉터
	SAcceptor	m_accepter;

	//게임서버에 연결될 소켓
	SSocket m_GameSrv;

public:
	Network(void);
	~Network(void);

	//리슨소켓 & 서버관련 MGR초기화
	BOOL	Init( int count );

	//서버 연결
	BOOL	SettingSrv( WORD port );

	//서버 종료
	void	Release();
};

#define GetNetwork Network::GetInstance()

