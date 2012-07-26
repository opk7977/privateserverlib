#include "DBSrvMgr.h"

#include "GameSession.h"
#include "SPacket.h"
#include "SSessionMgr.h"

#include "SLogger.h"

DBSrvMgr::DBSrvMgr(void)
: m_dbSrv(NULL)
{
	m_logger = &GetLogger;
}

DBSrvMgr::~DBSrvMgr(void)
{
}

BOOL DBSrvMgr::Init()
{
	if( !m_sock.Init() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::Init()\n���� �ʱ�ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.CreateSocket() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::Init()\n���� ����ȭ�� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL DBSrvMgr::ConnectToDBSrv( char* ipAddr, int port )
{
	//������ �ϰ�
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::ConnectToSrv()\n�κ񼭹����� ���ῡ �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::ConnectToSrv()\n���� ��������� �����߽��ϴ�.\n\n") );
		return FALSE;
	}

	//���� ����Mgr�� ���� ������ ��û
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void DBSrvMgr::DisConnect()
{
	if( m_dbSrv == NULL )
		return;

	m_dbSrv = NULL;
}

void DBSrvMgr::SetSession( GameSession* session )
{
	if( session == NULL )
		return;

	m_dbSrv = session;
}

BOOL DBSrvMgr::SendToDBServer( SPacket& packet )
{
	if( m_dbSrv == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("DBSrvMgr::SendToLogServer()\n�������� ������ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_dbSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("DBSrvMgr::SendToLogServer()\n������Ŷ�� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�\n\n") );
		return FALSE;
	}

	return TRUE;
}
