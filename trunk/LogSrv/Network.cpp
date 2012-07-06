#include "StdAfx.h"
#include "Network.h"
#include "LogSession.h"

#include "SSessionMgr.h"
#include "SegIOCP.h"
#include "SThreadMgr.h"

#include "SPacket.h"

#include "LogSrvProtocol.h"

//패킷 큐
#include "SPacketQueue.h"

Network::Network(void)
{
}

Network::~Network(void)
{
}

BOOL Network::Init( int sessionCount, int port )
{
	m_accept.Init();

	//======================================
	// 세션공간 확보
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(LogSession), sessionCount );

	//======================================
	// 패킷 큐 공간 확보
	//======================================
	GetPacketQ.Init();

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
	//======================================
	// accept 해제
	//======================================
	m_accept.Release();

	//======================================
	// IOCP해제
	//======================================
	GetIOCP.Release();

	//======================================
	// 쓰레드 종료
	//======================================
	GetThreadMgr.EndAllThread();

	//======================================
	// 세션 공간 삭제
	//======================================
	GetSessionMgr.Release();

	//======================================
	// 패킷 큐 해제
	//======================================
	GetPacketQ.Release();
}

void Network::SendShotdownMsg()
{
	SPacket sendPacket( LOG_SERVER_SHOTDOWN );

	GetSessionMgr.SendAllSession( sendPacket );
}
