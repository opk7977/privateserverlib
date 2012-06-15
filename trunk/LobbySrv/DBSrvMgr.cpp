#include "DBSrvMgr.h"

#include "LobbySession.h"
#include "SPacket.h"
#include "SSessionMgr.h"

#include "SLogger.h"

DBSrvMgr::DBSrvMgr(void)
: m_dbSrv(NULL)
, m_isConnect(FALSE)
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::Init()\n소켓 초기화에 실패했습니다.\n\n") );
		return FALSE;
	}

	if( !m_sock.CreateSocket() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::Init()\n소켓 생성화에 실패했습니다.\n\n") );
		return FALSE;
	}

	return TRUE;
}

BOOL DBSrvMgr::ConnectToDBSrv( char* ipAddr, int port )
{
	//연결을 하고
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::ConnectToSrv()\n로비서버와의 연결에 실패했습니다.\n\n") );
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("DBSrvMgr::ConnectToSrv()\n소켓 논블럭설정에 실패했습니다.\n\n") );
		return FALSE;
	}

	//직접 세션Mgr에 세션 생성을 요청
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void DBSrvMgr::DisConnect()
{
	if( m_dbSrv == NULL )
		return;

	//m_dbSrv->OnDestroy();

	m_dbSrv = NULL;
}

void DBSrvMgr::SetSession( LobbySession* session )
{
	if( session == NULL )
		return;

	m_dbSrv = session;
	m_isConnect = TRUE;
}

BOOL DBSrvMgr::SendToDBServer( SPacket& packet )
{
	if( !m_isConnect )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("DBSrvMgr::SendToLogServer()\n서버와의 연결이 되어 있지 않습니다.\n\n") );
		return FALSE;
	}

	if( m_dbSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("DBSrvMgr::SendToLogServer()\n보낸패킷이 패킷의 크기가 다릅니다\n\n") );
		return FALSE;
	}

	return TRUE;
}
