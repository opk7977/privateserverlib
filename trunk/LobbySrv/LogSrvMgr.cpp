#include "LogSrvMgr.h"

#include "LobbySession.h"
#include "LobbyProtocol.h"
#include "SSessionMgr.h"
#include "SPacket.h"
#include "SSynchronize.h"

#include "SLogger.h"

LogSrvMgr::LogSrvMgr(void)
: m_logSrv(NULL)
, m_isConnect(FALSE)
, m_serverID(0)
{
#ifdef _DEBUG
	m_logger = &GetLogger;
#endif
}

LogSrvMgr::~LogSrvMgr(void)
{
}

BOOL LogSrvMgr::Init( int SrvID )
{
	m_serverID = SrvID;

	if( !m_sock.Init() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LogSrvMgr::Init()\n소켓 초기화에 실패했습니다.\n\n") );
#endif
		return FALSE;
	}
	if( !m_sock.CreateSocket() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LogSrvMgr::Init()\n소켓 생성화에 실패했습니다.\n\n") );
#endif
		return FALSE;
	}

	return TRUE;
}

BOOL LogSrvMgr::ConnectToLogSrv( char* ipAddr, int port )
{
	//연결을 하고
	if( !m_sock.ConnectSock( ipAddr, port, &m_sockAddr ) )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LogSrvMgr::ConnectToSrv()\n서버와의 연결에 실패했습니다.\n\n") );
#endif
		return FALSE;
	}

	if( !m_sock.SetNonBlkSock() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LogSrvMgr::ConnectToSrv()\n소켓 논블럭설정에 실패했습니다.\n\n") );
#endif
		return FALSE;
	}

	//직접 세션Mgr에 세션 생성을 요청
	GetSessionMgr.CreateNewSession( m_sock.GetSocket(), m_sockAddr );

	return TRUE;
}

void LogSrvMgr::DisConnect()
{
	SSynchronize sync( this );

	if( !m_isConnect )
		return;

	//m_logSrv->OnDestroy();

	m_isConnect = FALSE;
	m_logSrv = NULL;
}

void LogSrvMgr::SetSession( LobbySession* session )
{
	if( session == NULL )
		return;

	m_logSrv = session;
	m_isConnect = TRUE;
}

BOOL LogSrvMgr::SendToLogSrv( SPacket& packet )
{
	if( !m_isConnect )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LogSrvMgr::SendToLogServer()\n서버와의 연결이 되어 있지 않습니다.\n\n") );
#endif
		return FALSE;
	}

	if( m_logSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LogSrvMgr::SendToLogServer()\n보낸패킷이 패킷의 크기가 다릅니다\n\n") );
#endif
		return FALSE;
	}

	return TRUE;
}

void LogSrvMgr::SendLog( int errLv, TCHAR* lpszFmt, ... )
{
	SSynchronize Sync( this );

	TCHAR tstr[SEND_LOG_STRING_LENTH]={0,};
	va_list Vargs;

	//-----------------------------------------------------------------
	// 가변인자를 바로 받아 문자열 형식으로 ..
	//-----------------------------------------------------------------
	va_start(Vargs, lpszFmt);
	vswprintf_s( tstr, lpszFmt, Vargs );
	va_end(Vargs);

	int size = _tcslen( tstr ) * sizeof( TCHAR );

	SPacket sendPacket( RECV_SERVER_LOG );
	sendPacket << m_serverID;
	sendPacket << errLv;
	sendPacket << size;
	sendPacket.PutData( tstr, size );

	SendToLogSrv( sendPacket );
}

void LogSrvMgr::SendErrLog( INT32 errorCode, TCHAR* lpszFmt, ... )
{
	SSynchronize Sync( this );

	TCHAR tmpTStr[SEND_LOG_STRING_LENTH]={0,};

	int retval = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM
								, NULL
								, errorCode
								, 0
								, (LPWSTR)&tmpTStr
								, TMPSTRING_LENTH
								, NULL );

	if( retval == 0 )
	{
		OutputDebugStringW( _T("Filed Get Error Meg\n") );
		return;
	}

	int size = _tcslen( tmpTStr ) * sizeof( TCHAR );

	SPacket sendPacket( RECV_SERVER_LOG );
	sendPacket << m_serverID;
	sendPacket << LOG_LEVEL_ERROR;
	sendPacket << size;
	sendPacket.PutData( tmpTStr, size );

	SendToLogSrv( sendPacket );
}
