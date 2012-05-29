#include "SrvNet.h"
#include "SPacket.h"
#include "SLogger.h"

#include "GameSession.h"
#include "SSessionMgr.h"

SrvNet::SrvNet(void) : m_LobbySrv(NULL)
{
}

SrvNet::~SrvNet(void)
{
}

BOOL SrvNet::ConnectToSrv( char* ipAddr, int port )
{
	if( !m_sock.Init() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� �ʱ�ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.CreateSocket() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� ����ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	//������ �ϰ�
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n�κ񼭹����� ���ῡ �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� ��������� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

// 	Sleep(1);

	//���� ����Mgr�� ���� ������ ��û
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void SrvNet::DisConnect()
{
	m_sock.Release();
}

BOOL SrvNet::SendToLobbyServer( SPacket &packet )
{
	if( m_LobbySrv == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::SendToLobbyServer()\n�������� ������ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_LobbySrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::SendToLobbyServer()\n������Ŷ�� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�\n\n") );
		return FALSE;
	}

	return TRUE;
}
