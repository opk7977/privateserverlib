#include "DBSession.h"
#include "DBProtocol.h"

#include "SrvMgr.h"
#include "DBMgr.h"
#include "PlayerMgr.h"

#include "SLogger.h"
#include "LogSrvMgr.h"

SIMPLEMENT_DYNAMIC(DBSession)
SIMPLEMENT_DYNCREATE(DBSession)

SrvMgr*		DBSession::m_login			= new SrvMgr;
SrvMgr*		DBSession::m_lobby			= new SrvMgr;
SrvMgr*		DBSession::m_game			= new SrvMgr;

DBMgr*		DBSession::m_dbMgr			= &GetDBMgr;
PlayerMgr*	DBSession::m_playerMgr		= &GetPlayerMgr;
SLogger*	DBSession::m_logger			= &GetLogger;

#ifdef CONNECT_LOG_SERVER
	LogSrvMgr*	DBSession::m_logSrv		= &GetLogSrvMgr;
#else
	LogSrvMgr*	DBSession::m_logSrv		= NULL;
#endif


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
	if( m_login->GetSession() == this )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::OnDestroy()\n로그 서버와 연결이 끈겼습니다.\n\n") );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::OnDestroy() 로그 서버와 연결이 끈겼습니다") );
#endif
		m_login->Release();
	}
	else if( m_lobby->GetSession() == this )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::OnDestroy()\n로비 서버와 연결이 끈겼습니다.\n\n") );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::OnDestroy() 로비 서버와 연결이 끈겼습니다") );
#endif
		m_lobby->Release();
	}
	else if( m_game->GetSession() == this )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("DBSession::OnDestroy()\n게임 서버와 연결이 끈겼습니다.\n\n") );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
			_T("DBSession::OnDestroy() 게임 서버와 연결이 끈겼습니다") );
#endif
		m_game->Release();
	}
#ifdef CONNECT_LOG_SERVER
	else if( m_logSrv->GetSession() == this )
	{
	#ifdef _DEBUG
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
				_T("DBSession::OnDestroy()\n로그 서버와 연결이 끈겼습니다.\n\n") );
	#endif
		m_logSrv->DisConnect();
	}
#endif

	SSession::OnDestroy();
}

void DBSession::PacketParsing( SPacket& packet )
{
	SSynchronize( this );

	switch( packet.GetID() )
	{
	case LOG_SERVER_CONNECT_OK:
		RecvLogSrvConnectOK();
		break;
	case LOG_SERVER_SHOTDOWN:
		RecvLogServerShotdown();
		break;
	//==============================================================> Log서버
	case SC_LOGIN_CONNECT_OK:
		RecvLoginConnectOK();
		break;
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case SC_GAME_CONNECT_OK:
		RecvGameConnectOK();
		break;
	//==============================================================> 다른 서버와의 연결 확인
	case LOGIN_TO_DB_CHECK_ID:
		RecvLoginCheckID( packet );
		break;
	case LOGIN_TO_DB_CREATE_ACCOUNT:
		RecvLoginCreateAccount( packet );
		break;
	case LOGIN_TO_DB_TRYLOGIN:
		RecvLoginTryLogin( packet );
		break;
	case LOBBY_TO_DB_CHARINSERT_READY_RESULT:
		RecvLobbyCharInsertReadyResult( packet );
		break;
	case GAME_TO_DB_UPDATE_USERDATA:
		RecvGameCharDataUpdate( packet );
		break;
	case OTHER_TO_DB_DISCONNECT_CHARACTER:
		RecvOtherSrvCharDisconnect( packet );
		break;
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("%d번은 정의되지 않은 패킷 id입니다.\n\n"), packet.GetID() );
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING, _T("%d번은 정의되지 않은 패킷 id입니다."), packet.GetID() );
#endif
		break;
	}
}

//==============================================================

void DBSession::RecvLogSrvConnectOK()
{
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SetSession( this );
#endif

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLogSrvConnectOK()\n로그 서버와 연결되었습니다\n\n") );
#endif

}

void DBSession::RecvLogServerShotdown()
{
#ifdef CONNECT_LOG_SERVER
	#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("DBSession::RecvLogServerShotdown()\n로그 서버와 연결이 끈겼습니다.\n\n") );
	#endif
	m_logSrv->DisConnect();
#endif
}

//==============================================================

void DBSession::RecvLoginConnectOK()
{
	//로그인 서버와 연결됨
	m_login->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLoginConnectOK()\n로그인 서버와 연결되었습니다\n\n") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLoginConnectOK() 로그인 서버와 연결되었습니다") );
#endif
}

void DBSession::RecvLobbyConnectOK()
{
	//로비서버와 연결된
	m_lobby->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLobbyConnectOK()\n로비 서버와 연결되었습니다\n\n") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLobbyConnectOK() 로비 서버와 연결되었습니다") );
#endif
}

void DBSession::RecvGameConnectOK()
{
	//게임서버와 연결됨
	m_game->SetSession( this );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvGameConnectOK()\n게임 서버와 연결되었습니다\n\n") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvGameConnectOK() 게임 서버와 연결되었습니다") );
#endif
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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
				_T("DBSession::RecvLoginCheckID()\nID %s의 중복결과는 %s입니다.\n\n"), tmpId, result ? _T("사용가능") : _T("사용불가능") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLoginCheckID() ID %s의 중복결과는 %s입니다."), tmpId, result ? _T("사용가능") : _T("사용불가능") );
#endif

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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLoginCreateAccount()\nID %s 계정생성 결과는 %s.\n\n"), tmpId, result ? _T("성공") : _T("실패") );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLoginCreateAccount() ID %s 계정생성 결과는 %s."), tmpId, result ? _T("성공") : _T("실패") );
#endif

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
		SendLoginTryLoginResult( indexId, NONEXISTENT_ID );
		return;
	}
	packet.GetData( tmpId, size );
	//pw추출
	packet >> size;
	if( size > 30 )
	{
		//pw불량
		SendLoginTryLoginResult( indexId, WRONG_PW );
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

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::RecvLoginTryLogin()\nID %s 로그인 성공/ 로비서버로 캐릭터 생성을 요청.\n\n"), tmpId );
#endif

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::RecvLoginTryLogin() ID %s 로그인 성공./ 로비서버로 캐릭터 생성을 요청"), tmpId );
#endif

		//로비에 알림
		SendLobbyChacterLogin( indexId, tmpPlayer );
	}
	else
	{
		//이미 로그인 되어 있는 ID
		//다른 서버로 애를 튕기라고 전한다
		SendLobbyNGameDropPlayer( sessionID );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("DBSession::RecvLoginTryLogin()\nID %s 이미 로그인 된 ID입니다. 기존의 player의 접속을 끊습니다.\n\n"), tmpId );
#endif

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("DBSession::RecvLoginTryLogin() ID %s 이미 로그인 된 ID입니다. 기존의 player의 접속을 끊습니다."), tmpId );
#endif

		//id가 이미 사용중이었다고 보낸다
		SendLoginTryLoginResult( indexId, PRE_LOGIN_ID );

		//캐릭터 삭제
		//m_playerMgr->ReturnPlayerSpace( tmpPlayer );
	}
}

void DBSession::RecvLobbyCharInsertReadyResult( SPacket& packet )
{
	int index, sessionId;

	packet >> index;
	packet >> sessionId;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvLobbyCharInsertReadyResult()\nSessionID %d번 로그인완벽하게 성공.\n\n"), sessionId );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvLobbyCharInsertReadyResult() SessionID %d번 로그인완벽하게 성공."), sessionId );
#endif

	//세션아이디에 결과값이 들어있는 셈이다
	SendLoginTryLoginResult( index, sessionId );
}

void DBSession::RecvGameCharDataUpdate( SPacket& packet )
{
	int roomNum, count;
	int sessionId, rankPoint, killCount, deathCount;

	packet >> roomNum;
	packet >> count;

	//게임서버로 다시 보낼 패킷( 아직 어떤 데이터를 보내야 하는지 결정이 X )
//  	SPacket sendToGame( DB_TO_GAME_LEVEL_UP_PLAYER );
	//로비로 보낼 패킷
	SPacket sendToLobby( DB_TO_LOBBY_UPDATE_USERDATA );

//  	sendToGame << roomNum << count;
	sendToLobby << roomNum << count;

	for( int i=0; i<count; ++i )
	{
		//데이터 추출
		packet >> sessionId >> rankPoint >> killCount >> deathCount;

		//캐릭터를 받아 온다
		PlayerObj* tmpPlayer = m_playerMgr->FindPlayerBySessionId( sessionId );
		if( tmpPlayer == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("DBSession::RecvGameCharDataUpdate()\nSessionID %d번 player정보를 찾을 수 없습니다.\n\n"), sessionId );
			continue;
		}

		//캐릭터 정보 업데이트
		tmpPlayer->AddRankPoint( rankPoint );
		tmpPlayer->AddKillCount( killCount );
		tmpPlayer->AddDeathCount( deathCount );

		//--------------------------------------------------------------
		//게임서버로 보낼 데이터를 넣어 준다
// 		sendToGame << sessionId;
// 		sendToGame << tmpPlayer->GetRankID();
		//--------------------------------------------------------------
		//로비로 보낼 데이터를 넣어 준다
		sendToLobby << sessionId;
		sendToLobby << tmpPlayer->GetRankID();
		sendToLobby << tmpPlayer->GetRankPoint();
		sendToLobby << tmpPlayer->GetAccumulKillCount();
		sendToLobby << tmpPlayer->GetAccumulDeathCount();
		//--------------------------------------------------------------		
	}

	//게임 서버로 전송!


	//로비로 전송!
 	m_lobby->SendPacketToSrv( sendToLobby );	
}

void DBSession::RecvOtherSrvCharDisconnect( SPacket& packet )
{
	int sessionId;

	packet >> sessionId;

	//id로 캐릭터를 찾아 DB를 업데이트 해 준다
	PlayerObj* tmpPlayer = m_playerMgr->FindPlayerBySessionId( sessionId );
	if( tmpPlayer == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("DBSession::RecvOtherSrvCharDisconnect()\nSessionID %d번 player정보를 찾을 수 없습니다.\n\n"), sessionId );

		return;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("DBSession::RecvOtherSrvCharDisconnect()/n캐릭터%s dataUPdate/n")
					_T("SessionID: %d/ rankID: %d/ rankPoint: %d/ KillCount: %d/ DeathCount: %d/n/n")
					, tmpPlayer->GetUserID()
					, sessionId, tmpPlayer->GetRankID(), tmpPlayer->GetRankPoint(), tmpPlayer->GetAccumulKillCount(), tmpPlayer->GetAccumulDeathCount() );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("DBSession::RecvOtherSrvCharDisconnect() 캐릭터%s dataUPdate\t")
					_T("SessionID: %d/ rankID: %d/ rankPoint: %d/ KillCount: %d/ DeathCount: %d")
					, tmpPlayer->GetUserID()
					, sessionId, tmpPlayer->GetRankID(), tmpPlayer->GetRankPoint(), tmpPlayer->GetAccumulKillCount(), tmpPlayer->GetAccumulDeathCount() );
#endif
	
	if( !tmpPlayer->DBDataUpdate() )
	{
		//로그를 남긴다.
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("DBSession::RecvOtherSrvCharDisconnect()\n캐릭터%s dataUPdate실패.\n\n"), tmpPlayer->GetUserID() );

#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
						_T("DBSession::RecvOtherSrvCharDisconnect() 캐릭터%s dataUPdate실패."), tmpPlayer->GetUserID() );
#endif
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("DBSession::RecvOtherSrvCharDisconnect()\n캐릭터%s 님이 로그아웃 하셨습니다.\n\n"), tmpPlayer->GetUserID() );
#endif

#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
		_T("DBSession::RecvOtherSrvCharDisconnect() 캐릭터%s 님이 로그아웃 하셨습니다."), tmpPlayer->GetUserID() );
#endif

	m_playerMgr->ReturnPlayerSpace( tmpPlayer );
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

BOOL DBSession::SendLobbyChacterLogin( int index, PlayerObj* player )
{
	SPacket sendPacket( DB_TO_LOBBY_CHARACTER_LOGIN );

	sendPacket << index;
	player->PackageMyAllInfo( sendPacket );

	m_lobby->SendPacketToSrv( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLoginTryLoginResult( int indexId, int result )
{
	SPacket sendPacket( DB_TO_LOGIN_LOGINRESULT );

	sendPacket << indexId;
	sendPacket << result;

	m_login->SendPacketToSrv( sendPacket );

	return TRUE;
}

BOOL DBSession::SendLobbyNGameDropPlayer( int sessionID )
{
	SPacket sendPacket( DB_TO_OTHER_DROP_PLAYER );
	
	sendPacket << sessionID;

	//저 아이디의 접속을 끊어버린다.
	m_lobby->SendPacketToSrv( sendPacket );
	m_game->SendPacketToSrv( sendPacket );

	return TRUE;
}

