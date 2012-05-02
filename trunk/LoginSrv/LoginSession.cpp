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
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("정의되지 않은 패킷 id입니다.\n") );
	}
}

//==============================================================

//CS_LOGIN_CHECK_ID
void LoginSession::RecvCheckId( SPacket& packet )
{
	TCHAR tstrID[64];
	int idSize;

	packet >> idSize;
	packet.GetData( tstrID, idSize );

	int result = GetDBMgr.CheckId( tstrID );

	SendCheckIdResult( result );
}

//CS_LOGIN_CREATE_ACCOUNT
void LoginSession::RecvCreateAccount( SPacket& packet )
{
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	TCHAR tstrMAIL[128];
	int tstrSize;

	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrMAIL, tstrSize );

	int result = GetDBMgr.AddAccount( tstrID, tstrPW, tstrMAIL );

	SendCreateResult( result );
}

//CS_LOGIN_LOGIN
void LoginSession::RecvTryLogin( SPacket& packet )
{
	TCHAR tstrID[64];
	TCHAR tstrPW[64];
	int tstrSize;

	packet >> tstrSize;
	packet.GetData( tstrID, tstrSize );
	packet >> tstrSize;
	packet.GetData( tstrPW, tstrSize );

	int sessionId = GetDBMgr.TryLogin( tstrID, tstrPW );

	if( sessionId > 0 )
	{
		//로그인은 체크됬으니 중복인지 확인한다
		int retval = GetDBMgr.IsLogin( sessionId );

		if( retval < 0 )
		{
			//오류
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvTryLogin()\nIsLogin쿼리 실패, 쿼리문을 다시 확인하세요\n\n") );
			sessionId = retval;
		}
		else if( retval > 0 )
		{
			//이미 로그인되어 있음
			sessionId = -5;
		}
		// 그냥 로그인 됨
	}
	else if( sessionId < -1 )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LoginSession::RecvTryLogin()\nTryLogin쿼리 실패, 쿼리문을 다시 확인하세요\n\n") );
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
		GetDBMgr.UpdateLogin( result );

		//ip주소와 port번호를 넣어 준다.
		char ipAddr[20]={0,};
		CopyMemory( ipAddr, GetDocument.LobbySrvIP, 15 );

		sendPacket << (int)(strlen( ipAddr ));
		sendPacket.PutData( ipAddr, strlen( ipAddr ) );
		sendPacket << GetDocument.LobbySrvPortNum;		//포트번호
	}

	if( SendPacket( sendPacket ) == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
