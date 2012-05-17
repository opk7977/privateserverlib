#pragma once

#include "SAcceptor.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	SAcceptor		m_accept;

private:
	Network(void);
	~Network(void);

public:
	BOOL Init( int sessionCount, int port );
	void Release();

	//======================================
	// 꺼지기 전에 모든 접속해 있는
	// 모든 서버에게 보낸다
	//======================================
	void SendShotdownMsg();
};

#define GetNetwork Network::GetInstance()

