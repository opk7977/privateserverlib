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
	// �ʱ�ȭ & ����
	//======================================
	BOOL Init( int sessionCount );
	BOOL SrvSetting( int port );

	//======================================
	// ����
	//======================================
	void Release();
};

#define GetNetwork Network::GetInstance()
