#pragma once

#include "SAcceptor.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	//서버 자신의 어셉터
	SAcceptor	m_acceptor;

public:
	Network(void);
	~Network(void);

	//서버관련 초기화
	BOOL Init( int count );

	//서버연결
	BOOL SettingSrv( WORD port );

	//서버 종료
	void Release();
};

#define GetNetwork Network::GetInstance()
