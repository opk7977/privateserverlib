#include "StdAfx.h"
#include "Network.h"
#include "LogSession.h"

#include "SSessionMgr.h"
#include "SegIOCP.h"

#include "SPacket.h"

Network::Network(void)
{
}

Network::~Network(void)
{
	Release();
}

BOOL Network::Init( int sessionCount, int port )
{
	m_accept.Init();

	//======================================
	// 세션공간 확보
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(LogSession), sessionCount );

	//======================================
	// IOCP
	//======================================
	if( !GetIOCP.Init() )
	{
		MessageBox( NULL, _T("IOCP설정에 문제가 있습니다"), _T("제길"), MB_OK );
		return FALSE;
	}

	//======================================
	// 리슨 설정/ accept
	//======================================
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;

	return TRUE;
}

void Network::Release()
{
	m_accept.Release();
}

void Network::SendShotdownMsg()
{
	SPacket sendPacket( LOG_SERVER_SHOTDOWN );

	GetSessionMgr.SendAllSession( sendPacket );
}
