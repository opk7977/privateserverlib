#include "LoginSession.h"
#include "LoginProtocol.h"
#include "LoginDB.h"
#include "DataLeader.h"

#include "SPacket.h"
#include "SLogger.h"

#include "LogSrvNet.h"
#include "LogMgr.h"


SIMPLEMENT_DYNAMIC(LoginSession)
SIMPLEMENT_DYNCREATE(LoginSession)

LoginSession::LoginSession(void)
: isLogin(FALSE)
{
	//singleTon객체를 미리 받아 논다.
	m_dbMgr		= &GetDBMgr;
	m_logger	= &GetLogger;
	m_document	= &GetDocument;
	m_logSrv	= &GetSrvNet;
	m_logMgr	= &GetLog;
}

LoginSession::~LoginSession(void)
{
}

void LoginSession::OnCreate()
{
	SSession::OnCreate();

	//로그서버로 전송
// 	m_logMgr->SendLog( 8800, LogMgr::LOG_LEVEL_INFORMATION, _T("[LoginSession::RecvLogSrvConnectOK()]_1. ID 접속이 정확히 잘 되나?.\n") );

	//연결에 성공했다는 packet을 보낸다.
	SendPacket( SC_LOGIN_CONNECT_OK );
}

void LoginSession::OnDestroy()
{
	//그냥 지워 주면 됨
	SSession::OnDestroy();
}

//--------------------------------------------------------------

void LoginSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	//======================================> LogServer
	case LOG_SERVER_CONNECT_OK:
		RecvLogSrvConnectOK();
		break;
	case LOG_SERVER_SHOTDOWN:
		RecvLogSrvShotdown();
		break;
	//======================================> Client
	case CS_LOGIN_CHECK_ID:
		RecvCheckId( packet );
		break;
	case CS_LOGIN_CREATE_ACCOUNT:
		RecvCreateAccount( packet );
		break;
	case CS_LOGIN_LOGIN:
		RecvTryLogin( packet );
		break;
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("정의되지 않은 패킷 id입니다.\n\n") );
	}
}

//==============================================================

void LoginSession::RecvLogSrvConnectOK()
{
	SSynchronize Sync( this );

	//log서버 셋팅해 주자
	m_logSrv->SetSession( this );

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n로그 서버와 연결되었습니다.\n\n") );
}

void LoginSession::RecvLogSrvShotdown()
{
	//로그서버 셋팅을 풀어 주자
	SSynchronize Sync( this );

	//log서버 셋팅해 주자
	m_logSrv->DisConnect();

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n로그 서버와의 연결이 종료 되었습니다.\n\n") );
}

//--------------------------------------------------------------

//CS_LOGIN_CHECK_ID
void LoginSession::RecvCheckId( SPacket& packet )
{
	//======================================
	// ID중복 체크
	//======================================
	TCHAR tstrID[64] = {0,};
	int idSize;

	SSynchronize Sync( this );

	//--------------------------------------
	// 데이터 추출
	//--------------------------------------
	packet >> idSize;
	packet.GetData( tstrID, idSize );
	//--------------------------------------

	//ID중복 체크
	int result = m_dbMgr->CheckId( tstrID );

	SendCheckIdResult( result );
}

//CS_LOGIN_CREATE_ACCOUNT
void LoginSession::RecvCreateAccount( SPacket& packet )
{
	//======================================
	// 계정생성
	//======================================
	TCHAR tstrID[64] = {0,};
	TCHAR tstrPW[64] = {0,};
	TCHAR tstrMAIL[128] = {0,};
	int tstrSize;

	SSynchronize Sync( this );

	//--------------------------------------
	// 데이터 추출
	//--------------------------------------
	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrMAIL, tstrSize );
	//--------------------------------------

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvCheckId()\n아이디 : %s\n패스워드 : %s\n이메일 : %s\n"),
					tstrID, tstrPW,	tstrMAIL );

	//계정 생성
	int result = m_dbMgr->AddAccount( tstrID, tstrPW, tstrMAIL );	

	SendCreateResult( result );
}

//CS_LOGIN_LOGIN
void LoginSession::RecvTryLogin( SPacket& packet )
{
	//======================================
	// 로그인
	//======================================
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	int tstrSize;

	SSynchronize Sync( this );

	//--------------------------------------
	// 데이터 추출
	//--------------------------------------
	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );
	//--------------------------------------

	//로그인!
	int sessionId = m_dbMgr->TryLogin( tstrID, tstrPW );

	if( sessionId > 0 )
	{
		//로그인은 체크됬으니 중복인지 확인한다
		int retval = m_dbMgr->IsLogin( sessionId );

		if( retval < 0 )
		{
			//오류
			sessionId = retval;
		}
		else if( retval > 0 )
		{
			//이미 로그인되어 있음
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LoginSession::RecvTryLogin()\nIsLogin이미 로그인 되어 있는 ID입니다.\n\n") );
			sessionId = PRE_LOGIN_ID;
		}
		// retval == 0이면 로그인
	}


	if( sessionId > 0 )
		SendLoginSuccess( sessionId );
	else
		SendLoginFailed( sessionId );
}

//==============================================================

//SC_LOGIN_CHECK_ID_RESULT
BOOL LoginSession::SendCheckIdResult( int result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CHECK_ID_RESULT );
	sendPacket << result;

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

//SC_LOGIN_CREATE_RESULT
BOOL LoginSession::SendCreateResult( int result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CREATE_RESULT );
	sendPacket << result;

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL LoginSession::SendLoginFailed( int result )
{
	//--------------------------------------
	// 로그인 실패!
	//--------------------------------------
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_LOGIN_RESULT );
	sendPacket << result;

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL LoginSession::SendLoginSuccess( int result )
{
	//--------------------------------------
	// 로그인 성공!!
	//--------------------------------------
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_LOGIN_RESULT );
	sendPacket << result;

	//로그인 처리
	//DB에 표시
	m_dbMgr->UpdateLogin( result );

	//ip주소와 port번호를 넣어 준다.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;		//포트번호

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
