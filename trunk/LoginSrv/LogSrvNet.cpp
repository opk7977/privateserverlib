#include "LogSrvNet.h"

#include "SPacket.h"
#include "SLogger.h"

#include "LoginSession.h"
#include "SSessionMgr.h"

LogSrvNet::LogSrvNet(void)
: m_LogSrv(NULL)
, m_isConnect(FALSE)
{
	m_logger = &GetLogger;
}

LogSrvNet::~LogSrvNet(void)
{
}

BOOL LogSrvNet::Init()
{
	if( !m_sock.Init() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::Init()\n���� �ʱ�ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.CreateSocket() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::Init()\n���� ����ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL LogSrvNet::ConnectToSrv( char* ipAddr, int port )
{
	//������ �ϰ�
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n�κ񼭹����� ���ῡ �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("SrvNet::ConnectToSrv()\n���� ������������ �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	//���� ����Mgr�� ���� ������ ��û
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void LogSrvNet::DisConnect()
{
	
}

void LogSrvNet::SetSession( LoginSession* session )
{
	if( session == NULL )
		return;

	m_LogSrv = session;
	m_isConnect = TRUE;
}

BOOL LogSrvNet::SendToLogServer( SPacket &packet )
{
	if( !m_isConnect )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("SrvNet::SendToLogServer()\n�������� ������ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_LogSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("SrvNet::SendToLogServer()\n������Ŷ�� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�\n\n") );
		return FALSE;
	}

	return TRUE;
}