#include "Network.h"
#include "LoginSession.h"
#include "SSessionMgr.h"
#include "SThreadMgr.h"
#include "SegIOCP.h"

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
	// 세션공간 확보
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(LoginSession), sessionCount );

	//======================================
	// IOCP
	//======================================
	if( !GetIOCP.Init( 2 ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("Network::Init()\nIOCP초기화에 실패 했습니다.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
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
	// 쓰레드 종료
	//======================================
	GetThreadMgr.EndAllThread();

	//======================================
	// accept 해제
	//======================================
	m_accept.Release();

	//======================================
	// IOCP해제
	//======================================
	GetIOCP.Release();

	//======================================
	// 세션 공간 삭제
	//======================================
	GetSessionMgr.Release();
}
