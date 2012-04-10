#include "Network.h"

#include "FSMSession.h"

#include "SSessionMgr.h"
#include "SThreadMgr.h"
#include "SegIOCP.h"
#include "SLogger.h"

Network::Network(void)
{
}

Network::~Network(void)
{
	//Release();
}

BOOL Network::Init( int count )
{
	m_accepter.Init();

	//技记 傍埃 10犬焊
	GetSessionMgr.Create( SRUNTIME_CLASS(FSMSession), count );

	//IOCP 积己
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
	//GetIOCP.Release();
	//m_accepter.Release();
}
