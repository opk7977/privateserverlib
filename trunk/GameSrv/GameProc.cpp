#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"

#include "CharMgr.h"
#include "SPacket.h"
#include "SLogger.h"


GameProc::GameProc(void)
{
	m_hStartGame = ::CreateEvent( NULL, TRUE, FALSE, NULL );

/*	m_timer.Init();*/
	Init();
}

GameProc::~GameProc(void)
{
	WaitForSingleObject( GetThreadHandle(), 1000 );
	Release();
}

void GameProc::Init()
{
	//정보는 초기화
	m_listPlayer.Clear();
	m_playerCount = 0;
	m_AttKillCount = m_DefKillCount = 0;
}

void GameProc::Init( int i )
{
	m_id = i;

	Init();
}

void GameProc::Release()
{
	m_listPlayer.Clear();
}

BOOL GameProc::Run()
{
	while(1)
	{
		//시작 신호를 기다린다.
		WaitForSingleObject( m_hStartGame, INFINITE );
		//게임 loop는 한번만 돌아야 하니까 바로 바꿔 준다
		ResetEvent( m_hStartGame );


		//모든 player가 접속하기를 기다린다.
		PreStartGame();


		if( GameRun() )
		{
			//게임이 정상종료
			EndLogic();
		}
		
		//게임 종료
		EndGame();
	}

	return TRUE;
}

BOOL GameProc::GameRun()
{
	return TRUE;
}

BOOL GameProc::StartGame()
{
	m_nowPlayTimeCount = m_playTime;
	m_nowIsPlaying = TRUE;

	SetEvent( m_hStartGame );

	return TRUE;
}

BOOL GameProc::PreStartGame()
{
// 	float basictime = 0.1f;
// 	float timeCount = 0.f;

	//모든 player가 접속하기를 기다려야 한다
	while( m_playerCount != m_listPlayer.GetItemCount() )
	{
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
	}

	return TRUE;
}

void GameProc::EndGame()
{
	//다시 실행되지 못하게 이벤트 막고
	ResetEvent( m_hStartGame );
	//while문 빠져 나오게 게임flag풀고
	m_nowIsPlaying = FALSE;

	Init();
}

void GameProc::EndLogic()
{

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

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter) == me )
			continue;

		(*iter)->GetMyInfo()->PackageMyInfo( packet );
	}
}
