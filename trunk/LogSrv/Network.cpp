#include "StdAfx.h"
#include "Network.h"
#include "LogSession.h"

#include "SSessionMgr.h"
#include "SegIOCP.h"

#include "SPacket.h"

#include "LogSrvProtocol.h"

//��Ŷ ť
#include "SPacketQueue.h"

Network::Network(void)
{
}

Network::~Network(void)
{
	Release();
}

BOOL Network::Init( int sessionCount, int port )
{
	m_accept.Init();

	//======================================
	// ���ǰ��� Ȯ��
	//======================================
	GetSessionMgr.Create( SRUNTIME_CLASS(LogSession), sessionCount );

	//======================================
	// ��Ŷ ť ���� Ȯ��
	//======================================
	GetPacketQ.Init();

	//======================================
	// IOCP
	//======================================
	if( !GetIOCP.Init() )
	{
		MessageBox( NULL, _T("IOCP������ ������ �ֽ��ϴ�"), _T("����"), MB_OK );
		return FALSE;
	}

	//======================================
	// ���� ����/ accept
	//======================================
	if( !m_accept.SetAcceptor( port ) )
		return FALSE;

	return TRUE;
}

void Network::Release()
{
	m_accept.Release();
}

void Network::SendShotdownMsg()
{
	SPacket sendPacket( LOG_SERVER_SHOTDOWN );

	GetSessionMgr.SendAllSession( sendPacket );
}