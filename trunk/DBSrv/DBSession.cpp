#include "DBSession.h"
#include "DBProtocol.h"

#include "SrvMgr.h"
#include "DBMgr.h"
#include "PlayerMgr.h"

SIMPLEMENT_DYNAMIC(DBSession)
SIMPLEMENT_DYNCREATE(DBSession)

SrvMgr*		DBSession::m_login		= new SrvMgr;
SrvMgr*		DBSession::m_lobby		= new SrvMgr;
SrvMgr*		DBSession::m_game		= new SrvMgr;

DBMgr*		DBSession::m_dbMgr		= &GetDBMgr;
PlayerMgr*	DBSession::m_playerMgr	= &GetPlayerMgr;

DBSession::DBSession(void)
{
}

DBSession::~DBSession(void)
{
}

void DBSession::OnCreate()
{
	SSession::OnCreate();

	//연결 오케이.
	SendPacket( DB_TO_OTHER_CONNECT_OK );
}

void DBSession::OnDestroy()
{
	//할일 합시돠...

	SSession::OnDestroy();
}

void DBSession::PacketParsing( SPacket& packet )
{
	SSynchronize( this );

	switch( packet.GetID() )
	{
	case LOG_SERVER_CONNECT_OK:
		//
		break;
	case LOG_SERVER_SHOTDOWN:
		//
		break;
	//==============================================================
	case SC_LOGIN_CONNECT_OK:
		RecvLoginConnectOK();
		break;
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case SC_GAME_CONNECT_OK:
		RecvGameConnectOK();
		break;
	//==============================================================
	case LOGIN_TO_DB_CHECK_ID:
		RecvLoginCheckID( packet );
		break;
	case LOGIN_TO_DB_CREATE_ACCOUNT:
		RecvLoginCreateAccount( packet );
		break;
	case LOGIN_TO_DB_TRYLOGIN:
		RecvLoginTryLogin( packet );
		break;
	case GAME_TO_DB_UPDATE_USERDATA:
		//
		break;
	case OTHER_TO_DB_DISCONNECT_CHARACTER:
		//
		break;
	default:
		//
		break;
	}
}

//==============================================================

void DBSession::RecvLoginConnectOK()
{
	//로그인 서버와 연결됨
	m_login->SetSession( this );
}

void DBSession::RecvLobbyConnectOK()
{
	//로비서버와 연결된
	m_lobby->SetSession( this );
}

void DBSession::RecvGameConnectOK()
{
	//게임서버와 연결됨
	m_game->SetSession( this );
}

//--------------------------------------------------------------

void DBSession::RecvLoginCheckID( SPacket& packet )
{
	int indexId, size;
	TCHAR tmpId[10]={0,};

	packet >> indexId;
	//id추출
	packet >> size;
	//유니코드는 10글자가 size 20임
	if( size > 20 )
	{
		//id불량
		SendLoginCheckIDResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );

	//db체크
	BOOL result = m_dbMgr->CheckID( tmpId );

	//결과 return
	SendLoginCheckIDResult( indexId, result );
}

void DBSession::RecvLoginCreateAccount( SPacket& packet )
{
	int indexId, size;
	TCHAR tmpId[10]={0,}, tmpPw[15]={0,}, tmpMail[20]={0,};

	packet >> indexId;
	//id추출
	packet >> size;
	if( size > 20 )
	{
		//id불량
		SendLoginCreateAccountResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );
	//pw추출
	packet >> size;
	if( size > 30 )
	{
		//pw불량
		SendLoginCreateAccountResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );
	//email추출
	packet >> size;
	if( size > 40 )
	{
		//email불량
		SendLoginCreateAccountResult( indexId, FALSE );
		return;
	}
	packet.GetData( tmpId, size );

	//db에 계정 생성
	BOOL result = m_dbMgr->CreateAccount( tmpId, tmpPw, tmpMail );

	//결과 return
	SendLoginCreateAccountResult( indexId, result );
}

void DBSession::RecvLoginTryLogin( SPacket& packet )
{
	int indexId, size;
	TCHAR tmpId[10]={0,}, tmpPw[15]={0,};

	packet >> indexId;
	//id추출
	packet >> size;
	if( size > 20 )
	{
		//id불량
		//SendLoginTryLoginResult( indexId, NONEXISTENT_ID );
		return;
	}
	packet.GetData( tmpId, size );
	//pw추출
	packet >> size;
	if( size > 30 )
	{
		//pw불량
		//SendLoginTryLoginResult( indexId, WRONG_PW );
		return;
	}
	packet.GetData( tmpPw, size );

	int sessionID = m_dbMgr->TryLogin( tmpId, tmpPw );

	if( sessionID == -1 )
	{
		//id없음
		SendLoginTryLoginResult( indexId, NONEXISTENT_ID );
		return;
	}
	else if( sessionID == 0 )
	{
		//pw틀림
		SendLoginTryLoginResult( indexId, WRONG_PW );
		return;
	}
	else if( sessionID == -10 )	
	{
		//서버오류
		SendLoginTryLoginResult( indexId, SERVER_ERROR );
		return;
	}

	//로그인에 성공했다면 이미 로그인되어 있는 id인지를 검사한다.
	PlayerObj* tmpPlayer = m_playerMgr->FindPlayerBySessionId( sessionID );

	if( tmpPlayer == NULL )
	{
		//정상적인 로그인
		//캐릭터를 저장
		tmpPlayer = m_playerMgr->GetPlayerSpace();
		m_dbMgr->SettingUserData( sessionID, tmpPlayer );
		m_playerMgr->AddPlayer( sessionID, tmpPlayer );

		//로비에 알림
		//SendLobbyChacterLogin( tmpPlayer );

		//로그인서버에 결과를 알림
		SendLoginTryLoginResult( indexId, sessionID );
	}
	else
	{
		//이미 로그인 되어 있는 ID
		//다른 서버로 애를 튕기라고 전한다
		//
		//
		//

		//id가 이미 사용중이었다고 보낸다
		SendLoginTryLoginResult( indexId, PRE_LOGIN_ID );

		//캐릭터 삭제
		m_playerMgr->ReturnPlayerSpace( tmpPlayer );
	}
}

//==============================================================

BOOL DBSession::SendLoginCheckIDResult( int indexId, BOOL result )
{
	SPacket sendPacket( DB_TO_LOGIN_CHECK_ID_RESULT );

	sendPacket << indexId;
	sendPacket << result;

	SendPacket( sendPacket );
	
	return TRUE;
}

BOOL DBSession::SendLoginCreateAccountResult( int indexId, BOOL result )
{
	SPacket sendPacket( DB_TO_LOGIN_CREATE_ACCOUNT_RESULT );

	sendPacket << indexId;
	sendPacket << result;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLobbyChacterLogin( PlayerObj* player )
{
	SPacket sendPacket( DB_TO_LOBBY_CHARACTER_LOGIN );

	player->PackageMyAllInfo( sendPacket );

	m_lobby->SendPacketToSrv( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLoginTryLoginResult( int indexId, int result )
{
	SPacket sendPacket( DB_TO_LOGIN_LOGINRESULT );

	sendPacket << indexId;
	sendPacket << result;

	SendPacket( sendPacket );

	return TRUE;
}

