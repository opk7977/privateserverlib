#include "GameProc.h"
#include "GameSession.h"

#include "SPacket.h"
#include "SLogger.h"

GameProc::GameProc(void)
{
	m_critical = new SServerObj;
	m_hStartGame = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	Init();
}

GameProc::~GameProc(void)
{
	WaitForSingleObject( GetThreadHandle(), 1000 );
	Release();
}

void GameProc::Init()
{
	//다시 실행되지 못하게 이벤트 막고
	ResetEvent( m_hStartGame );
	//while문 빠져 나오게 게임flag풀고
	m_nowIsPlaying = FALSE;

	//정보는 초기화
	m_listPlayer.clear();
	m_playerCount = m_inGamePlayerCount = 0;
	m_AttKillCount = m_DefKillCount = 0;
}

void GameProc::Release()
{
	delete m_critical;
}

BOOL GameProc::Run()
{
	while(1)
	{
		//시작 신호를 기다린다.
		WaitForSingleObject( m_hStartGame, INFINITE );

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

BOOL GameProc::StartGame( int playercount )
{
	m_playerCount = playercount;
	m_nowIsPlaying = TRUE;

	return TRUE;
}

BOOL GameProc::PreStartGame()
{
	//모든 player가 접속하기를 기다려야 한다
	while( m_playerCount != m_inGamePlayerCount )
	{

	}

	return TRUE;
}

void GameProc::EndGame()
{

}

void GameProc::EndLogic()
{

}

void GameProc::AddPlayer( GameSession* player )
{
	if( m_inGamePlayerCount >= m_playerCount )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameProc::AddPlayer()\n설정된 player수를 넘습니다\n\n") );
		return;
	}

	{
		SSynchronize sync( m_critical );

		m_listPlayer.push_back( player );
		++m_inGamePlayerCount;
	}
}

void GameProc::DelPlayer( GameSession* player )
{
	if( m_listPlayer.empty() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameProc::DelPlayer()\nlist에 player가 존재하지 않습니다\n\n") );
		return;
	}

	std::list<GameSession*>::iterator	iter, iterPre;

	{
		SSynchronize sync( m_critical );

		iter = m_listPlayer.begin();
		for( ; iter != m_listPlayer.end(); )
		{
			iterPre = iter++;
			if( (*iterPre) == player )
			{
				m_listPlayer.erase( iterPre );
				return;
			}
		}
	}

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameProc::DelPlayer()\n삭제할 player가 존재하지 않습니다\n\n") );
}

void GameProc::SendAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	std::list<GameSession*>::iterator	iter;

	{
		SSynchronize sync( m_critical );

		iter = m_listPlayer.begin();
		for( ; iter != m_listPlayer.end(); ++iter )
		{
			if( (*iter) == me )
				continue;
			
			(*iter)->SendPacket( packet );
		}
	}
}

void GameProc::PackageAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	std::list<GameSession*>::iterator	iter;

	{
		SSynchronize sync( m_critical );

		//몇명있는지 우선 담고
		packet << m_inGamePlayerCount;

		//모두의 정보를 담자!
		iter = m_listPlayer.begin();
		for( ; iter != m_listPlayer.end(); ++iter )
		{
			if( (*iter) == me )
				continue;

			(*iter)->PackageMyInfo( packet );
		}
	}
}
