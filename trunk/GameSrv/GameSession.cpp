#include "GameSession.h"
#include "GameProtocol.h"

#include "SrvNet.h"
#include "SLogger.h"
#include "CharMgr.h"
#include "ItemMgr.h"
#include "GameMgr.h"
#include "DataLeader.h"

#include "DBSrvMgr.h"

#include "GameProc.h"
#include "MineItem.h"

SIMPLEMENT_DYNAMIC(GameSession)
SIMPLEMENT_DYNCREATE(GameSession)

SrvNet*		GameSession::m_srvNet	= &GetSrvNet;
SLogger*	GameSession::m_logger	= &GetLogger;
GameMgr*	GameSession::m_gameMgr	= &GetGameMgr;
CharMgr*	GameSession::m_charMgr	= &GetCharMgr;
ItemMgr*	GameSession::m_itemMgr	= &GetItemMgr;
DataLeader*	GameSession::m_document	= &GetDocument;

DBSrvMgr*	GameSession::m_dbMgr	= &GetDBSrv;

GameSession::GameSession(void)
: m_myCharInfo(NULL)
, m_myGameProc(NULL)
, isEndGame(TRUE)
{
}

GameSession::~GameSession(void)
{
}

void GameSession::Clear()
{
	SSynchronize sync( this );

	m_myCharInfo	= NULL;
	m_myGameProc	= NULL;
	isEndGame		= TRUE;
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
	//SSynchronize sync( this );

	packet << m_myCharInfo->GetSessionID();

	int size = strlen( m_IPAddr );
	packet << size;
	packet.PutData( m_IPAddr, size );
	packet << m_myGameProc->GetPort();
}

void GameSession::PacketParsing( SPacket& packet )
{
	SSynchronize Sync( this );

	switch( packet.GetID() )
	{
	case DB_TO_OTHER_CONNECT_OK:
		RecvDBConnectOK();
		break;
	case DB_TO_OTHER_DROP_PLAYER:
		RecvCharDrop( packet );
		break;
	//==============================================================> DBSrv
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case LG_START_GAME:
		RecvLobbyStartGame( packet );
		break;
	case LG_END_GAME_READY_OK:
		RecvLobbyEndReadyOK( packet );
		break;
	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	case CS_GAME_GAME_READY_OK:
		RecvGameReadyOK();
		break;
	case CS_GAME_CHARACTER_SYNC:
		RecvGameCharacterSync( packet );
		break;
	case CS_GAME_CHARACTER_JUMP:
		RecvGameCharacterJump();
		break;
	case CS_GAME_CHARACTER_LAND:
		RecvGameCharacterLand( packet );
		break;
	case CS_GAME_CHAR_CHANGE_OBJ:
		RecvGameCharChangeObj( packet );
		break;
	case CS_GAME_ATTACK:
		RecvGameAttack( packet );
		break;
	case CS_GAME_TRY_ATTACK:
		RecvGameTryAttack( packet );
		break;
	case CS_GAME_JUST_SHOOT:
		RecvGameJustShoot();
		break;
	case CS_GAME_LAY_MINE:
		RecvGameLayMine( packet );
		break;
	case CS_GAME_MINE_HIT:
		RecvGameMineHit( packet );
		break;
	case CS_GAME_WEAPON_CHANGE:
		RecvGameChangeWeapon( packet );
		break;
	case CS_GAME_VISIBLE_HIDE:
		RecvGameVisibleHide();
		break;
	case CS_GAME_INVISIBLE_HIDE:
		RecvGameInvisibleHide();
		break;
	case CS_GAME_SKILL_SCAN_ON:
		RecvGameSkillScanOn();
		break;
	case CS_GAME_SKILL_SCAN_OFF:
		RecvGameSkillScanOff();
		break;
	case CS_GAME_KILL_MYSELF:
		RecvGameKillMyself();
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
	case CS_GAME_RADIO_PLAY:
		RecvGameRadioPlay( packet );
		break;
	case CS_GAME_GOTO_LOBBY:
		RecvGameGotoLobby();
		break;
// 	case CS_GAME_END_GUNSELECT:
// 		RecvGameEndGunSelect();
// 		break;
// 	case CS_GAME_START_TIME_COUNTDOWN:
// 		RecvGameStartCountDown();
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
// DB 서버와의 커뮤니케이션
//--------------------------------------
void GameSession::RecvDBConnectOK()
{
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvDBConnectOK()\nDB서버와 연결에 성공하였습니다\n\n") );

	m_dbMgr->SetSession( this );
}

void GameSession::RecvCharDrop( SPacket &packet )
{
	int sessionId;
	packet >> sessionId;

	CharObj* tmpChar = m_charMgr->FindCharAsSessionId( sessionId );
	//캐릭터를 찾을 수 없다면 그건 로비에 있는 캐릭터니까 신경쓰지 말자
	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvCharDrop()\n") );
		return;
	}

	//아니면 접속을 끊으라는 명령을 보낸다.
	tmpChar->GetSession()->SendGameSelfDisconnect();
}

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
	//SSynchronize Sync( this );

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

	{
		SSynchronize sync( tmpGame );
		
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
		int rankId;
		TCHAR stringID[30];
		//인원수에 맞게 캐릭터를 생성해 준다
		for( int i=0; i<count; ++i )
		{
			ZeroMemory( stringID, 30 );
			packet >> sessionId;
			packet >> size;
			packet.GetData( stringID, size );
			packet >> team;

			packet >> rankId;

			//캐릭터 생성
			CharObj* tmpChar = m_charMgr->GetCharSpace();
			if( tmpChar == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("GameSession::RecvLobbyStartGame()\n캐릭터 공간이 없습니다.\n\n") );
				SendStartFaild( roomNum );
				return;
			}
			//캐릭터 셋팅
			tmpChar->Init();
			tmpChar->SetSessionID( sessionId );
			tmpChar->SetID( stringID );
			tmpChar->SetTeam( team );
			tmpChar->SetRankID( rankId );
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
	}

	//게임을 시작해도 된다는 패킷을 Lobby로 보낸다
	SendStartOK( roomNum );
}

void GameSession::RecvLobbyEndReadyOK( SPacket &packet )
{
	int roomNum;
	packet >> roomNum;

	GameProc* tmpGame = m_gameMgr->FindGame( roomNum );
	if( tmpGame == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvLobbyEndReadyOK()\n%d번 방은 없습니다.\n\n"), roomNum );
		return;
	}

	tmpGame->RecvLobbyEndReadyOK();
}

//--------------------------------------
// Client와의 커뮤니케이션
//--------------------------------------
void GameSession::RecvInGame( SPacket &packet )
{
	//SSynchronize Sync( this );

	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	if( roomNum == 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvInGame()\n0번 방은 존재하지 않습니다.\n\n") );
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
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameReadyOK()\n해당 게임 프로세스를 찾지 못했습니다.\n\n") );
		return;
	}

	//ready count를 하나 올려 준다.
	m_myGameProc->AddReadyCount();
}

void GameSession::RecvGameCharacterSync( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameCharacterSync()\n해당 캐릭터 정보가 없습니다.\n\n") );
		return;
	}

	//캐릭터 정보를 빼고
	float posX, posY, posZ;

	packet >> posX >> posY >> posZ;

	//위치정보 수정
	m_myCharInfo->SetPos( posX, posY, posZ );
}

void GameSession::RecvGameCharacterJump()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterJump()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterJump()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	SendGameCharacterJump();
}

void GameSession::RecvGameCharacterLand( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	int objIndex, charState;

	packet >> objIndex;
	packet >> charState;

	SendGameCharacterLand( objIndex, charState );
}

void GameSession::RecvGameCharChangeObj( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	int objIndex;
	packet >> objIndex;

	SendGameCharChangeObj( objIndex );
}

void GameSession::RecvGameAttack( SPacket &packet )
{
	//SSynchronize Sync( this );

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

	int attectedSessionID, shoutData;
	int dataHead, dataWeapon, dataDamage;
	float posX, posY, posZ, normalX, normalY, normalZ;

	packet >> attectedSessionID;
	packet >> shoutData;
	dataHead	= ( ( shoutData & 0xff000000 ) >> 24 );
	dataWeapon	= ( ( shoutData & 0x00ff0000 ) >> 16 );
	dataDamage	= ( shoutData & 0x0000ffff );

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

	//맞은 애가 죽은애인지 확인하기
	if( tmpChar->IsDie() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s는 이미 죽었습니다.\n\n"), tmpChar->GetID() );
		return;
	}

	//맞은 애가 무적인지 확인하기
	if( tmpChar->IsInvincible() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n캐릭터%s는 무적상태 입니다.\n\n"), tmpChar->GetID() );
		return;
	}

// 	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("받음 : %d\n\n"), GetTickCount() );

	//에너지를 달게 한다
	//죽으면 death가 오른다
	tmpChar->DownHP( dataDamage );

	//attect패킷
	//모두에게
	SendGameAttack( tmpChar, posX, posY, posZ, normalX, normalY, normalZ );

	//맞은 놈에게
	SendGameYouAttack( tmpChar );

	//피격대상의 피를 확인한다
	if( tmpChar->IsDie() )
	{
		//애 죽었으면 방의 killcount를 정리
		m_myGameProc->AddKillCount( tmpChar->GetTeam() );
		//캐릭터 killcount정리
		m_myCharInfo->KillCountUp();

		//캐릭터가 설치한 지뢰가 있으면 지워주고 새로 설치할 수 있게 한다
		m_myGameProc->MineResetTarget( tmpChar->GetSessionID() );

		//die패킷
		SendGameDie( dataHead, dataWeapon, tmpChar );
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

void GameSession::RecvGameLayMine( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameLayMine()\n캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameLayMine()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	float posX, posY, posZ;
	float dirX, dirY, dirZ;

	packet >> posX >> posY >> posZ;
	packet >> dirX >> dirY >> dirZ;

	m_myGameProc->SettingMine( this, posX, posY, posZ, dirX, dirY, dirZ );
}

void GameSession::RecvGameMineHit( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameMineHit()\n캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameMineHit()\n캐릭더 %s의 게임 프로세스를 찾지 못했습니다.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//지뢰의 id를 받고
	int mineId;
	packet >> mineId;

	MineItem* tmpMine = m_myGameProc->FindMine( mineId );
	if( tmpMine == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameMineHit()\n번 지뢰가 존재하지 않습니다.\n\n") );
		return;
	}

	//설치된 지뢰인지 체크
	if( !tmpMine->IsInstall() )
		return;

	//지뢰폭발
	tmpMine->Boom();
}

void GameSession::RecvGameChangeWeapon( SPacket &packet )
{
	int weapon;
	
	packet >> weapon;

	//설정

	SendGameChangeWeapon( weapon );
}

void GameSession::RecvGameVisibleHide()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameVisibleHide()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameVisibleHide()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("%s님이 은신 사용\n\n"), m_myCharInfo->GetID() );
	
// 	//이놈이 지금 NONE상태가 아니면 안됨
// 	if( m_myCharInfo->GetSkillState() != SKILL_NONE )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("GameSession::RecvGameVisibleHide()\n")
// 			_T("넌 지금 기본 상태가 아님\n\n") );
// 		return;
// 	}

	//나 은신 설정
	m_myCharInfo->SetSkillHide();

	GameCharVisibleHide();
}

void GameSession::RecvGameInvisibleHide()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameInvisibleHide()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameInvisibleHide()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("%s님이 은신 취소\n\n"), m_myCharInfo->GetID() );

// 	//은신 상태가 아니면 무시해
// 	if( m_myCharInfo->GetSkillState() != SKILL_HIDE )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("GameSession::RecvGameVisibleHide()\n")
// 			_T("넌 지금 은신 상태가 아님\n\n") );
// 		return;
// 	}

	//나 은신 설정 해제
	m_myCharInfo->SetSkillNone();

	GameCharInvisibleHide();
}

void GameSession::RecvGameSkillScanOn()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameSkillScanOn()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	//나 은신 설정
	m_myCharInfo->SetSkillScan();
}

void GameSession::RecvGameSkillScanOff()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameSkillScanOff()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	//나 은신 해제
	m_myCharInfo->SetSkillNone();
}

void GameSession::RecvGameKillMyself()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameKillMyself()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameKillMyself()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_myCharInfo->DownHP( 100 );

	SendGameDie();
}

void GameSession::RecvGameChangeState( SPacket &packet )
{
	//SSynchronize Sync( this );

	int state, objIndex;
	BOOL isJump;
	packet >> state;
	packet >> isJump;
	packet >> objIndex;

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameChangeDir()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	SendGameChangeState( state, isJump, objIndex );
}

void GameSession::RecvGameAskRevival( SPacket &packet )
{
	//SSynchronize sync( this );

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
	//SSynchronize Sync( this );

	int target, size;
	TCHAR tmpChatting[256] = {0,};
	//TCHAR Chatting[300] = {0,};

	packet >> target;
	packet >> size;
	//256이상은 잘라 버림
	if( size > 256 )
		size = 256;

	packet.GetData( tmpChatting, size );

	
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\n캐릭터의 정보가 유효하지 않습니다.\n\n") );
		return;
	}

	switch( target )
	{
	case CHATTING_ALL:
		//swprintf_s( Chatting, _T("[All][%s] %s"), m_myCharInfo->GetID(), tmpChatting );
		SendGameChatting( tmpChatting );
		break;
	case CHATTING_TEAM:
		//swprintf_s( Chatting, _T("[Team][%s] %s"), m_myCharInfo->GetID(), tmpChatting );
		SendGameTeamChat( tmpChatting );
		break;
	default:
		//이상한 데이터네?
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\ntarget이 유효하지 않습니다.\n\n") );
		return;
	}

// 	size = _tcslen( Chatting )*sizeof( TCHAR );
// 
// 	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );
// 
// 	SendGameChatting( Chatting, size );
}

void GameSession::RecvGameRadioPlay( SPacket &packet )
{
	int index;
	packet >> index;
	//packet.SetID( SC_GAME_RADIO_PLAY );

	//SendGameRadioPlay( packet );
	SendGameRadioPlay( index );
}

void GameSession::RecvGameAttackTower( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttackTower()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttackTower()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	int damege;
	packet >> damege;

	m_myGameProc->TowerDameged( m_myCharInfo->GetTeam(), damege );
}

void GameSession::RecvGameGotoLobby()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameGotoLobby()\n")
			_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameGotoLobby()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	//로비로 알림
	SendGameToLobbyCharGotoLobby();

	//로비로 옮긴다는 표시를 함
	SetGotoLobby();

	//로비로 가라고 알림
	SendGameGotoLobby();
}

//test
void GameSession::RecvGameEndGunSelect()
{
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameEndGunSelect()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_myGameProc->SetIsSelectGun();
}

//test
void GameSession::RecvGameStartCountDown()
{
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameStartCountDown()\n")
			_T("게임 proc정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_myGameProc->SetIsStartGame();
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

	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendStartOK( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_OK );
	sendPacket << roomNum;

	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameEnd( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_GAME_END );
	sendPacket << roomNum;

	m_srvNet->SendToLobbyServer( sendPacket );

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

BOOL GameSession::SendGameCharacterJump()
{
	SPacket sendPacket( SC_GAME_CHARACTER_JUMP );

	sendPacket << m_myCharInfo->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameCharacterLand( int objIndex, int charState )
{
	SPacket sendPacket( SC_GAME_CHARACTER_LAND );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << objIndex;
	sendPacket << charState;

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameCharChangeObj( int objIndex )
{
	SPacket sendPacket( SC_GAME_CHAR_CHANGE_OBJ );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << objIndex;

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameAttack( CharObj* attactedChar, float posX, float posY, float posZ, float normalX, float normalY, float normalZ )
{
	SPacket sendPacket( SC_GAME_ATTACK );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << attactedChar->GetSessionID();

	sendPacket << posX << posY << posZ;
	sendPacket << normalX << normalY << normalZ;

	m_myGameProc->SendAllPlayerInGame( sendPacket, attactedChar->GetSession() );

// 	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("보냄 : %d\n\n"), GetTickCount() );


	return TRUE;
}

BOOL GameSession::SendGameYouAttack( CharObj* attactedChar )
{
	SPacket sendPacket( SC_GAME_YOU_ATTACKED );
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

BOOL GameSession::SendGameDie( BOOL isHead, int weaponType, CharObj* dieChar )
{
	int shoutData = 0;
	shoutData |= ( isHead << 16 );
	shoutData |= weaponType;
	//======================================
	// 남들에게 보냄
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << dieChar->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//======================================
	// 나에게 보냄
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameDie()
{
	//헤드샷은 아니고 무기는 8번
	int shoutData = 0;
	shoutData |= 8;

	//======================================
	// 남들에게 보냄
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	//======================================
	// 나에게 보냄
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameLayMine( float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	SPacket sendPacket( SC_GAME_LAY_MINE );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetTeam();
	sendPacket << posX << posY << posZ;
	sendPacket << dirX << dirY << dirZ;

	//======================================
	// 함께 게임하는 모두에게 보낸다
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket );
	return TRUE;
}

BOOL GameSession::SendGameChangeWeapon( int weapon )
{
	SPacket sendPacket( SC_GAME_WEAPON_CHANGE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << weapon;

	//======================================
	// 함께 게임하는 모두에게 보낸다( 나는 뺀다 )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::GameCharVisibleHide()
{
	SPacket sendPacket( SC_GAME_CHAR_VISIBLE_HIDE );

	sendPacket << m_myCharInfo->GetSessionID();

	//======================================
	// 함께 게임하는 모두에게 보낸다( 나는 뺀다 )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::GameCharInvisibleHide()
{
	SPacket sendPacket( SC_GAME_CHAR_INVISIBLE_HIDE );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetHidePoint();

	//======================================
	// 함께 게임하는 모두에게 보낸다( 나는 뺀다 )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChangeState( int state, BOOL isJump, int objIndex )
{
	SPacket sendPacket( SC_GAME_CHANGE_STATE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << state;
	sendPacket << isJump;
	sendPacket << objIndex;

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChangeDir()\n")
						_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	//======================================
	// 함께 게임하는 모두에게 보낸다( 나는 뺀다 )
	//======================================
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

	//======================================
	// 함께 게임하는 모두에게 보낸다( 나는 뺀다 )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChatting( TCHAR* chatting )
{
	//채팅 문장 만들기
	TCHAR ChattingMsg[300] = {0,};
	swprintf_s( ChattingMsg, _T("[All][%s] %s"), m_myCharInfo->GetID(), chatting );
	int size = _tcslen( ChattingMsg )*sizeof( TCHAR );

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << (int)CHATTING_ALL;
	sendPacket << size;
	sendPacket.PutData( ChattingMsg, size );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n방 게임 proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	//======================================
	// 함께 게임하는 모두에게 보낸다
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameTeamChat( TCHAR* chatting )
{
	//채팅 문장 만들기
	TCHAR ChattingMsg[300] = {0,};
	swprintf_s( ChattingMsg, _T("[Team][%s] %s"), m_myCharInfo->GetID(), chatting );
	int size = _tcslen( ChattingMsg )*sizeof( TCHAR );

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << (int)CHATTING_TEAM;
	sendPacket << size;
	sendPacket.PutData( ChattingMsg, size );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendGameChatting()\n방 게임 proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	//======================================
	// 내 팀에게만 보낸다
	//======================================
	m_myGameProc->SendPacketToMyTeam( m_myCharInfo->GetTeam(), sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameRadioPlay( int index )
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
	SPacket sendPacket( SC_GAME_RADIO_PLAY );
	sendPacket   << m_myCharInfo->GetSessionID();
	sendPacket << index;

	//======================================
	// 내 팀에게만 보낸다
	//======================================
	m_myGameProc->SendPacketToMyTeam( m_myCharInfo->GetTeam(), sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameToLobbyCharGotoLobby()
{
	SPacket sendPacket( GL_GAME_GOTO_LOBBY );

	sendPacket << m_myGameProc->GetGameID();
	sendPacket << m_myCharInfo->GetSessionID();

	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameGotoLobby()
{
	SPacket sendPacket( SC_GAME_GOTO_LOBBY );

	//로비의 ip와 port번호를 넣는다.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameSelfDisconnect()
{
	SSynchronize sync( this );

	SPacket sendPacket( SC_LOBBY_GAME_SELF_DISCONNECT );

	//======================================
	// 나에게만 보낸다
	//======================================
	SendPacket( sendPacket );

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

	//======================================
	// 함께 게임하는 모두에게 보낸다( 나는 어차피 지워졌으니 모두에게 보내도 됨 )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
