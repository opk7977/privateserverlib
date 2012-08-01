#include "SrvNet.h"
#include "SPacket.h"
#include "SLogger.h"

#include "GameSession.h"
#include "SSessionMgr.h"

SrvNet::SrvNet(void) : m_LobbySrv(NULL)
{
	m_logger	= &GetLogger;
}

SrvNet::~SrvNet(void)
{
}

BOOL SrvNet::ConnectToSrv( char* ipAddr, int port )
{
	if( !m_sock.Init() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� �ʱ�ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.CreateSocket() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� ����ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	//������ �ϰ�
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n�κ񼭹����� ���ῡ �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� ��������� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	//���� ����Mgr�� ���� ������ ��û
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void SrvNet::DisConnect()
{
	if( m_LobbySrv == NULL )
		return;

	m_LobbySrv = NULL;
}

BOOL SrvNet::SendToLobbyServer( SPacket &packet )
{
	if( m_LobbySrv == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::SendToLobbyServer()\n�������� ������ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_LobbySrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::SendToLobbyServer()\n������Ŷ�� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�\n\n") );
		return FALSE;
	}

	return TRUE;
}
