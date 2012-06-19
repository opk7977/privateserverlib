#include "Network.h"
#include "LobbySession.h"
#include "SSessionMgr.h"
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
	GetSessionMgr.Create( SRUNTIME_CLASS(LobbySession), sessionCount );

	//======================================
	// IOCP
	//======================================
	if( !GetIOCP.Init() )
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
