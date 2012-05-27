#include "Network.h"
#include "UDPSession.h"

#include "SSessionMgr.h"
#include "SegIOCP.h"
#include "SLogger.h"

Network::Network(void)
{
	m_sessionMgr	= &GetSessionMgr;
	m_iocp			= &GetIOCP;
	m_logger		= &GetLogger;
}

Network::~Network(void)
{
}

BOOL Network::Init( int sessionCount )
{
	m_accept.Init();

	//======================================
	// 세션 공간 확보
	//======================================
	m_sessionMgr->Create( SRUNTIME_CLASS(UDPSession), sessionCount );

	//======================================
	// IOCP 설정
	//======================================
	if( !m_iocp->Init() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Network::Init() IOCP초기화에 실패 했습니다.\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	//======================================
	// 서버 셋팅
	//======================================
	if( !m_accept.SetAcceptor( port ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Network::SrvSetting() 서버 세팅 실패\n") );
		return FALSE;
	}

	return TRUE;
}
