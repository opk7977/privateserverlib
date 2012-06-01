#include "GameSession.h"
#include "GameProtocol.h"

#include "SrvNet.h"
#include "SLogger.h"
#include "CharMgr.h"
#include "ItemMgr.h"
#include "GameMgr.h"

#include "GameProc.h"


SIMPLEMENT_DYNAMIC(GameSession)
SIMPLEMENT_DYNCREATE(GameSession)

GameSession::GameSession(void)
: m_myCharInfo(NULL)
, m_myGameProc(NULL)
, isEndGame(TRUE)
{
	m_srvNet	= &GetSrvNet;
	m_logger	= &GetLogger;
	m_gameMgr	= &GetGameMgr;
	m_charMgr	= &GetCharMgr;
	m_itemMgr	= &GetItemMgr;
}

GameSession::~GameSession(void)
{
}

void GameSession::Clear()
{
	SSynchronize sync( this );

	m_myCharInfo = NULL;
	m_myGameProc = NULL;
	isEndGame	= TRUE;
}

void GameSession::OnCreate()
{
	SSession::OnCreate();

	SendPacket( SC_GAME_CONNECT_OK );
}

void GameSession::OnDestroy()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		if( m_srvNet->GetSession() == this )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
							_T("GameSession::OnDestroy()\n 서버와의 접속을 끊습니다.\n\n") );
		}
		else
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
							_T("GameSession::OnDestroy()\n 캐릭정보가 없는 세션이 접속을 종료합니다.\n\n") );
		}
	}
	else
	{
		//캐릭터 정보가 있습니다.
		//정상적으로 지웁쉬다..
		if( isEndGame )
		{
			//완전히 게임을 끔
			//lobby서버로 접속을 종료 했음을 알려 준다
			SendToSrvThatPlayerDisconnect();
		}

		//방에서 나를 제거
		if( !m_myGameProc->DelPlayer( this ) )
		{
			//현재 게임 내에 아무도 남지 않았음

			//게임 proc닫음
			m_myGameProc->EndGame();

			//로비로 방의 게임 proc이 닫혔음을 알림?
			SendGameEnd( m_myGameProc->GetGameID() );
		}
		else
		{
			//아직 남아 있는 같이 게임하고 있던 사람들에게 내가 나가는것을 알림
			SendCharDisconnect();
		}

		//캐릭터 공간의 정보를 지워 준다.
		m_charMgr->ReturnCharSpace( m_myCharInfo );
	}

	//세션 초기화
	Clear();

	SSession::OnDestroy();
}

// void GameSession::PackageMyInfo( SPacket& packet )
// {
// 	SSynchronize sync( this );
// 
// 	packet << m_myCharInfo->GetIndexId();
// 	packet << m_myCharInfo->GetState();
// 	packet << m_myCharInfo->GetPosX();
// 	packet << m_myCharInfo->GetPosY();
// 	packet << m_myCharInfo->GetPosZ();
// 	packet << m_myCharInfo->GetDirX();
// 	packet << m_myCharInfo->GetDirY();
// 	packet << m_myCharInfo->GetDirZ();
// 	packet << m_myCharInfo->GetDirInt();
// }

void GameSession::PackageMyNetInfo( SPacket& packet )
{
	//SSynchronize sync( this );

	int size = strlen( m_IPAddr );
	packet << size;
	packet.PutData( m_IPAddr, size );
	packet << m_myGameProc->GetPort();
}

void GameSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case LG_START_GAME:
		RecvLobbyStartGame( packet );
		break;

	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	case CS_GAME_GAME_READY_OK:
		RecvGameReadyOK();
		break;
// 	case CS_GAME_MOVE_CHAR:
// 		RecvMoveChar( packet );
// 		break;
	case CS_GAME_ATTACK:
		RecvGameAttec( packet );
		break;
	case CS_GAME_TRY_ATTACK:
		RecvGameTryAttact( packet );
		break;
// 	case CS_GAME_SYNC:
// 		RecvGameSync( packet );
// 		break;
// 	case CS_GAME_CHANGE_STATE:
// 		RecvGameChangeState( packet );
// 		break;
// 	case CS_GAME_ROTATION:
// 		RecvGameRotation( packet );
// 		break;
	case CS_GAME_CHATTING:
		RecvGameChatting( packet );
		break;
	case CS_GAME_INSTALL_BOOM:
		//
		break;
	case CS_GAME_UNINSTALL_BOOM:
		//
		break;
	case CS_GAME_INSTALL_ITEM:
		//
		break;
	case CS_GAME_GOTO_LOBBY:
		//
		break;

	//==============================================================> Client
	default:
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::PacketParsing()\n(%d번 ID)받은 패킷의 아이디가 유효하지 않습니다.\n\n"), packet.GetID() );
	}
}

//======================================
// 받은 패킷 처리
//======================================
//--------------------------------------
// Lobby Srv와의 커뮤니케이션
//--------------------------------------
void GameSession::RecvLobbyConnectOK()
{
	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvLobbyConnectOK()\n로비서버와 연결에 성공하였습니다\n\n") );

	GetSrvNet.SetSession( this );

// 	SPacket sendPacket;
// 	sendPacket.SetID( GL_CONNECT_SERVER );
// 
// 	GetSrvNet.SendToLobbyServer( sendPacket );
}

void GameSession::RecvLobbyStartGame( SPacket &packet )
{
	SSynchronize Sync( this );

	int roomNum, mapNum, gameMode, playTime, playCount, count;

	packet >> roomNum;		//방번호
	packet >> mapNum;
	packet >> gameMode;		//게임 모드
	packet >> playTime;		//판당 게임시간
	packet >> playCount;	//게임 판수

	packet >> count;		//방의 인원수

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvLobbyStartGame()\n%d번 방에 %d명의 캐릭터가 게임을 시작합니다.\n\n") 
						, roomNum, count );

	//우선 방 번호에 해당하는 게임을 연다
	GameProc* tmpGame = GetGameMgr.FindGame( roomNum );
	if( tmpGame == NULL )
	{
		//게임proc을 열수 없거나 실패했다는 패킷을 보낸다
		SendStartFaild( roomNum );
		return;
	}
	if( !tmpGame->NowIsPlay() )
	{
		//이미 게임이 진행 중이면 실패했다는 패킷을 보낸다.
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvLobbyStartGame()\n%d번 방이 이미 게임 중입니다.\n\n") 
			, roomNum );

		SendStartFaild( roomNum );
		return;
	}

	//방을 얻어 왔고 게임이 시작중이 아니라면 게임을 열고 캐릭터를 저장해 준다

	int sessionId, size, team;
	TCHAR stringID[30];
	//인원수에 맞게 캐릭터를 생성해 준다
	for( int i=0; i<count; ++i )
	{
		ZeroMemory( stringID, 30 );
		packet >> sessionId;
		packet >> size;
		packet.GetData( stringID, size );
		packet >> team;

		//캐릭터 생성
		CharObj* tmpChar = GetCharMgr.GetCharSpace();
		tmpChar->Init();
		tmpChar->SetIndexId( sessionId );
		tmpChar->SetID( stringID );
		tmpChar->SetTeam( team );
		//위치 설정!!
		tmpChar->SetStartPoint( i );
	}

	//======================================
	// 게임 proc을 열기 위해 준비
	//======================================
	//게임의 정보를 셋팅해 준다.
	tmpGame->SetGameStage( mapNum );
	//tmpGame->SetPlayerCount( count );
	tmpGame->SetGameMode( gameMode );
	tmpGame->SetGamePlayTime( playTime );
	tmpGame->SetGamePlayCount( playCount );

	//게임 proc을 활성화 한다
	tmpGame->StartGame();

	//게임을 시작해도 된다는 패킷을 Lobby로 보낸다
	SendStartOK( roomNum );
}

//--------------------------------------
// Client와의 커뮤니케이션
//--------------------------------------
void GameSession::RecvInGame( SPacket &packet )
{
	SSynchronize Sync( this );

	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	if( roomNum == 0 )
	{
		//임시 캐릭터 공간을 만든다
		CharObj* tmpChar = GetCharMgr.GetCharSpace();
		tmpChar->SetIndexId( sessionId );
		tmpChar->SetID( _T("Unknown") );
		tmpChar->SetTeam( (int)(sessionId % 2) );
		tmpChar->SetState( SRV_CHAR_STATE_STAND );
//		tmpChar->SetPosition( (float)(sessionId+10), 0.f, (float)(sessionId+10) );
// 		tmpChar->SetDirection( 0.f, 0.f, 1.f );
// 		tmpChar->SetDirInt( 0 );
	}

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::RecvInGame()\n%d번 캐릭터가 %d번게임으로 입장합니다.\n\n") 
					, sessionId, roomNum );

	//세션 ID입력
	SetSessionID( sessionId );

	//캐릭터를 검색
	m_myCharInfo = GetCharMgr.FindCharAsSessionId( sessionId );
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}

	m_myCharInfo->SetSession( this );
	
	m_myGameProc = GetGameMgr.FindGame( roomNum );
	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n해당 게임 프로세스를 찾지 못했습니다.\n\n") );
		return;
	}

	//방에 나를 등록
	m_myGameProc->AddPlayer( this );

	//방의 사람들의 정보(내 정보 포함)를 나에게보냄
	SendOtherCharInfoToMe();

	//방의 사람들에게 내 정보를 전송
	SendMyCharInfoToInGamePlayer();
}

void GameSession::RecvGameReadyOK()
{
	SSynchronize sync( this );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameReadyOK()\n해당 게임 프로세스를 찾지 못했습니다.\n\n") );
		return;
	}

	//ready count를 하나 올려 준다.
	m_myGameProc->AddReadyCount();
}

// void GameSession::RecvMoveChar( SPacket &packet )
// {
// 	SSynchronize Sync( this );
// 
// 	int state, dirInt;
// 	POINT3 pos, dir;
// 	packet >> state;
// 	//위치
// 	packet >> pos.m_X;
// 	packet >> pos.m_Y;
// 	packet >> pos.m_Z;
// 	//방향
// 	packet >> dir.m_X;
// 	packet >> dir.m_Y;
// 	packet >> dir.m_Z;
// 	packet >> dirInt;
// 
// 	if( m_myCharInfo == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvMoveChar()\n캐릭터 설정에 문제가 있습니다.\n\n") );
// 		return;
// 	}
// 
// 	m_myCharInfo->SetState( state );
// 	m_myCharInfo->SetPosition( pos );
// 	m_myCharInfo->SetDirection( dir.m_X, dir.m_Y, dir.m_Z );
// 	m_myCharInfo->SetDirInt( dirInt );
// 
// 	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 					_T("GameSession::RecvMoveChar()\n")
// 					_T("%s(%d) 캐릭터가 %d번 상태\n")
// 					_T("pos(%f, %f, %f)위치")
// 					_T("/ dir(%f, %f, %f, %i)방향으로 전환\n\n")
// 					, m_myCharInfo->GetID()
// 					, m_myCharInfo->GetIndexId()
// 					, state
// 					, pos.m_X, pos.m_Y, pos.m_Z
// 					, dir.m_X, dir.m_Y, dir.m_Z, dirInt );
// 
// 	SendMoveChar();
// }

void GameSession::RecvGameAttec( SPacket &packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	int weapon, attectedSessionID, damage;
	packet >> weapon;
	packet >> attectedSessionID;
	packet >> damage;

	//피격 대상을 받아 온다
	CharObj* tmpChar = m_myGameProc->FindChar( attectedSessionID );
	if( tmpChar == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s의 피격대상을 찾을 수 없습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}
	//내가 죽은애인지 확인하기
	if( m_myCharInfo->IsDie() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s는 이미 죽었습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//상대 팀인지 확인하기
	if( m_myCharInfo->GetTeam() == tmpChar->GetTeam() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s의 피격대상이 같은 팀입니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//에너지를 달게 한다
	tmpChar->DownHP( damage );

	//피격대상의 피를 확인한다
	if( tmpChar->IsDie() )
	{
		//die패킷
		SendGameDie( weapon, tmpChar );

		//애 죽었으면 방의 killcount를 정리
		m_myGameProc->AddKillCount( tmpChar->GetTeam() );
	}
	else
	{
		//attect패킷
		//모두에게
		SendGameAttact( weapon, tmpChar );

		//맞은 놈에게
		SendGameYouAttact( weapon, damage, tmpChar );
	}
}

void GameSession::RecvGameTryAttact( SPacket &packet )
{
	//ID만 바꿔 준다
	packet.SetID( SC_GAME_TRY_ATTACK );

	SendGameTryAttact( packet );
}

// void GameSession::RecvGameSync( SPacket &packet )
// {
// 	SSynchronize Sync( this );
// 
// 	if( m_myCharInfo == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvMoveChar()\n캐릭터 설정에 문제가 있습니다.\n\n") );
// 		return;
// 	}
// 
// 	POINT3 pos, dir;
// 	//위치
// 	packet >> pos.m_X;
// 	packet >> pos.m_Y;
// 	packet >> pos.m_Z;
// 	//방향
// 	packet >> dir.m_X;
// 	packet >> dir.m_Y;
// 	packet >> dir.m_Z;
// 
// 	m_myCharInfo->SetPosition( pos );
// 	m_myCharInfo->SetDirection( dir );
// 
// 	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 					_T("GameSession::RecvGameSync()\n")
// 					_T("%s(%d) 캐릭터\n")
// 					_T("pos(%f, %f, %f)위치")
// 					_T("/ dir(%f, %f, %f)방향으로 전환\n\n")
// 					, m_myCharInfo->GetID()
// 					, m_myCharInfo->GetIndexId()
// 					, pos.m_X, pos.m_Y, pos.m_Z
// 					, dir.m_X, dir.m_Y, dir.m_Z );
// 
// 	SendGameSync();
// }

// void GameSession::RecvGameChangeState( SPacket &packet )
// {
// 	SSynchronize Sync( this );
// 
// 	int state;
// 	packet >> state;
// 
// 	if( m_myCharInfo == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 						_T("GameSession::RecvGameChangeDir()\n")
// 						_T("캐릭터 정보가 유효하지 않습니다\n\n") );
// 		return;
// 	}
// 
// 	m_myCharInfo->SetState( state );
// 
// 	SendGameChangeState();
// }

// void GameSession::RecvGameRotation( SPacket &packet )
// {
// 	SSynchronize Sync( this );
// 
// 	POINT3 dir;
// 	packet >> dir.m_X >> dir.m_Y >> dir.m_Z;
// 
// 	if( m_myCharInfo == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 						_T("GameSession::RecvGameRotation()\n")
// 						_T("캐릭터 정보가 유효하지 않습니다\n\n") );
// 		return;
// 	}
// 
// 	m_myCharInfo->SetDirection( dir );
// 
// 	SendGameRotation();
// }

void GameSession::RecvGameChatting( SPacket &packet )
{
	SSynchronize Sync( this );

	int size;
	TCHAR tmpChatting[256] = {0,};
	TCHAR Chatting[300] = {0,};

	packet >> size;
	packet.GetData( tmpChatting, size );

	
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\n캐릭터의 정보가 유효하지 않습니다.\n\n") );
		return;
	}

	swprintf_s( Chatting, _T("[%s] %s"), m_myCharInfo->GetID(), tmpChatting );
	size = _tcslen( Chatting )*sizeof( TCHAR );

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );

	SendGameChatting( Chatting, size );
}

//======================================
// 보내는 패킷 처리
//======================================
//--------------------------------------
// Lobby Srv와의 커뮤니케이션
//--------------------------------------

BOOL GameSession::SendStartFaild( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_FAILD );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendStartOK( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_OK );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameEnd( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_GAME_END );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendToSrvThatPlayerDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( GL_PLAYER_DISCONNECT );
	sendPacket << m_myGameProc->GetGameID();	//게임번호(방번호)를 넣고
	sendPacket << m_myCharInfo->GetIndexId();	//sessionId를 넣고

	//로비서버로 캐릭터가 게임을 종료하고 나갔다는 것을 알린다.
	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

//--------------------------------------
// Client와의 커뮤니케이션
//--------------------------------------

BOOL GameSession::SendOtherCharInfoToMe()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//게임내의 모두의 정보를 담는다.
	m_myGameProc->PackageAllPlayerInGame( sendPacket );

	int retval = SendPacket( sendPacket );
	if( retval != sendPacket.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMyCharInfoToInGamePlayer()\n전송된 패킷크기와 패킷의 크기 일치 하지 않습니다.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL GameSession::SendMyCharInfoToInGamePlayer()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;
	
	//방에 나만 있으면 안보내도 된다
// 	if( m_myGameProc->GetNowPlayerCount() <= 1 )
// 		return TRUE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//내 정보만 넣을꺼
	sendPacket << (int)1;
	m_myCharInfo->PackageMyInfo( sendPacket );
	//PackageMyInfo( sendPacket );

	//나 빼고 다 보낸다
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

// BOOL GameSession::SendMoveChar()
// {
// 	SPacket sendPacket;
// 	sendPacket.SetID( SC_GAME_MOVE_CHAR );
// 
// 	POINT3 pos = m_myCharInfo->GetPosition();
// 	POINT3 dir = m_myCharInfo->GetDirection();
// 
// 	sendPacket << m_myCharInfo->GetIndexId();
// 	sendPacket << m_myCharInfo->GetState();
// 	sendPacket << pos.m_X << pos.m_Y << pos.m_Z;
// 	sendPacket << dir.m_X << dir.m_Y << dir.m_Z;
// 	sendPacket << m_myCharInfo->GetDirInt();
// 
// 	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 					_T("GameSession::SendMoveChar()\n")
// 					_T("%s(%d) 캐릭터가 %d번 상태\n")
// 					_T("pos(%f, %f, %f)위치")
// 					_T("/ dir(%f, %f, %f, %i)방향으로 전환\n\n")
// 					, m_myCharInfo->GetID()
// 					, m_myCharInfo->GetIndexId()
// 					, m_myCharInfo->GetState()
// 					, pos.m_X, pos.m_Y, pos.m_Z
// 					, dir.m_X, dir.m_Y, dir.m_Z, m_myCharInfo->GetDirInt() );
// 
// 	if( m_myGameProc == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMoveChar()\n방정보에 문제가 있습니다.\n\n") );
// 		return FALSE;
// 	}
// 
// 	m_myGameProc->SendAllPlayerInGame( sendPacket, this );
// 
// 	return TRUE;
// }

BOOL GameSession::SendGameAttact( int weapon, CharObj* attactedChar )
{
	SPacket sendPacket( SC_GAME_ATTACK );
	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << weapon;
	sendPacket << attactedChar->GetIndexId();

	m_myGameProc->SendAllPlayerInGame( sendPacket, attactedChar->GetSession() );

	return TRUE;
}

BOOL GameSession::SendGameYouAttact( int weapon, int damage, CharObj* attactedChar )
{
	SPacket sendPacket( SC_GAME_YOU_ATTACKED );
	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << weapon;
	sendPacket << damage;

	attactedChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameTryAttact( SPacket& packet )
{
	m_myGameProc->SendAllPlayerInGame( packet, this );

	return TRUE;
}

BOOL GameSession::SendGameDie( int weapon, CharObj* dieChar )
{
	//======================================
	// 남들에게 보냄
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << weapon;
	sendPacket << dieChar->GetIndexId();

	m_myGameProc->SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//======================================
	// 나에게 보냄
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << weapon;

	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

// BOOL GameSession::SendGameSync()
// {
// 	SPacket sendPacket;
// 	sendPacket.SetID( SC_GAME_SYNC );
// 
// 	const POINT3 pos = m_myCharInfo->GetPosition();
// 	const POINT3 dir = m_myCharInfo->GetDirection();
// 	
// 
// 	sendPacket << m_myCharInfo->GetIndexId();
// 	//sendPacket << m_myCharInfo->GetState();
// 	sendPacket << pos.m_X;
// 	sendPacket << pos.m_Y;
// 	sendPacket << pos.m_Z;
// 
// 	sendPacket << dir.m_X;
// 	sendPacket << dir.m_Y;
// 	sendPacket << dir.m_Z;
// 	//sendPacket << m_myCharInfo->GetDirInt();
// 
// 	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 					_T("GameSession::SendGameSync()\n")
// 					_T("%s(%d) 캐릭터\n")
// 					_T("pos(%f, %f, %f)위치\n")
// 					_T("dir(%f, %f, %f)방향으로 전환\n\n")
// 					, m_myCharInfo->GetID()
// 					, m_myCharInfo->GetIndexId()
// 					, pos.m_X, pos.m_Y, pos.m_Z
// 					, dir.m_X, dir.m_Y, dir.m_Z );
// 
// 	if( m_myGameProc == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMoveChar()\n방정보에 문제가 있습니다.\n\n") );
// 		return FALSE;
// 	}
// 
// 	m_myGameProc->SendAllPlayerInGame( sendPacket, this );
// 
// 	return TRUE;
// }

BOOL GameSession::SendGameChatting( TCHAR* chatting, int size )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << size;
	sendPacket.PutData( chatting, size );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n방 게임 proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

// BOOL GameSession::SendGameChangeState()
// {
// 	SPacket sendPacket;
// 	sendPacket.SetID( SC_GAME_CHANGE_STATE );
// 	sendPacket << m_myCharInfo->GetIndexId();
// 	sendPacket << m_myCharInfo->GetState();
// 
// 	if( m_myGameProc == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 						_T("GameSession::SendGameChangeDir()\n")
// 						_T("게임proc정보가 유효하지 않습니다.\n\n") );
// 		return FALSE;
// 	}
// 
// 	m_myGameProc->SendAllPlayerInGame( sendPacket, this );
// 
// 	return TRUE;
// }

// BOOL GameSession::SendGameRotation()
// {
// 	SPacket sendPacket;
// 	sendPacket.SetID( SC_GAME_ROTATION );
// 
// 	POINT3 dir = m_myCharInfo->GetDirection();
// 
// 	sendPacket << m_myCharInfo->GetIndexId();
// 	sendPacket << dir.m_X << dir.m_Y << dir.m_Z;
// 
// 	if( m_myGameProc == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 						_T("GameSession::SendGameRotation()\n")
// 						_T("게임proc정보가 유효하지 않습니다.\n\n") );
// 		return FALSE;
// 	}
// 
// 	m_myGameProc->SendAllPlayerInGame( sendPacket, this );
// 
// 	return TRUE;
// }

BOOL GameSession::SendCharDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHAR_DISCONNECT );

	//자신의 세션 ID를 넣어 준다
	sendPacket << m_myCharInfo->GetIndexId();

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendCharDisconnect()\n")
			_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	//나는 이미 빠져 있으니 모두에게 보내도 된다.
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
