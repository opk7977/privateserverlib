#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"
#include "DataLeader.h"
#include "DBSrvMgr.h"

#include "CharMgr.h"
#include "ItemMgr.h"

#include "SPacket.h"
#include "SLogger.h"
#include "SSynchronize.h"

#include "SLogger.h"

SLogger*	GameProc::m_logger		= &GetLogger;
DataLeader* GameProc::m_document	= &GetDocument;
DBSrvMgr*	GameProc::m_dbMgr		= &GetDBSrv;

GameProc::GameProc(void)
{
	m_hStartGame	= ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hStartEvent	= ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hReturnResult	= ::CreateEvent( NULL, TRUE, FALSE, NULL );

	m_logger = &GetLogger;

	//결과 정산함수를 셋팅
	m_GameResult[GAME_MODE_DEATH_MATCH]		= &GameProc::GameEnd_DeathMatch;
	m_GameResult[GAME_MODE_BOOM_MISSION]	= &GameProc::GameEnd_BoomMission;
	m_GameResult[GAME_MODE_HACKING_MISSION]	= &GameProc::GameEnd_HackingMission;

	m_timer.Init();
}

GameProc::~GameProc(void)
{
	WaitForSingleObject( GetThreadHandle(), 1000 );
}

void GameProc::Init( int i )
{
	m_id	= i;
	//각 방의 udp포트 번호
	m_port	= m_document->GameSrvPortNum+i+10;

	Init();
}

void GameProc::Init()
{
	SSynchronize sync( this );

	//정보는 초기화
	m_listPlayer.Clear();
	//ClearItem();
	m_nowIsPlaying		= FALSE;
	m_playerCount		= 8;	//기본...
	m_AttKillCount		= m_DefKillCount	= 0;
	m_isWin				= -1;
	m_AttKillAllCount	= m_DefKillAllCount = 0;
	m_AttWinCount		= m_DefWinCount		= 0;
	m_nowPlayTimeCount	= 0;
	m_TieCount			= 0;
	m_readyCount		= 0;

	ResetEvent( m_hStartGame );
	ResetEvent( m_hStartEvent );
	ResetEvent( m_hReturnResult );
}

BOOL GameProc::Run()
{
	while(1)
	{
		//======================================
		// 시작 신호를 기다린다.
		//======================================
		WaitForSingleObject( m_hStartGame, INFINITE );
		//게임 loop는 한번만 돌아야 하니까 바로 바꿔 준다
		ResetEvent( m_hStartGame );
		//======================================

		//======================================
		// 모든 player가 접속하기를 기다린다.
		//======================================
		WaitForSingleObject( m_hStartEvent, INFINITE );
		//--------------------------------------
		// 게임 시작 packet을 보낸다.
		//--------------------------------------
		SendStartPacket();
		//======================================

		//게임 loop를 돈다
		BOOL isEnd = FALSE;
		while( !isEnd )
		{
			//======================================
			// 게임
			//======================================
			GameRun();

			//======================================
			// 사람이 모두 나가서 끝난거면 그냥 종료
			//======================================
			if( m_listPlayer.IsEmpty() )
				break;

			//======================================
			// 우선은 게임을 멈춤
			// 게임 타임아웃 packet을 보낸다.
			//======================================
			SPacket sendPacket( SC_TIME_OUT );
			SendAllPlayerInGame( sendPacket );
			//======================================

			//======================================
			// 현재 판(?)을 닫음
			// 게임 횟수가 남아 있다면 TRUE를 return
			//======================================
			if( ResetGame() )
			{
				//======================================
				// 게임 다시 시작! 아직 판수가 남음
				// 잠시 대기 후에 다시 시작 패킷을 보낸다.
				//======================================
				WaitTimeLogic( WAIT_GAME_END_TIME );

				SendRestartPacket();
				continue;
			}
			else
			{
				//======================================
				// 게임 끗/ DB서버로 보낸다.
				// 캐릭터마다 승패를 따져 rankPoint를 정산
				//======================================
				int whosWin = -1;
				if( m_AttWinCount > m_DefWinCount )
				{
					//공격팀이 이겼음
					whosWin = 0;
				}
				else if( m_AttWinCount < m_DefWinCount )
				{
					//방어팀이 이겼음
					whosWin = 1;
				}
				else
				{
					//비김
					whosWin = 3;
				}

				(this->*m_GameResult[m_gameMode])(whosWin);
				// 게임의 결과 rankPoint를 DB에 데이터를 보낸다
				SendToDBGameResult();
				isEnd = TRUE;
			}
		}
		//DB서버에와 로비서버에 데이터가 정상적으로 저장되기까지의
		//시간을 기다려줘야 한다
		//로비에서 신호가 오기를 기다린다.
		WaitForSingleObject( m_hReturnResult, INFINITE );

		//이제 끝나기를 5초정도 기다렸다가 끝나는 신호를 클라들에게 전송한다.
		WaitTimeLogic( 5 );

		//클라들에게 로비로 돌아가라는 패킷을 보낸다.
		SendGotoLobbyPacket();

		//게임이 종료됨
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("GameProc::Run()\n%d번 게임이 종료됩니다. 게임 Proc이 일시적으로 닫힙니다.\n\n"), m_id );
#endif
	}

	return TRUE;
}

void GameProc::GameRun()
{
	float frameTime		= 0.f;
	float lifeUpTime	= 0.f;

	SPacket sendPacket;

	//======================================
	// 게임 play시간동안 loop
	//======================================
	while( m_nowPlayTimeCount > 0 )
	{
		//======================================
		// 0.5초 단위 처리
		//======================================
		lifeUpTime += m_timer.GetElapsedTime();
		if( lifeUpTime >= 0.5f )
		{
			//우선 초기화
			lifeUpTime = 0.f;

			//======================================
			// hp를 1씩 올려 준다.
			//======================================
			//부상당한 캐릭터를 모두 1씩 올려 준다
			PlayerHeal();
			//그애들만 보낸다.
			SendPlayerHeal();

			//======================================
			// 지뢰 폭발
			//======================================
		}

		//======================================
		// 1초 단위 처리
		//======================================
		m_timer.ProcessTime();

		frameTime += m_timer.GetElapsedTime();
		if( frameTime >= 1.0f )
		{
			//우선 초기화
			frameTime = 0.f;

			//시간 줄이고
			if( --m_nowPlayTimeCount < 0 )
				break;

			//======================================
			// 시간 처리
			//======================================
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("보낸 시간: %d\n"), m_nowPlayTimeCount );
			//시간 패킷 보내기
			sendPacket.PacketClear();
			sendPacket.SetID( SC_GAME_TIME_COUNTDOWN );
			sendPacket << m_nowPlayTimeCount;
			SendAllPlayerInGame( sendPacket );

			// 만약 지뢰가 1초 단위로 터진다면			//
			// 충돌 체크 전에 폭발체크부터 해 줘야 한다	//

			//======================================
			// 지뢰 충돌체크
			//======================================
			
		}
	}
}

void GameProc::AddReadyCount()
{
	//준비완료 인원이 게임 실행 인원과 같아 지면 게임을 시작할 수 있다.

	SSynchronize sync( this );

	if( ++m_readyCount == m_playerCount )
		SetEvent( m_hStartEvent );
}

BOOL GameProc::StartGame()
{
	//게임 시간을 셋팅
 	m_nowPlayTimeCount	= m_playTime;
 	m_nowIsPlaying		= TRUE;
	
	m_AttKillCount		= m_DefKillCount = 0;
	m_isWin				= -1;

	SetEvent( m_hStartGame );

	return TRUE;
}

BOOL GameProc::ResetGame()
{
// 	//======================================
// 	// 게임 타임아웃 packet을 보낸다.
// 	//======================================
// 	SPacket sendPacket( SC_TIME_OUT );
// 	SendAllPlayerInGame( sendPacket );
// 	//======================================

	//======================================
	// 포인트 정산!
	//======================================
	switch( m_gameMode )
	{
	case GAME_MODE_DEATH_MATCH:
		{
			//킬 /데스 비교
			if( m_AttKillCount > m_DefKillCount )
				++m_AttWinCount;		//어택팀이 이김
			else if( m_AttKillCount < m_DefKillCount )
				++m_DefWinCount;		//디펜스팀이 이김
			else
				++m_TieCount;			//비김
		}
		break;
	case GAME_MODE_BOOM_MISSION:
	case GAME_MODE_HACKING_MISSION:
		{
			// m_isWin확인
			if( m_isWin == GAME_TEAM_ATT )
				++m_AttWinCount;
			else if( m_isWin == GAME_TEAM_DEF )
				++m_DefWinCount;
			else
				++m_TieCount;
		}
		break;
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::ResetGame()\n게임 모드가 유효하지 않습니다\n\n") );
		return FALSE;
	}
	
	//총 킬/ 데스 카운터를 적립해준다.
	m_AttKillAllCount	+= m_AttKillCount;
	m_DefKillAllCount	+= m_DefKillCount;


	//게임을 모두 끝냈으면 FALSE를 return하여 게임을 종료 할 수 있게 한다.
	if( --m_playCount <= 0 )
		return FALSE;

	//게임을 더 해야 하는 경우는 다시 정보를 초기화
	m_nowPlayTimeCount	= m_playTime;
	m_nowIsPlaying		= TRUE;
	m_AttKillCount		= m_DefKillCount = 0;
	m_isWin				= -1;
	m_SendList.Clear();

	//캐릭터의 HP등을 모두 reset!
	CharacterRestart();
	//////////////////////////////////////////////////////////////////////////
	//게임내에서 사용된 item도 초기화 해 줘야 함
	//////////////////////////////////////////////////////////////////////////
	//ClearItem();

	return TRUE;
}

void GameProc::EndGame()
{
	//게임 종료
	m_nowPlayTimeCount = 0;
// 	ResetEvent( m_hReturnResult );
}

void GameProc::WaitTimeLogic( int waitTime /*= WAIT_GAME_END_TIME */)
{
	float frameTime = 0.f;

	//시간이 다 되기 전까지 loop
	while(1)
	{
		//======================================
		// 시간 처리
		//======================================
		m_timer.ProcessTime();

		frameTime += m_timer.GetElapsedTime();
		if( frameTime >= 1.0f )
		{
			//우선 초기화
			frameTime = 0.f;

			//시간 줄이고 다 줄었으면 return
			if( --waitTime <= 0 )
				return;
		}
	}
}

void GameProc::GameEnd_DeathMatch( int winnerTeam )
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	CharObj* tmpObj = NULL;
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpObj = (*iter)->GetMyInfo();

		if( tmpObj->GetTeam() == winnerTeam )
		{
			//이긴팀
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * DEATHMATCH_WINNER_KILL_POINT );
		}
		else
		{
			//진팀/ 혹은 비겼을때
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * DEATHMATCH_LOSER_KILL_POINT );
		}
	}
}

void GameProc::GameEnd_BoomMission( int winnerTeam )
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	CharObj* tmpObj = NULL;
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpObj = (*iter)->GetMyInfo();

		if( tmpObj->GetTeam() == winnerTeam )
		{
			//이긴팀
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_WINNER_KILL_POINT + MISSION_WINNER_INCREASE_POINT );
		}
		else
		{
			//진팀/ 혹은 비겼을때
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_LOSER_KILL_POINT );
		}
	}
}

void GameProc::GameEnd_HackingMission( int winnerTeam )
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	CharObj* tmpObj = NULL;
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpObj = (*iter)->GetMyInfo();

		if( tmpObj->GetTeam() == winnerTeam )
		{
			//이긴팀
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_WINNER_KILL_POINT + MISSION_WINNER_INCREASE_POINT );
		}
		else
		{
			//진팀/ 혹은 비겼을때
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_LOSER_KILL_POINT );
		}
	}
}

BOOL GameProc::SendToDBGameResult()
{
	SPacket sendPacket( GAME_TO_DB_UPDATE_USERDATA );
	//모든 캐릭터의 rankPoint정보를 담자!

	//지금게임의 게임번호(방번호)를 담고
	sendPacket << m_id;
	//우선 인원수부터 담고
	sendPacket << m_listPlayer.GetItemCount();

	//정보를 담는다.
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		(*iter)->GetMyInfo()->PackageMyInfoForDB( sendPacket );
	}

	//DB서버로 데이터를 보낸다.
	m_dbMgr->SendToDBServer( sendPacket );

	return TRUE;
}

void GameProc::AddKillCount( BOOL deathTeam )
{
	SSynchronize sync( this );

	if( deathTeam == GAME_TEAM_ATT )
		++m_DefKillCount;
	else
		++m_AttKillCount;
}

void GameProc::AddPlayer( GameSession* player )
{
	SSynchronize Sync( this );

	if( m_listPlayer.GetItemCount() >= m_playerCount )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::AddPlayer()\n설정된 player수를 넘습니다\n\n") );
		return;
	}

	m_listPlayer.AddItem( player );
}

BOOL GameProc::DelPlayer( GameSession* player )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::DelPlayer()\nlist에 player가 존재하지 않습니다\n\n") );
		return FALSE;
	}

	m_listPlayer.DelItem( player );

	if( m_listPlayer.IsEmpty() )
		return FALSE;

	return TRUE;
}

void GameProc::CharacterRestart()
{
	SSynchronize sync( this );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; m_listPlayer.IsEnd( iter ); ++iter )
	{
		//HP회복!
		(*iter)->GetMyInfo()->SetAlive();
	}
}

void GameProc::ClearItem()
{
	if( m_listItem.IsEmpty() )
		return;

	std::list<ItemObj*>::iterator iter = m_listItem.GetHeader();
	for( ; !m_listItem.IsEnd( iter ); ++iter )
	{
		GetItemMgr.ReturnSpace( (*iter) );
	}

	m_listItem.Clear();
}

CharObj* GameProc::FindChar( int sessionID )
{
	SSynchronize Sync( this );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter)->GetSessionID() == sessionID )
			return (*iter)->GetMyInfo();
	}

	//없으면 NULL
	return NULL;
}

void GameProc::PlayerHeal()
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter)->GetMyInfo()->HPUpOnePoint() )
		{
			//hp를 올린애는 list에 넣어 준다
			m_SendList.AddItem( (*iter) );
		}
	}
}

void GameProc::SendPlayerHeal()
{
	SPacket sendPacket;

	std::list<GameSession*>::iterator iter = m_SendList.GetHeader();
	for( ; !m_SendList.IsEnd( iter ); ++iter )
	{
		sendPacket.PacketClear();
		sendPacket.SetID( SC_GAME_REMAIN_HP );

		sendPacket << (*iter)->GetMyInfo()->GetHP();
		(*iter)->SendPacket( sendPacket );
	}

	m_SendList.Clear();
}

BOOL GameProc::SendStartPacket()
{
	SPacket sendPacket( SC_GAME_START_GAME );
	
	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendRestartPacket()
{
	SPacket sendPacket( SC_GAME_RESTART );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGotoLobbyPacket()
{
	SPacket sendPacket( SC_GAME_GOTO_LOBBY );

	//로비의 ip와 port번호를 넣는다.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

void GameProc::SendAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	SSynchronize Sync( this );
	
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter) == me )
			continue;

		(*iter)->SendPacket( packet );
	}
}

void GameProc::SendPacketToMyTeam( int team, SPacket& packet, GameSession* me/* = NULL*/ )
{
	SSynchronize Sync( this );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//나는 안보내도 됨
		if( (*iter) == me )
			continue;

		//자신의 팀이면 보낸다
		if( (*iter)->GetMyInfo()->GetTeam() == team )
		(*iter)->SendPacket( packet );
	}
}

void GameProc::PackageAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	SSynchronize Sync( this );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter) == me )
			continue;

		(*iter)->PackageMyNetInfo( packet );
	}
}
