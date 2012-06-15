#include "Network.h"
#include "LoginSession.h"
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
	GetSessionMgr.Create( SRUNTIME_CLASS(LoginSession), sessionCount );

	//======================================
	// IOCP
	//======================================
	if( !GetIOCP.Init() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("Network::Init()\nIOCP�ʱ�ȭ�� ���� �߽��ϴ�.\n\n") );
#endif
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
