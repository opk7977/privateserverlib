#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"
#include "DataLeader.h"
#include "DBSrvMgr.h"

#include "CharMgr.h"
#include "ItemMgr.h"
#include "MineItem.h"
#include "CharMgr.h"

#include "SPacket.h"
#include "SLogger.h"
#include "SSynchronize.h"

#include "SLogger.h"

SLogger*		GameProc::m_logger		= &GetLogger;
DataLeader*		GameProc::m_document	= &GetDocument;
DBSrvMgr*		GameProc::m_dbMgr		= &GetDBSrv;
ItemMgr*		GameProc::m_itemMgr		= &GetItemMgr;
CharMgr*		GameProc::m_charMgr		= &GetCharMgr;

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

	//지뢰를 위한 Sync객체
	m_mineCritical = new SServerObj;
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

	//지뢰정보 초기화
	MineClear();

	//정보는 초기화
	m_listPlayer.Clear();
	
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

	//test
	m_isSelectCountDown	= FALSE;
	m_isGameCountDown	= FALSE;
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
		// 게임 준비가 완료 되었다는 신호를 보낸다
		//--------------------------------------
		//SendReadyPacket();
		SendStartPacket();
		//--------------------------------------
		// 무기 고를 시간 줍니다.
		//--------------------------------------
		//CountDownLogin( 29, SENDTIME_TEN, LASTTIME_THREE );
		CountDownLogic( 19 );
		//--------------------------------------
		// 게임 시작 packet을 보낸다.
		//--------------------------------------
		//SendStartPacket();
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
		//m_nowIsPlaying가 FALSE이면 애들이 다 나가서 꺼진거니까 로비의 결과를 기다리지 않는다
		if( !m_nowIsPlaying )
			continue;

		WaitForSingleObject( m_hReturnResult, INFINITE );

		//이제 끝나기를 5초정도 기다렸다가 끝나는 신호를 클라들에게 전송한다.
		WaitTimeLogic( WAIT_GAME_END_TIME );

		//캐릭터 상태가 로비로 돌아가는 준비중이라는 표시를 해 준다
		//SetGotoLobby();

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
	float TimeOneSec			= 0.f;


	float ElapsedTime;

	SPacket sendPacket;

	//======================================
	// 게임 play시간동안 loop
	//======================================
	while( m_nowPlayTimeCount > 0 )
	{
		m_timer.ProcessTime();
		ElapsedTime = m_timer.GetElapsedTime();

		//======================================
		// 1초 단위 처리_시간 처리
		//======================================
// 		if( m_isGameCountDown )
// 		{
			TimeOneSec += ElapsedTime;
			if( TimeOneSec >= 1.0f )
			{
				//우선 초기화
				TimeOneSec = 0.f;

				//시간 줄이고
				if( --m_nowPlayTimeCount < 0 )
					break;

				//--------------------------------------
				// 시간 처리
				//--------------------------------------
				//m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("보낸 시간: %d\n"), m_nowPlayTimeCount );
				//시간 패킷 보내기
				sendPacket.PacketClear();
				sendPacket.SetID( SC_GAME_TIME_COUNTDOWN );
				sendPacket << m_nowPlayTimeCount;
				SendAllPlayerInGame( sendPacket );
			}
// 		}
		//======================================
		// 프레임 단위로 처리
		//======================================

		//======================================
		// HP
		//======================================
		CountUpCharHP( ElapsedTime );

		//======================================
		// 은신/ 스캔 수치
		//--------------------------------------
		CountSkillPoints( ElapsedTime );

		//======================================
		// 무적처리
		//--------------------------------------
		// 캐릭터 무적상태인 애들 시간을 줄이고
		// 시간이 다 되면 무적을 풀어 준다
		//--------------------------------------
		CountDownCharInvincible();
		//======================================

		//======================================
		// 지뢰 처리
		//--------------------------------------
		// 터질 예정인 애들 시간 줄이기
		//--------------------------------------
		CountDownRunningMine();
		//--------------------------------------
		// 폭발한 지뢰 충돌체크
		//--------------------------------------
		ExplosionMineCrashCheck();
		//--------------------------------------
		// 캐릭터와 충돌체크
		//--------------------------------------
		MineCrashCheck();
		//======================================
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
// 	m_SendList.Clear();

	//캐릭터의 HP등을 모두 reset!
	CharacterRestart();
	//지뢰정보 reset!
	MineReset();

	return TRUE;
}

void GameProc::EndGame()
{
	//게임 종료
	m_nowPlayTimeCount = 0;
// 	ResetEvent( m_hReturnResult );
}

void GameProc::WaitTimeLogic( int waitTime )
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
			if( --frameTime <= 0 )
				return;
		}
	}
}

// void GameProc::CountDownLogin( int waitTime, SendTime sendTime, LastTime lastTime )
// {
// 	float framTime = 0.f;
// 	TCHAR TmpChar[64]={0,};
// 
// 	while(1)
// 	{
// 		//======================================
// 		// 시간 처리
// 		//======================================
// 		m_timer.ProcessTime();
// 
// 		framTime += m_timer.GetElapsedTime();
// 		if( framTime > 1.0f )
// 		{
// 			//우선 초기화
// 			framTime = 0.f;
// 
// 			//정해진 시간단위로 남은 시간을 공지형식으로 보낸다
// 			if( (waitTime % sendTime) == 0 )
// 			{
// 				ZeroMemory( TmpChar, 64*2 );
// 				_stprintf_s( TmpChar, 64, _T("[Notice] %d초 남았습니다."), waitTime );
// 
// 				SendNotice( TmpChar );
// 			}
// 
// 			//마지막 카운트는 그냥 숫사를 보낸다
// 			if( waitTime <= lastTime )
// 			{
// 				SendTimeRemain( waitTime );
// 			}
// 
// 			//시간 줄이고 다 줄었으면 return
// 			if( --waitTime <= 0 )
// 				return;
// 		}
// 	}
// }

void GameProc::CountDownLogic( int waitTime )
{
	float framTime = 0.f;
	TCHAR TmpChar[64]={0,};
	int m_wait = waitTime;

	SendTimeRemain( waitTime );

	while(1)
	{
		//======================================
		// 시간 처리
		//======================================
		m_timer.ProcessTime();

// 		if( !m_isSelectCountDown )
// 			m_wait = waitTime;

		framTime += m_timer.GetElapsedTime();
		if( framTime > 1.0f )
		{
			//우선 초기화
			framTime = 0.f;

			//시간 줄이고 다 줄었으면 return
			if( --m_wait <= 0 )
				return;
			
			SendTimeRemain( m_wait );
		}
	}
}

void GameProc::MineClear()
{
	SSynchronize sync( m_mineCritical );

	if( m_mapMine.IsEmpty() )
		return;

	POSITION pos = m_mapMine.GetStartPosition();
	MineItem* tmp;
	while( pos )
	{
		tmp = m_mapMine.GetNextValue( pos );
		//모든 공간을 반납하고 나서!
		m_itemMgr->ReturnMineSpace( tmp );
	}
	//다 지워
	m_mapMine.RemoveAll();
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
	SSynchronize PlayerSync( &m_listPlayer );
	SSynchronize MineSync( m_mineCritical );

	if( m_listPlayer.GetItemCount() >= m_playerCount )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::AddPlayer()\n설정된 player수를 넘습니다\n\n") );
		return;
	}

	//지뢰공간도 만들어 준다
	MineItem* tmp = m_itemMgr->GetMineItemSpace();
	if( tmp == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::AddPlayer()\n지뢰 공간을 받아 올 수 없습니다.\n\n") );
		return;
	}

	tmp->Init();
	tmp->SetMineSpace( player->GetSessionID(), player->GetMyInfo()->GetTeam() );
	m_mapMine[player->GetSessionID()] = tmp;

	m_listPlayer.AddItem( player );
}

BOOL GameProc::DelPlayer( GameSession* player )
{
	SSynchronize PlayerSync( &m_listPlayer );
	SSynchronize MineSync( m_mineCritical );

	if( m_listPlayer.IsEmpty() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::DelPlayer()\nlist에 player가 존재하지 않습니다\n\n") );
		return FALSE;
	}

	m_listPlayer.DelItem( player );
	m_mapMine.RemoveKey( player->GetSessionID() );

	if( m_listPlayer.IsEmpty() )
		return FALSE;

	return TRUE;
}

void GameProc::CharacterRestart()
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; m_listPlayer.IsEnd( iter ); ++iter )
	{
		//HP회복!
		(*iter)->GetMyInfo()->SetAlive();
	}
}

BOOL GameProc::MineResetTarget( int sessionID )
{
	SSynchronize sync( m_mineCritical );

	MineItem* tmpMine = NULL;

	if( !m_mapMine.Lookup( sessionID, tmpMine ) )
	{
		return FALSE;
	}

	tmpMine->Reset();

	return TRUE;
}

void GameProc::MineReset()
{
	//SSynchronize Sync( &m_boomSoon );
	SSynchronize sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();
	while( pos )
	{
		MineItem* tmp = m_mapMine.GetNextValue( pos );
		tmp->Reset();
	}
}

CharObj* GameProc::FindChar( int sessionID )
{
	SSynchronize Sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter)->GetSessionID() == sessionID )
			return (*iter)->GetMyInfo();
	}

	//없으면 NULL
	return NULL;
}

MineItem* GameProc::FindMine( int sessionID )
{
	//SSynchronize sync( &m_boomSoon );
	SSynchronize sync( m_mineCritical );

	MineItem* tmpMine = NULL;
	if( !m_mapMine.Lookup( sessionID, tmpMine ) )
	{
		return NULL;
	}

	return tmpMine;
}

void GameProc::CountUpCharHP( float elaps )
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	//모든 캐릭터를 돌면서 올려줄 스킬 수치는 올려준다
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//증감의 변화가 있다면 TRUE를 return받아 hp를 전송한다
		if( (*iter)->GetMyInfo()->HPUpOnePoint( elaps ) )
			SendHPCountUp( (*iter) );
	}
}

BOOL GameProc::SendHPCountUp( GameSession* session )
{
	SPacket sendPacket( SC_GAME_REMAIN_HP );

	sendPacket << session->GetMyInfo()->GetHP();

	session->SendPacket( sendPacket );

	return TRUE;
}

void GameProc::CountSkillPoints( float elaps )
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	//모든 캐릭터를 돌면서 올려줄 스킬 수치는 올려준다
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//증감의 변화가 있다면 TRUE를 return받아 스킬 포인트를 전송한다
		if( (*iter)->GetMyInfo()->CountSkillPoint( elaps ) )
			SendSkillPoint( (*iter) );
	}
}

BOOL GameProc::SendSkillPoint( GameSession* session )
{
	CharObj* tmpChar = session->GetMyInfo();

	int hidePoint = tmpChar->GetHidePoint();
	int scanPoint = tmpChar->GetScanPoint();
	if( tmpChar->GetSkillState() == SKILL_HIDE )
	{
		//지금이 Hide상태이고
		if( hidePoint <= 0 )
		{
			//포인트를 모두 소비했다면 스킬 취소
			tmpChar->SetSkillNone();
			SendHideOff( session );
		}
	}
	else if( tmpChar->GetSkillState() == SKILL_SCAN )
	{
		//지금이 scan상태이고
		if( scanPoint <= 0 )
		{
			//포인트를 모두 소비했다면 스킬 취소
			tmpChar->SetSkillNone();
			SendScanOff( session );
		}
	}

	//그리고 skill의 포인트를 전송
	SPacket sendPacket( SC_GAME_SKILL_POINT );

	sendPacket << hidePoint;
	sendPacket << scanPoint;

	session->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameProc::SendHideOff( GameSession* session )
{
	//======================================
	// 해당 세션에게 너 은신 풀렸다고 알림
	//======================================
	SPacket sendPacket( SC_GAME_TIMEOUT_HIDE );
	session->SendPacket( sendPacket );

	//======================================
	// 다른 모두에게 은신이 풀렸다는 것을 알림
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_CHAR_INVISIBLE_HIDE );
	sendPacket << session->GetMyInfo()->GetSessionID();
	SendAllPlayerInGame( sendPacket, session );

	return TRUE;
}

BOOL GameProc::SendScanOff( GameSession* session )
{
	//======================================
	// 해당 세션에게 너 스캔 풀렸다고 알림
	//======================================
	SPacket sendPacket( SC_GAME_TIMEOUTL_SCAN );
	session->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameProc::SettingMine( GameSession* session, float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	SSynchronize sync( m_mineCritical );

	MineItem* tmpMine;

	//sessionID확인
	if( !m_mapMine.Lookup( session->GetSessionID(), tmpMine ) )
	{
		session->SendPacket( SC_GAME_LAY_MINE_FAILD );
		return TRUE;
	}

	//지뢰를 이미 사용했다면 사용할 수 없다
	if( !tmpMine->CanUse() )
	{
		session->SendPacket( SC_GAME_LAY_MINE_FAILD );
		return TRUE;
	}

	//아니면 사용하자
	tmpMine->SetMine( posX, posY, posZ, dirX, dirY, dirZ );

	//설치 성공
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("GameProc::SettingMine()\n캐릭더 %s이 %.2f/%.2f/%.2f/위치에 지뢰설치.\n\n"),
		session->GetMyInfo()->GetID(), posX, posY, posZ );


	//지뢰 설치 성공
	session->SendGameLayMine( posX, posY, posZ, dirX, dirY, dirZ );

	//보낸후부터 지뢰체크하는 flag를 설정한다.
	tmpMine->SetInstall();

	return TRUE;
}

// BOOL GameProc::SettingMine( int sessionId, float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
// {
// 	SSynchronize sync( m_mineCritical );
// 
// 	MineItem* tmpMine;
// 
// 	//sessionID확인
// 	if( !m_mapMine.Lookup( sessionId, tmpMine ) )
// 		return FALSE;
// 
// 	//지뢰를 이미 사용했다면 사용할 수 없다
// 	if( !tmpMine->CanUse() )
// 		return FALSE;
// 
// 	//아니면 사용하자
// 	tmpMine->SetMine( posX, posY, posZ, dirX, dirY, dirZ );
// 
// 	//설치 성공
// 
// 	return TRUE;
// }

void GameProc::CountDownRunningMine()
{
	float elapsed = m_timer.GetElapsedTime();

	SSynchronize Sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();

	while( pos )
	{
		MineItem* tmpMine = m_mapMine.GetNextValue( pos );

		//설치된 지뢰가 맞는가?
		if( !tmpMine->IsInstall() )
			continue;

		//실행중인게 아니면 안한다
		if( !tmpMine->IsRun() )
			continue;

		//시간을 줄여 준다
		tmpMine->DownTimer( elapsed );
	}
}

void GameProc::ExplosionMineCrashCheck()
{
	SSynchronize sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();
	std::list<GameSession*>::iterator iterChar;
	
	while( pos )
	{
		MineItem* tmpMine = m_mapMine.GetNextValue( pos );

		//설치된 지뢰가 맞는가?
		if( !tmpMine->IsInstall() )
			continue;

		//실행중인게 아니면 안한다
		if( !tmpMine->IsRun() )
			continue;

		//지금 터져도 되?
		if( !tmpMine->IsBoom() )
			continue;

		//아니면 터뜨리고 전송
		//게임에 있는 모두에게 전송
		SendGameExplosionMine( tmpMine );

		//폭발 충돌체크하자
		for( iterChar = m_listPlayer.GetHeader(); !m_listPlayer.IsEnd( iterChar ); ++iterChar )
		{
			//지뢰맞은애
			CharObj* tmpChar = (*iterChar)->GetMyInfo();
			//죽은 애는 패스
			if( tmpChar->IsDie() )
				continue;

			//무적인 애도 패스
			if( tmpChar->IsInvincible() )
				continue;

			POINT3 charpos = tmpChar->GetPos();

			//지뢰주인을 찾고
			CharObj* mineMaster = m_charMgr->FindCharAsSessionId( tmpMine->GetSessionID() );
			
			int damege = tmpMine->IsBoomCollision( charpos.m_X, charpos.m_Y, charpos.m_Z );

			if( damege > 0 )
			{
				tmpChar->DownHP( damege );

				//나를 뺀 모두에게 폭탄터저 에너지가 달았다는 패킷을 보낸다
				//나에게 너 폭탄 맞았다는 패킷을 보낸다.
				SendGameCharDamegedByMine( mineMaster->GetSessionID(), tmpChar );
				

				if( tmpChar->IsDie() )
				{
					//다른편이면
					if( tmpChar->GetTeam() != mineMaster->GetTeam() )
					{
						//죽은캐릭터 반대 팀의 kill수를 올려 주고
						(*iterChar)->GetMyGame()->AddKillCount( mineMaster->GetTeam() );

						//지뢰 주인의 kill수를 올려 준다
						mineMaster->KillCountUp();
					}

					//죽으면 지뢰정보를 초기화 해 줘야 한다
					MineResetTarget( tmpChar->GetSessionID() );

					//죽은것에 대한 패킷
					SendGameCharDieByMine( mineMaster->GetSessionID(), tmpChar );
				}
			}
		}
		//터졌으몇 지뢰data를 변경해 준다.
		//list에서 지워준다
		tmpMine->SetExplosion();
	}
}

void GameProc::MineCrashCheck()
{
	SSynchronize sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();
	std::list<GameSession*>::iterator iterChar;

	while( pos )
	{
		MineItem* tmpMine = m_mapMine.GetNextValue( pos );

		//설치되어 있는 지뢰가 맞음?
		if( !tmpMine->IsInstall() )
			continue;

		//이미 실행중인 지뢰면 넘어 간다
		if( tmpMine->IsRun() )
			continue;

		for( iterChar= m_listPlayer.GetHeader(); !m_listPlayer.IsEnd( iterChar ); ++iterChar )
		{
			//지뢰밟은애 받아 오고
			CharObj* tmpChar = (*iterChar)->GetMyInfo();

			//죽은 애는 패스
			if( tmpChar->IsDie() )
				continue;

			//내 팀이면 무시
			if( tmpMine->GetTeam() == tmpChar->GetTeam() )
				continue;
			
			POINT3 charpos= tmpChar->GetPos();

			//지뢰와 캐릭터의 충돌 체크
			if( tmpMine->IsCollision( charpos.m_X, charpos.m_Y, charpos.m_Z ) )
			{
				//충돌
				//지뢰실행flog를 실행
				tmpMine->SetRun();

				//지뢰 실행 패킷을 보낸다.
				SendGameRunMine( tmpMine );

				//이미 실행된 지뢰 다른 캐릭터 처리는 않해도 된다
				break;
			}
		}
	}
}

void GameProc::CountDownCharInvincible()
{
	//캐릭터를 돌면서 시간을 줄여 준다
	float elapsed = m_timer.GetElapsedTime();

	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	CharObj* tmpChar;
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpChar = (*iter)->GetMyInfo();

		//무적이 아니면 됬음
		if( !tmpChar->IsInvincible() )
			continue;

		//무적이면 시간 다운
		tmpChar->CountDownInvincibleTime( elapsed );

		//다시 검사해서 무적이 풀렸으면 패킷을 보내 준다
		if( !tmpChar->IsInvincible() )
			SendEndInvincible( tmpChar );
	}
}

void GameProc::SendEndInvincible( CharObj* uChar )
{
	SSynchronize Sync( &m_listPlayer );

	//======================================
	// 다른 애들에게
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_END_INVINCIBLE );
	sendPacket << uChar->GetSessionID();

	SendAllPlayerInGame( sendPacket, uChar->GetSession() );

	//======================================
	// 나에게
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_END_INVINCIBLE );

	uChar->GetSession()->SendPacket( sendPacket );
}

BOOL GameProc::SendReadyPacket()
{
	SPacket sendPacket( SC_GAME_GAME_READY );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendStartPacket()
{
	SPacket sendPacket( SC_GAME_START_GAME );
	
	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendTimeRemain( int remainTime )
{
	SPacket sendPacket( SC_GAME_TIME_REMAIN );

	sendPacket << remainTime;

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendNotice( TCHAR* notice )
{
	SPacket sendPacket( SC_GAME_NOTICE );

	int size = _tcslen( notice ) * sizeof( TCHAR );

	sendPacket << size;
	sendPacket.PutData( notice, size );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendRestartPacket()
{
	SPacket sendPacket( SC_GAME_RESTART );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

void GameProc::SetGotoLobby()
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		(*iter)->SetGotoGame();
	}
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

BOOL GameProc::SendGameRunMine( MineItem* mine )
{
	SPacket sendPacket( SC_GAME_RUN_MINE );

	sendPacket << mine->GetSessionID();
	sendPacket << mine->GetX();
	sendPacket << mine->GetY();
	sendPacket << mine->GetZ();

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGameExplosionMine( MineItem* mine )
{
	SPacket sendPacket( SC_GAME_EXPLOSION_MINE );

	sendPacket << mine->GetSessionID();
	sendPacket << mine->GetX();
	sendPacket << mine->GetY();
	sendPacket << mine->GetZ();

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGameCharDamegedByMine( int mineSessionID, CharObj* damegedChar )
{
	//다른 애들한테 전송
	SPacket sendPacket( SC_GAME_CHARACTER_DAMEGED_BY_MINE );
	sendPacket << damegedChar->GetSessionID();

	SendAllPlayerInGame( sendPacket, damegedChar->GetSession() );

	//나한테 전송
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DAMEGED_BY_MINE );
	sendPacket << mineSessionID;
	sendPacket << damegedChar->GetHP();

	damegedChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGameCharDieByMine( int masterID, CharObj* dieChar )
{
	//다른 애들한테 전송
	SPacket sendPacket( SC_GAME_CHARACTER_DIE_BY_MINE );
	sendPacket << masterID;
	sendPacket << dieChar->GetSessionID();

	SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//나한테 전송
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE_BY_MINE );
	sendPacket << masterID;

	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

void GameProc::SendAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	SSynchronize Sync( &m_listPlayer );
	
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
	SSynchronize Sync( &m_listPlayer );

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
	SSynchronize Sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter) == me )
			continue;

		(*iter)->PackageMyNetInfo( packet );
	}
}
