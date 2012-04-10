#include "Network.h"

#include "DemoGameSession.h"
#include "SSessionMgr.h"
#include "SegIOCP.h"
#include "SLogger.h"

Network::Network(void)
{
}

Network::~Network(void)
{
	Release();
}

BOOL Network::Init( int count )
{
	m_accepter.Init();

	//���ǰ��� 10 Ȯ��
	GetSessionMgr.Create( SRUNTIME_CLASS(DemoGameSession), count );

	//IOCP
	if( !GetIOCP.Init() )
		return FALSE;

	return TRUE;
}

BOOL Network::SettingSrv( WORD port )
{
	if( !m_accepter.SetAcceptor( port ) )
		return FALSE;

	return TRUE;
}

void Network::Release()
{
}
