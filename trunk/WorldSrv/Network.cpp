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
	// ����
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(WorldSession), sessionCount );

	//iocp
	if( !GetIOCP.Init( 2 ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("Network::Init() iocp���� ����") );
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
	// accept ����
	//======================================
	m_accept.Release();

	//======================================
	// IOCP����
	//======================================
	GetIOCP.Release();

	//======================================
	// ������ ����
	//======================================
	GetThreadMgr.EndAllThread();

	//======================================
	// ���� ���� ����
	//======================================
	GetSessionMgr.Release();
}
