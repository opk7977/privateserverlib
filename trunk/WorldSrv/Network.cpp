#include "Network.h"

#include "SSessionMgr.h"
#include "WorldSession.h"
#include "SLogger.h"

Network::Network(void)
{
}

Network::~Network(void)
{
}

BOOL Network::Init( int sessionCount )
{
	m_accept.Init();

	//======================================
	// 세션
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(WorldSession), sessionCount );

	//iocp
	if( !GetIOCP.Init( 2 ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("Network::Init() iocp설정 오류") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
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
}
