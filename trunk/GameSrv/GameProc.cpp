#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"
#include "DataLeader.h"

#include "CharMgr.h"

#include "SPacket.h"
#include "SLogger.h"
#include "SSynchronize.h"

#include "SLogger.h"

GameProc::GameProc(void)
{
	m_hStartGame	= ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hStartEvent	= ::CreateEvent( NULL, TRUE, FALSE, NULL );

	m_logger = &GetLogger;

	m_timer.Init();

/*	m_timer.Init();*/
/*	Init();*/
}

GameProc::~GameProc(void)
{
	WaitForSingleObject( GetThreadHandle(), 1000 );
	Release();
}

void GameProc::Init( int i )
{
	m_id	= i;
	m_port	= GetDocument.GameSrvProtNum+i+10;

	Init();
}

void GameProc::Init()
{
	SSynchronize sync( this );

	//정보는 초기화
	m_listPlayer.Clear();
	m_playerCount	= 8;
	m_AttKillCount	= 0;
	m_DefKillCount	= 0;
	//m_isAllReady	= FALSE;
	m_readyCount	= 0;
}

void GameProc::ResultGame()
{
// 	SSynchronize sync( this );
// 
// 	if( m_WinTeam )
}

void GameProc::Release()
{
	SSynchronize sync( this );

	m_listPlayer.Clear();
	m_listItem.Clear();
}

BOOL GameProc::Run()
{
	while(1)
	{
		//시작 신호를 기다린다.
		WaitForSingleObject( m_hStartGame, INFINITE );
		//게임 loop는 한번만 돌아야 하니까 바로 바꿔 준다
		ResetEvent( m_hStartGame );

		//게임 시작 전에 모든 player가 접속하기를 기다린다.
		PreStartGame();

		//게임 loop를 돈다
// 		while(1)
// 		{
// 			if( !GameRun() )
// 			{
// 				//게임을 다시 시작해야 한다.
// 			}
// 			else
// 			{
// 				//게임 판수를 모두 맞침
// 				//빠져 나간다.
// 				break;
// 			}
//		}
		//우선 1판 기준
		GameRun();
		
		//게임 종료
		//EndGame();

		//게임이 종료됨
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("GameProc::Run()\n%d번 게임이 종료됩니다. 게임 Proc이 일시적으로 닫힙니다.\n\n"), m_id );
	}

	return TRUE;
}

BOOL GameProc::GameRun()
{
	float frameTime = 0.f;

	//시간이 다 되기 전까지 loop
	while( m_nowIsPlaying )
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

			//시간 줄이고
			if( --m_nowPlayTimeCount <= 0 )
				m_nowIsPlaying = FALSE;

			//시간 패킷 보내기
			SPacket sendPacket( SC_GAME_TIME_COUNTDOWN );
			sendPacket << m_nowPlayTimeCount;
			SendAllPlayerInGame( sendPacket );
		}
		//======================================

		//
		//
		//
	}

	//게임 셋팅을 바꿔 준다...새로이..여기서 해주는게 맞겠지?....
	//게임 판수에 따라 판수를 다 하지 않았으면 FALSE를 return 해준다.

	return TRUE;
}

void GameProc::AddReadyCount()
{
	//준비완료 인원이 게임 실행 인원과 같아 지면 게임을 시작할 수 있다.
	if( ++m_readyCount == m_playerCount )
		SetEvent( m_hStartEvent );
}

BOOL GameProc::StartGame()
{
	//게임 시간을 셋팅
 	m_nowPlayTimeCount = m_playTime;
 	m_nowIsPlaying = TRUE;

	SetEvent( m_hStartGame );

	return TRUE;
}

BOOL GameProc::PreStartGame()
{
// 	float basictime = 0.1f;
// 	float timeCount = 0.f;
	//우선 막고
	ResetEvent( m_hStartEvent );

	//모든 player가 준비완료가 되기를 기다려야 한다
	//while( m_playerCount != m_listPlayer.GetItemCount() )
	WaitForSingleObject( m_hStartEvent, INFINITE );
//	{
// 		m_timer.ProcessTime();
// 
// 		timeCount += m_timer.GetElapsedTime();
// 
// 		if( timeCount >= basictime )
// 		{
// 			timeCount = 0;
// 
// 			SPacket sendPacket;
// 			sendPacket.SetID( SC_GAME_SYNC );
// 
// 			PackageAllPlayerInGame( sendPacket );
// 			SendAllPlayerInGame( sendPacket );
// 		}
//	}

	//게임 시작 packet을 보낸다.
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_START_GAME );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

void GameProc::EndGame()
{
	SSynchronize sync( this );

	//다시 실행되지 못하게 이벤트 막고
	ResetEvent( m_hStartGame );
	//while문 빠져 나오게 게임flag풀고
	m_nowIsPlaying = FALSE;

	Init();
}

void GameProc::EndLogic()
{

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

BOOL GameProc::SendStartPacket()
{
	SPacket sendPacket( SC_GAME_START_GAME );
	
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

void GameProc::PackageAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	SSynchronize Sync( this );

	//우선 사람 수를 넣고
	int count = m_listPlayer.GetItemCount();;
	if( me == NULL )
		packet << count;
	else
		packet << count-1;

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter) == me )
			continue;

		(*iter)->GetMyInfo()->PackageMyInfo( packet );
	}
}
