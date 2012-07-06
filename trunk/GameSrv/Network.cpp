#include "Network.h"
#include "GameSession.h"

#include "SSessionMgr.h"
#include "SegIOCP.h"
#include "SThreadMgr.h"

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

	//세션 공간 확보
	GetSessionMgr.Create( SRUNTIME_CLASS(GameSession), sessionCount );

	//IOCP초기화
	if( !GetIOCP.Init() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("Network::Init()\nIOCP초기화에 실패 했습니다.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;

	//로비서버와 연결한다.

	
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
