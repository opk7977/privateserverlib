#include "Network.h"

#include "FSMSession.h"

#include "SSessionMgr.h"
#include "SThreadMgr.h"
#include "SegIOCP.h"
#include "SLogger.h"

Network::Network(void)
{
}

Network::~Network(void)
{
	//Release();
}

BOOL Network::Init( int count )
{
	m_accepter.Init();

	//���� ���� 10Ȯ��
	GetSessionMgr.Create( SRUNTIME_CLASS(FSMSession), count );

	//IOCP ����
	if( !GetIOCP.Init() )
		return FALSE;

	return TRUE;
}

BOOL Network::SettingSrv( WORD port )
{
	if( !m_accepter.SetAcceptor( port ) )
		return FALSE;

	return TRUE;
}

void Network::Release()
{
	//GetIOCP.Release();
	//m_accepter.Release();
}
