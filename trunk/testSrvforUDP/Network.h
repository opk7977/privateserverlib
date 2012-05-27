#pragma once

#include "SAcceptor.h"

class SSessionMgr;
class SegIOCP;
class SLogger;

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	SAcceptor		m_accept;

	//--------------------------------------
	// singleTon °´Ã¼µé
	//--------------------------------------
	SSessionMgr*	m_sessionMgr;
	SegIOCP*		m_iocp;
	SLogger*		m_logger;

private:
	Network(void);
	~Network(void);

public:
	BOOL Init( int sessionCount );
	BOOL SrvSetting( int port );
};

#define GetNetwork Network::GetInstance()

