#include "Network.h"
#include "LoginSession.h"
#include "SSessionMgr.h"
#include "SegIOCP.h"

Network::Network(void)
{
}

Network::~Network(void)
{
}

BOOL Network::Init( int sessionCount )
{
	m_accept.Init();

	//세션공간 10 확보
	GetSessionMgr.Create( SRUNTIME_CLASS(LoginSession), sessionCount );

	//IOCP
	if( !GetIOCP.Init() )
		return FALSE;

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;

	return TRUE;
}
