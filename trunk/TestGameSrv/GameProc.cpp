#include "GameProc.h"
#include "TestGSession.h"
#include "TestGProtocol.h"
#include "SSynchronize.h"

GameProc::GameProc(void)
{
	m_timer.Init();
	m_eventStart = ::CreateEvent( NULL, TRUE, FALSE, NULL);
	
	Init();
}

GameProc::~GameProc(void)
{
	Release();

	//쓰레드가 죽길 기다린다.
	WaitForSingleObject( GetThreadHandle(), 1000 );
}

BOOL GameProc::Init()
{
	m_iPlayerCount			= 0;
	m_iNowPlayerCount		= 0;
	m_iIsReadyPlayerCount	= 0;
	m_isPlay				= FALSE;
	m_iTimeCount			= GAMETIME_COUNT;

	return TRUE;
}

void GameProc::Release()
{
	DeleteGame();
}

BOOL GameProc::Run()
{
	while(1)
	{
		//실행 신호를 기다린다.
		WaitForSingleObject( m_eventStart, INFINITE );

		//player가 모두 접속하기를 기다린다.
		if( m_iPlayerCount != m_iIsReadyPlayerCount )
			continue;

		//게임 실행 packet을 모두에게 보낸다
		SendAllPlayer( SC_TGAME_STARTGAME );

		//게임하기
		if( GameRun() )
		{
			//시간이 종료되어 게임이 끝났다.
			//player가 모두 나가길 기다린다.
// 			while(1)
// 			{
// 				//지금 있는 player가 0보다 작거나 같으면 while문을 빠져나간다.
// 				if( m_iNowPlayerCount <= 0 )
// 					break;
// 			}

			//게임 종료 루틴을 처리한다.
		}
		//사람이 모두 나가 게임이 종료 되었다.
		//return이 FALSE면 player가 게임중 다 사라진거니까 그냥 초기화만

		//게임이 끝나면 다시 초기화
		DeleteGame();
	}
}

BOOL GameProc::GameRun()
{
	float timeCount = 0.f;
	
	while( m_isPlay )
	{
		//Sleep(10);
// 		m_timer.ProcessTime();
// 		timeCount += m_timer.GetElapsedTime();
// 
// 		if( timeCount >= 1.f )
// 		{
// 			timeCount -= 1.f;
// 
// 			//초마다 player들에게 timeCountDown패킷을 보냄
// 			SendAllPlayer( SC_TGAME_COUNTDWON_TIME );
// 
// 			//초를 줄임
// 			--m_iTimeCount;
// 		}

		//player가 모두 나가면 게임 종료
		if( m_iNowPlayerCount <= 0 )
			return FALSE;

		//초를 계산해서 초가 모두 다 됬으면 게임 종료
// 		if( m_iTimeCount <= 0 )
// 		{
// 			SendAllPlayer( SC_TGAME_ENDGAME );
// 			return TRUE;
// 		}
	}

	return FALSE;
}

BOOL GameProc::CreateGame( int playerCount )
{
	Init();

	m_listPlayer.clear();

	m_iPlayerCount = playerCount;
	m_isPlay = TRUE;
	SetEvent( m_eventStart );

	return TRUE;
}

void GameProc::DeleteGame()
{
	ResetEvent( m_eventStart );
	m_isPlay = FALSE;
	m_listPlayer.clear();
}

int GameProc::GetPlayerCount()
{
	return m_iPlayerCount;
}

int GameProc::GetNowPlayerCount()
{
	return m_iNowPlayerCount;
}

BOOL GameProc::IsPlay()
{
	return m_isPlay;
}

void GameProc::AddPlayer( TestGSession* session )
{
	m_listPlayer.push_back( session );
	++m_iNowPlayerCount;
}

void GameProc::PlayerCountUp()
{
	++m_iIsReadyPlayerCount;
}

BOOL GameProc::DelPlayer( TestGSession* session )
{
	std::list<TestGSession*>::iterator iter, iterPre;

	{
		SSynchronize sync( this );

		iter = m_listPlayer.begin();
		while( iter != m_listPlayer.end() )
		{
			iterPre = iter++;
			if( *iterPre == session )
			{
				m_listPlayer.erase( iterPre );
				--m_iNowPlayerCount;
				return TRUE;
			}
		}
		return FALSE;
	}
}

void GameProc::SendAllPlayer( SPacket& packet )
{
	if( m_iNowPlayerCount <= 0 )
		return;

	std::list<TestGSession*>::iterator iter;
	
	{
		SSynchronize sync( this );

		iter = m_listPlayer.begin();
		while( iter != m_listPlayer.end() )
		{
			(*iter)->SendPacket( packet );
			iter++;
		}
	}
}

void GameProc::SendAllPlayer( unsigned int protocolID )
{
	if( m_iNowPlayerCount <= 0 )
		return;

	std::list<TestGSession*>::iterator iter;

	{
		SSynchronize sync( this );

		iter = m_listPlayer.begin();
		while( iter != m_listPlayer.end() )
		{
			(*iter)->SendPacket( protocolID );
			iter++;
		}
	}
}
