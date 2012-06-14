#include "LoginSession.h"
#include "LoginProtocol.h"
#include "DataLeader.h"

#include "SPacket.h"
#include "SLogger.h"

#include "LogSrvNet.h"
#include "LogMgr.h"

#include "DBSrvMgr.h"
#include "TmpSessionSpace.h"


SIMPLEMENT_DYNAMIC(LoginSession)
SIMPLEMENT_DYNCREATE(LoginSession)

LoginSession::LoginSession(void)
: isLogin(FALSE)
, m_isWaitting(FALSE)
{
	//singleTon객체를 미리 받아 논다.
	m_logger	= &GetLogger;
	m_document	= &GetDocument;
	m_logSrv	= &GetSrvNet;
	m_logMgr	= &GetLog;
	m_dbSrvMgr	= &GetDBSrv;
	m_tmpSpace	= &GetTmpSpace;
}

LoginSession::~LoginSession(void)
{
}

void LoginSession::OnCreate()
{
	SSynchronize Sync( this );

	SSession::OnCreate();

	//로그서버로 전송
// 	m_logMgr->SendLog( 8800, LogMgr::LOG_LEVEL_INFORMATION, _T("[LoginSession::RecvLogSrvConnectOK()]_1. ID 접속이 정확히 잘 되나?.\n") );

	//연결에 성공했다는 packet을 보낸다.
	SendPacket( SC_LOGIN_CONNECT_OK );
}

void LoginSession::OnDestroy()
{
	SSynchronize Sync( this );

	//그냥 지워 주면 됨
	SSession::OnDestroy();
}

//--------------------------------------------------------------

void LoginSession::PacketParsing( SPacket& packet )
{
	SSynchronize Sync( this );

	switch( packet.GetID() )
	{
	//======================================> LogServer
	case LOG_SERVER_CONNECT_OK:
		RecvLogSrvConnectOK();
		break;
	case LOG_SERVER_SHOTDOWN:
		RecvLogSrvShotdown();
		break;
	//======================================> DBServer
	case DB_TO_OTHER_CONNECT_OK:
		RecvDBConnectOK();
		break;
	case DB_TO_LOGIN_CHECK_ID_RESULT:
		RecvDBCheckIDResult( packet );
		break;
	case DB_TO_LOGIN_CREATE_ACCOUNT_RESULT:
		RecvDBCreateAccountResult( packet );
		break;
	case DB_TO_LOGIN_LOGINRESULT:
		RecvDBLoginResult( packet );
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
	//SSynchronize Sync( this );

	//log서버 셋팅해 주자
	m_logSrv->SetSession( this );

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n로그 서버와 연결되었습니다.\n\n") );
}

void LoginSession::RecvLogSrvShotdown()
{
	//로그서버 셋팅을 풀어 주자
	//SSynchronize Sync( this );

	//log서버 셋팅해 주자
	m_logSrv->DisConnect();

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvLogSrvConnectOK()\n로그 서버와의 연결이 종료 되었습니다.\n\n") );
}
//--------------------------------------------------------------

void LoginSession::RecvDBConnectOK()
{
	//DB서버 접속 성공 셋팅해 주자
	m_dbSrvMgr->SetSession( this );

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
		_T("LoginSession::RecvDBConnectOK()\nDB 서버와 연결되었습니다.\n\n") );
}

void LoginSession::RecvDBCheckIDResult( SPacket& packet )
{
	//아이디 중복 검사 결과 패킷이 왔음
	int index;
	BOOL result;

	packet >> index;
	packet >> result;

	//대기중인 session정보를 받아 온다
	LoginSession* tmpSession = m_tmpSpace->FindSessionByIndex( index );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\nindex에 해당하는session이 없습니다.\n\n") );
		return;
	}

	tmpSession->SendCheckIdResult( index, result );
}

void LoginSession::RecvDBCreateAccountResult( SPacket& packet )
{
	int index;
	BOOL result;

	packet >> index;
	packet >> result;

	//대기중인 session정보를 받아 온다
	LoginSession* tmpSession = m_tmpSpace->FindSessionByIndex( index );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\nindex에 해당하는session이 없습니다.\n\n") );
		return;
	}

	tmpSession->SendCreateResult( index, result );
}

void LoginSession::RecvDBLoginResult( SPacket& packet )
{
	int index, result;

	packet >> index;
	packet >> result;

	//대기중인 session정보를 받아 온다
	LoginSession* tmpSession = m_tmpSpace->FindSessionByIndex( index );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LoginSession::RecvDBCheckIDResult()\nindex에 해당하는session이 없습니다.\n\n") );
		return;
	}

	tmpSession->SendLoginResult( index, result );
}

//--------------------------------------------------------------

//CS_LOGIN_CHECK_ID
void LoginSession::RecvCheckId( SPacket& packet )
{
	//이미 DB의 응답을 기다리고 있는 중이므로 아무것도 받지 않는다
	if( m_isWaitting )
		return;

	m_isWaitting = TRUE;

	//======================================
	// ID중복 체크
	//======================================
	TCHAR tstrID[64] = {0,};
	int idSize;

	//--------------------------------------
	// 데이터 추출
	//--------------------------------------
	packet >> idSize;
	packet.GetData( tstrID, idSize );
	//--------------------------------------

	SendToDBCheckID( idSize, tstrID );
}

//CS_LOGIN_CREATE_ACCOUNT
void LoginSession::RecvCreateAccount( SPacket& packet )
{
	//이미 DB의 응답을 기다리고 있는 중이므로 아무것도 받지 않는다
	if( m_isWaitting )
		return;

	m_isWaitting = TRUE;

	//======================================
	// 계정생성
	//======================================
	TCHAR tstrID[64] = {0,};
	TCHAR tstrPW[64] = {0,};
	TCHAR tstrMAIL[128] = {0,};
	int IDSize, PWSize, MAILSize;

	//--------------------------------------
	// 데이터 추출
	//--------------------------------------
	packet >> IDSize;
	packet.GetData( tstrID, IDSize );
	packet >> PWSize;
	packet.GetData( tstrPW, PWSize );
	packet >> MAILSize;
	packet.GetData( tstrMAIL, MAILSize );
	//--------------------------------------

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LoginSession::RecvCheckId()\n아이디 : %s\n패스워드 : %s\n이메일 : %s\n"),
					tstrID, tstrPW,	tstrMAIL );

	SendToDBCreateAccount( IDSize, tstrID, PWSize, tstrPW, MAILSize, tstrMAIL );
}

//CS_LOGIN_LOGIN
void LoginSession::RecvTryLogin( SPacket& packet )
{
	//이미 DB의 응답을 기다리고 있는 중이므로 아무것도 받지 않는다
	if( m_isWaitting )
		return;

	m_isWaitting = TRUE;

	//======================================
	// 로그인
	//======================================
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	int IDSize, PWSize;

	//--------------------------------------
	// 데이터 추출
	//--------------------------------------
	packet >> IDSize;
	packet.GetData( tstrID, IDSize );
	packet >> PWSize;
	packet.GetData( tstrPW, PWSize );
	//--------------------------------------

	SendToDBTryLogin( IDSize, tstrID, PWSize, tstrPW );

// 	//로그인!
// 	int sessionId = m_dbMgr->TryLogin( tstrID, tstrPW );
// 
// 	if( sessionId > 0 )
// 	{
// 		//로그인은 체크됬으니 중복인지 확인한다
// 		int retval = m_dbMgr->IsLogin( sessionId );
// 
// 		if( retval < 0 )
// 		{
// 			//오류
// 			sessionId = retval;
// 		}
// 		else if( retval > 0 )
// 		{
// 			//이미 로그인되어 있음
// 			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LoginSession::RecvTryLogin()\nIsLogin이미 로그인 되어 있는 ID입니다.\n\n") );
// 			sessionId = PRE_LOGIN_ID;
// 		}
// 		// retval == 0이면 로그인
// 	}
// 
// 
// 	if( sessionId > 0 )
// 		SendLoginSuccess( sessionId );
// 	else
// 		SendLoginFailed( sessionId );
}

//==============================================================

BOOL LoginSession::SendToDBCheckID( int size, TCHAR* uid )
{
	//이미 DB의 응답을 기다리고 있는 중이므로 아무것도 받지 않는다
// 	if( m_isWaitting )
// 		return;

	//index를 하나 받아 온다
	int index = m_tmpSpace->GetIndex();
	//우선 데기열에 집어 넣고
	m_tmpSpace->WaitForDBSignal( index, this );

	//패킷 만들기
	SPacket sendPacket( LOGIN_TO_DB_CHECK_ID );
	sendPacket << index;
	sendPacket << size;
	sendPacket.PutData( uid, size );

	//db로 전송
	m_dbSrvMgr->SendToDBServer( sendPacket );

	return TRUE;
}

BOOL LoginSession::SendToDBCreateAccount( int uidSize, TCHAR* uid, int pwSize, TCHAR* pw, int mailSize, TCHAR* mail )
{
	//index를 하나 받아 온다
	int index = m_tmpSpace->GetIndex();
	//우선 데기열에 집어 넣고
	m_tmpSpace->WaitForDBSignal( index, this );

	SPacket sendPacket( LOGIN_TO_DB_CREATE_ACCOUNT );
	sendPacket << index;
	sendPacket << uidSize;
	sendPacket.PutData( uid, uidSize );
	sendPacket << pwSize;
	sendPacket.PutData( pw, pwSize );
	sendPacket << mailSize;
	sendPacket.PutData( mail, mailSize );

	//db로 전송
	m_dbSrvMgr->SendToDBServer( sendPacket );
	
	return TRUE;
}

BOOL LoginSession::SendToDBTryLogin( int uidSize, TCHAR* uid, int pwSize, TCHAR* pw )
{
	//index를 하나 받아 온다
	int index = m_tmpSpace->GetIndex();
	//우선 데기열에 집어 넣고
	m_tmpSpace->WaitForDBSignal( index, this );

	SPacket sendPacket( LOGIN_TO_DB_TRYLOGIN );
	sendPacket << index;
	sendPacket << uidSize;
	sendPacket.PutData( uid, uidSize );
	sendPacket << pwSize;
	sendPacket.PutData( pw, pwSize );

	//db로 전송
	m_dbSrvMgr->SendToDBServer( sendPacket );

	return TRUE;
}

//--------------------------------------------------------------

//SC_LOGIN_CHECK_ID_RESULT
BOOL LoginSession::SendCheckIdResult( int index, BOOL result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CHECK_ID_RESULT );
	sendPacket << result;

	BOOL sendResult = FALSE;
	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		sendResult = TRUE;

	//대기 공간에서 빠진다
	m_tmpSpace->GetSignal( index );

	//다시 FALSE로 바꿔 준다
	m_isWaitting = FALSE;

	return sendResult;
}

//SC_LOGIN_CREATE_RESULT
BOOL LoginSession::SendCreateResult( int index, BOOL result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_CREATE_RESULT );
	sendPacket << result;

	BOOL sendResult = FALSE;
	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		sendResult = TRUE;

	//대기 공간에서 빠진다
	m_tmpSpace->GetSignal( index );

	//다시 FALSE로 바꿔 준다
	m_isWaitting = FALSE;

	return sendResult;
}

BOOL LoginSession::SendLoginResult( int index, int result )
{
	BOOL SendResult;

	if( result <= 0 )
		SendResult = SendLoginFailed( result );
	else
		SendResult = SendLoginSuccess( result );

	//대기 공간에서 빠진다
	m_tmpSpace->GetSignal( index );

	//다시 FALSE로 바꿔 준다
	m_isWaitting = FALSE;

	return SendResult;
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
	//m_dbMgr->UpdateLogin( result );

	//ip주소와 port번호를 넣어 준다.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;		//포트번호

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
