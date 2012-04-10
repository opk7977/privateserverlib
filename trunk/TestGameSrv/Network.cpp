#include "Network.h"
#include "SSessionMgr.h"
#include "TestGSession.h"
#include "SegIOCP.h"

Network::Network(void)
{
}

Network::~Network(void)
{
	Release();
}

BOOL Network::Init( int count )
{
	m_acceptor.Init();

	//세션공간 확보
	GetSessionMgr.Create( SRUNTIME_CLASS(TestGSession), count );

	if( !GetIOCP.Init() )
		return FALSE;

	return TRUE;
}

BOOL Network::SettingSrv( WORD port )
{
	if( !m_acceptor.SetAcceptor( port ) )
		return FALSE;

	return TRUE;
}

void Network::Release()
{
	m_acceptor.Release();
}
