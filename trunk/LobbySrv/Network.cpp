#include "Network.h"
#include "LobbySession.h"
#include "SSessionMgr.h"
#include "SegIOCP.h"

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
	GetSessionMgr.Create( SRUNTIME_CLASS(LobbySession), sessionCount );

	//IOCP�ʱ�ȭ
	if( !GetIOCP.Init() )
		return FALSE;

	return TRUE;
}

BOOL Network::SrvSetting( int port )
{
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;
	
	return TRUE;
}