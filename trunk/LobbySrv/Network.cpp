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
	// ���ǰ��� Ȯ��
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(LobbySession), sessionCount );

	//======================================
	// IOCP
	//======================================
	if( !GetIOCP.Init() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("Network::Init()\nIOCP�ʱ�ȭ�� ���� �߽��ϴ�.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	//======================================
	// ���� ����/ accept
	//======================================
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;
	
	return TRUE;
}
