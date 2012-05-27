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
	// ���� ���� Ȯ��
	//======================================
	m_sessionMgr->Create( SRUNTIME_CLASS(UDPSession), sessionCount );

	//======================================
	// IOCP ����
	//======================================
	if( !m_iocp->Init() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Network::Init() IOCP�ʱ�ȭ�� ���� �߽��ϴ�.\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	//======================================
	// ���� ����
	//======================================
	if( !m_accept.SetAcceptor( port ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Network::SrvSetting() ���� ���� ����\n") );
		return FALSE;
	}

	return TRUE;
}
