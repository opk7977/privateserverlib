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
	//======================================
	// 초기화 & 셋팅
	//======================================
	BOOL Init( int sessionCount );
	BOOL SrvSetting( int port );

	//======================================
	// 해제
	//======================================
	void Release();
};

#define GetNetwork Network::GetInstance()
