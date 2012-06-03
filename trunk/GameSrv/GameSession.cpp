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

SrvNet*		GameSession::m_srvNet	= &GetSrvNet;
SLogger*	GameSession::m_logger	= &GetLogger;
GameMgr*	GameSession::m_gameMgr	= &GetGameMgr;
CharMgr*	GameSession::m_charMgr	= &GetCharMgr;
ItemMgr*	GameSession::m_itemMgr	= &GetItemMgr;

GameSession::GameSession(void)
: m_myCharInfo(NULL)
, m_myGameProc(NULL)
, isEndGame(TRUE)
{
// 	m_srvNet	= &GetSrvNet;
// 	m_logger	= &GetLogger;
// 	m_gameMgr	= &GetGameMgr;
// 	m_charMgr	= &GetCharMgr;
// 	m_itemMgr	= &GetItemMgr;
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
			m_myGameProc->Init();

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

void GameSession::PackageMyNetInfo( SPacket& packet )
{
	SSynchronize sync( this );

	packet << m_myCharInfo->GetSessionID();

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
	case CS_GAME_ATTACK:
		RecvGameAttack( packet );
		break;
	case CS_GAME_TRY_ATTACK:
		RecvGameTryAttack( packet );
		break;
	case CS_GAME_JUST_SHOOT:
		//
		break;
	case CS_GAME_CHANGE_STATE:
		RecvGameChangeState( packet );
		break;
	case CS_GAME_ASK_REVIVAL:
		RecvGameAskRevival( packet );
		break;
	case CS_GAME_CHATTING:
		RecvGameChatting( packet );
		break;
	case CS_GAME_INSTALL_BOOM:
		//
		break;
	case CS_GAME_UNINSTALL_BOOM:
		//
		break;
	case CS_GAME_RADIO_PLAY:
		RecvGameRadioPlay( packet );
		break;
	case CS_GAME_INSTALL_ITEM:
		//
		break;
// 	case CS_GAME_GOTO_LOBBY:
// 		//
// 		break;

	//==============================================================> Client
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::PacketParsing()\n(%d번 ID)받은 패킷의 아이디가 유효하지 않습니다.\n\n"), packet.GetID() );
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
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvLobbyConnectOK()\n로비서버와 연결에 성공하였습니다\n\n") );

	m_srvNet->SetSession( this );
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

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvLobbyStartGame()\n%d번 방에 %d명의 캐릭터가 게임을 시작합니다.\n\n") 
						, roomNum, count );

	//우선 방 번호에 해당하는 게임을 연다
	GameProc* tmpGame = m_gameMgr->FindGame( roomNum );
	if( tmpGame == NULL )
	{
		//게임proc을 열수 없거나 실패했다는 패킷을 보낸다
		SendStartFaild( roomNum );
		return;
	}
	if( tmpGame->NowIsPlay() )
	{
		//이미 게임이 진행 중이면 실패했다는 패킷을 보낸다.
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
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
		CharObj* tmpChar = m_charMgr->GetCharSpace();
		if( tmpChar == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("GameSession::RecvLobbyStartGame()\n캐릭터 공간이 없습니다.\n\n") );
			SendStartFaild( roomNum );
			return;
		}
		tmpChar->Init();
		tmpChar->SetSessionID( sessionId );
		tmpChar->SetID( stringID );
		tmpChar->SetTeam( team );
	}

	//======================================
	// 게임 proc을 열기 위해 준비
	//======================================
	//게임의 정보를 셋팅해 준다.
	tmpGame->SetGameStage( mapNum );
	tmpGame->SetPlayerCount( count );
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvInGame()\n%d번 캐릭터 임시 캐릭터 공간을 만듭니다.\n\n") 
						, sessionId );

		//임시 캐릭터 공간을 만든다
		CharObj* tmpChar = m_charMgr->GetCharSpace();
		tmpChar->SetSessionID( sessionId );
		tmpChar->SetID( _T("Unknown") );
		tmpChar->SetTeam( (int)(sessionId % 2) );
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::RecvInGame()\n%d번 캐릭터가 %d번게임으로 입장합니다.\n\n") 
					, sessionId, roomNum );

	//세션 ID입력
	SetSessionID( sessionId );

	//캐릭터를 검색
	m_myCharInfo = m_charMgr->FindCharAsSessionId( sessionId );
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}
	m_myCharInfo->SetSession( this );
	
	m_myGameProc = m_gameMgr->FindGame( roomNum );
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n해당 게임 프로세스를 찾지 못했습니다.\n\n") );
		return;
	}
	//방에 나를 등록
	m_myGameProc->AddPlayer( this );

	//방의 사람들의 정보를 나에게보냄( 내 정보는 필요 없음 )
	SendOtherCharInfoToMe();

	//방의 사람들에게 내 정보를 전송
	SendMyCharInfoToInGamePlayer();
}

void GameSession::RecvGameReadyOK()
{
	SSynchronize sync( this );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameReadyOK()\n해당 게임 프로세스를 찾지 못했습니다.\n\n") );
		return;
	}

	//ready count를 하나 올려 준다.
	m_myGameProc->AddReadyCount();
}

void GameSession::RecvGameAttack( SPacket &packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	if( !m_myGameProc->NowIsPlay() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n게임이 활성화 되어 있지 않습니다.\n\n") );
		return;
	}

	int attectedSessionID, damage;
	float posX, posY, posZ, normalX, normalY, normalZ;
	BOOL isHead;
	packet >> isHead;
	packet >> attectedSessionID;
	packet >> damage;

	packet >> posX >> posY >> posZ;
	packet >> normalX >> normalY >> normalZ;

	//피격 대상을 받아 온다
	CharObj* tmpChar = m_myGameProc->FindChar( attectedSessionID );
	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s의 피격대상을 찾을 수 없습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}
	//내가 죽은애인지 확인하기
	if( m_myCharInfo->IsDie() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s는 이미 죽었습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//상대 팀인지 확인하기
	if( m_myCharInfo->GetTeam() == tmpChar->GetTeam() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s의 피격대상이 같은 팀입니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//에너지를 달게 한다
	tmpChar->DownHP( damage );

	//attect패킷
	//모두에게
	SendGameAttack( isHead, tmpChar, posX, posY, posZ, normalX, normalY, normalZ );

	//맞은 놈에게
	SendGameYouAttack( isHead, tmpChar );

	//피격대상의 피를 확인한다
	if( tmpChar->IsDie() )
	{
		//애 죽었으면 방의 killcount를 정리
		m_myGameProc->AddKillCount( tmpChar->GetTeam() );
		//캐릭터 killcount정리
		m_myCharInfo->KillCountUp();
		//캐릭터 deathcount정리
		tmpChar->DeathCountUp();

		//die패킷
		SendGameDie( isHead, tmpChar );
	}
}

void GameSession::RecvGameTryAttack( SPacket &packet )
{
	//ID만 바꿔 준다
	packet.SetID( SC_GAME_TRY_ATTACK );

	SendGameTryAttact( packet );
}

void GameSession::RecvGameJustShoot()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameJustShoot()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	SPacket sendPacket( SC_GAME_JUST_SHOOT );

	sendPacket << m_myCharInfo->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );
}

void GameSession::RecvGameChangeState( SPacket &packet )
{
	SSynchronize Sync( this );

	int state;
	packet >> state;

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChangeDir()\n")
						_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	SendGameChangeState( state );
}

void GameSession::RecvGameAskRevival( SPacket &packet )
{
	SSynchronize sync( this );

	//우선 애를 부활시켜 준다.
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameAskRevival()\n")
						_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_myCharInfo->SetAlive();

	int startPoingIndex;

	packet >> startPoingIndex;

	SendGameCharRevival( startPoingIndex );
}

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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\n캐릭터의 정보가 유효하지 않습니다.\n\n") );
		return;
	}

	swprintf_s( Chatting, _T("[%s] %s"), m_myCharInfo->GetID(), tmpChatting );
	size = _tcslen( Chatting )*sizeof( TCHAR );

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );

	SendGameChatting( Chatting, size );
}

void GameSession::RecvGameRadioPlay( SPacket &packet )
{
	packet.SetID( SC_GAME_RADIO_PLAY );

	SendGameRadioPlay( packet );
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
	sendPacket << m_myCharInfo->GetSessionID();	//sessionId를 넣고	
	sendPacket << m_myCharInfo->GetTeam();

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

	int count = m_myGameProc->GetConnectPlayerCount();

	//나만 있으면 안보내도 된다.
	if( count <= 1 )
		return TRUE;

	//게임내의 모두의 정보를 담는다.
	sendPacket << count-1;
	m_myGameProc->PackageAllPlayerInGame( sendPacket, this );

	int retval = SendPacket( sendPacket );
	if( retval != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMyCharInfoToInGamePlayer()\n전송된 패킷크기와 패킷의 크기 일치 하지 않습니다.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL GameSession::SendMyCharInfoToInGamePlayer()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;
	
	//방에 나만 있으면 안보내도 된다
	if( m_myGameProc->GetConnectPlayerCount() <= 1 )
		return TRUE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//내 정보만 넣을꺼
	sendPacket << (int)1;
	PackageMyNetInfo( sendPacket );

	//나 빼고 다 보낸다
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameAttack( BOOL isHead, CharObj* attactedChar, float posX, float posY, float posZ, float normalX, float normalY, float normalZ )
{
	SPacket sendPacket( SC_GAME_ATTACK );
	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << attactedChar->GetSessionID();

	sendPacket << posX << posY << posZ;
	sendPacket << normalX << normalY << normalZ;

	m_myGameProc->SendAllPlayerInGame( sendPacket, attactedChar->GetSession() );

	return TRUE;
}

BOOL GameSession::SendGameYouAttack( BOOL isHead, CharObj* attactedChar )
{
	SPacket sendPacket( SC_GAME_YOU_ATTACKED );
	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << attactedChar->GetHP();		//남은 HP

	attactedChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameTryAttact( SPacket& packet )
{
	m_myGameProc->SendAllPlayerInGame( packet, this );

	return TRUE;
}

BOOL GameSession::SendGameDie( BOOL isHead, CharObj* dieChar )
{
	//======================================
	// 남들에게 보냄
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << dieChar->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//======================================
	// 나에게 보냄
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameChangeState( int state )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHANGE_STATE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << state;

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChangeDir()\n")
						_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameCharRevival( int pointIndex )
{
	SPacket sendPacket( SC_GAME_CHAR_REVIVAL );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << pointIndex;

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameCharRevival()\n")
						_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChatting( TCHAR* chatting, int size )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << size;
	sendPacket.PutData( chatting, size );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n방 게임 proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameRadioPlay( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendGameRadioPlay()\n캐릭터 정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendGameRadioPlay()\n캐릭터 %s의 게임 proc정보가 유효하지 않습니다.\n\n"), m_myCharInfo->GetID() );
		return FALSE;
	}

	//자신의 팀에만 보낸다
	m_myGameProc->SendPacketToMyTeam( m_myCharInfo->GetTeam(), packet, this );

	return TRUE;
}

BOOL GameSession::SendCharDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHAR_DISCONNECT );

	//자신의 세션 ID를 넣어 준다
	sendPacket << m_myCharInfo->GetSessionID();

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendCharDisconnect()\n")
						_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	//나는 이미 빠져 있으니 모두에게 보내도 된다.
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
