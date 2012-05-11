#include "LoginSession.h"
#include "LoginProtocol.h"
#include "LoginDB.h"
#include "SLogger.h"

#include "DataLeader.h"

SIMPLEMENT_DYNAMIC(LoginSession)
SIMPLEMENT_DYNCREATE(LoginSession)

LoginSession::LoginSession(void)
: isLogin(FALSE)
{
	m_dbMgr = &GetDBMgr;
	m_logger = &GetLogger;
}

LoginSession::~LoginSession(void)
{
}

void LoginSession::OnCreate()
{
	SSession::OnCreate();

	//연결에 성공했다는 packet을 보낸다.
	SendPacket( SC_LOGIN_CONNECT_OK );
}

void LoginSession::OnDestroy()
{
	SSession::OnDestroy();
}

//--------------------------------------------------------------

void LoginSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("정의되지 않은 패킷 id입니다.\n\n") );
	}
}

//==============================================================

//CS_LOGIN_CHECK_ID
void LoginSession::RecvCheckId( SPacket& packet )
{
	TCHAR tstrID[64] = {0,};
	int idSize;

	SSynchronize Sync( this );

	packet >> idSize;
	packet.GetData( tstrID, idSize );

	int result = m_dbMgr->CheckId( tstrID );

	if( result < 0 )
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvCheckId()\n아이디 %s는 중복되는 ID가 이미 존재합니다.\n\n"), tstrID );
	else
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvCheckId()\n아이디 %s는 사용할 수 있는 ID입니다.\n\n"), tstrID );

	SendCheckIdResult( result );
}

//CS_LOGIN_CREATE_ACCOUNT
void LoginSession::RecvCreateAccount( SPacket& packet )
{
	TCHAR tstrID[64] = {0,};
	TCHAR tstrPW[64] = {0,};
	TCHAR tstrMAIL[128] = {0,};
	int tstrSize;

	SSynchronize Sync( this );

	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrMAIL, tstrSize );

	int result = m_dbMgr->AddAccount( tstrID, tstrPW, tstrMAIL );

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvCheckId()\n아이디 : %s\n패스워드 : %s\n이메일 : %s\n"), tstrID, tstrPW, tstrMAIL );

	if( result < 0 )
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("계정 생성에 실패하였습니다.\n\n") );
	else
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("계정 생성에 성공하였습니다.\n\n") );

	SendCreateResult( result );
}

//CS_LOGIN_LOGIN
void LoginSession::RecvTryLogin( SPacket& packet )
{
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	int tstrSize;

	SSynchronize Sync( this );

	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );

	int sessionId = m_dbMgr->TryLogin( tstrID, tstrPW );

	if( sessionId > 0 )
	{
		//로그인은 체크됬으니 중복인지 확인한다
		int retval = m_dbMgr->IsLogin( sessionId );

		if( retval < 0 )
		{
			//오류
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvTryLogin()\nIsLogin쿼리 실패, 쿼리문을 다시 확인하세요.\n\n") );
			sessionId = retval;
		}
		else if( retval > 0 )
		{
			//이미 로그인되어 있음
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvTryLogin()\nIsLogin이미 로그인 되어 있는 ID입니다.\n\n") );
			sessionId = -5;
		}
		// 그냥 로그인 됨
	}
	else if( sessionId < -1 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvTryLogin()\nTryLogin쿼리 실패, 쿼리문을 다시 확인하세요.\n\n") );
	}


	SendLoginResult( sessionId );
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

//SC_LOGIN_LOGIN_RESULT
BOOL LoginSession::SendLoginResult( int result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOGIN_LOGIN_RESULT );
	sendPacket << result;

	if( result > 0 )
	{
		//result값이 양수(session)값이면

		//로그인 처리
		//DB에 표시
		m_dbMgr->UpdateLogin( result );

		//ip주소와 port번호를 넣어 준다.
		char ipAddr[15]={0,};
		CopyMemory( ipAddr, GetDocument.LobbySrvIP, 15 );

		sendPacket << (int)(strlen( ipAddr ));
		sendPacket.PutData( ipAddr, strlen( ipAddr ) );
		sendPacket << GetDocument.LobbySrvPortNum;		//포트번호
	}

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
