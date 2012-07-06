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

	//���� ���� Ȯ��
	GetSessionMgr.Create( SRUNTIME_CLASS(GameSession), sessionCount );

	//IOCP�ʱ�ȭ
	if( !GetIOCP.Init() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("Network::Init()\nIOCP�ʱ�ȭ�� ���� �߽��ϴ�.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;

	//�κ񼭹��� �����Ѵ�.

	
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
