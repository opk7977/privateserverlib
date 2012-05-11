#include "GameProc.h"
#include "GameSession.h"

#include "SPacket.h"
#include "SLogger.h"
#include "GameProtocol.h"

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
// 	//�ٽ� ������� ���ϰ� �̺�Ʈ ����
// 	ResetEvent( m_hStartGame );
// 	//while�� ���� ������ ����flagǮ��
// 	m_nowIsPlaying = FALSE;

	//������ �ʱ�ȭ
	m_listPlayer.clear();
	m_playerCount = m_inGamePlayerCount = 0;
	m_AttKillCount = m_DefKillCount = 0;
}

void GameProc::Release()
{
	m_listPlayer.clear();
}

BOOL GameProc::Run()
{
	while(1)
	{
		//���� ��ȣ�� ��ٸ���.
		WaitForSingleObject( m_hStartGame, INFINITE );

		//��� player�� �����ϱ⸦ ��ٸ���.
		PreStartGame();

		if( GameRun() )
		{
			//������ ��������
			EndLogic();
		}
		
		//���� ����
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

	SetEvent( m_hStartGame );

	return TRUE;
}

BOOL GameProc::PreStartGame()
{
// 	float basictime = 0.1f;
// 	float timeCount = 0.f;

	//��� player�� �����ϱ⸦ ��ٷ��� �Ѵ�
	while( m_playerCount != m_inGamePlayerCount )
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
	//�ٽ� ������� ���ϰ� �̺�Ʈ ����
	ResetEvent( m_hStartGame );
	//while�� ���� ������ ����flagǮ��
	m_nowIsPlaying = FALSE;
}

void GameProc::EndLogic()
{

}

void GameProc::AddPlayer( GameSession* player )
{
	if( m_inGamePlayerCount >= m_playerCount )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameProc::AddPlayer()\n������ player���� �ѽ��ϴ�\n\n") );
		return;
	}

	{
		SSynchronize sync( this );

		m_listPlayer.push_back( player );
		++m_inGamePlayerCount;
	}
}

BOOL GameProc::DelPlayer( GameSession* player )
{
	if( m_listPlayer.empty() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameProc::DelPlayer()\nlist�� player�� �������� �ʽ��ϴ�\n\n") );
		return FALSE;
	}

	std::list<GameSession*>::iterator	iter, iterPre;

	{
		SSynchronize sync( this );

		iter = m_listPlayer.begin();
		for( ; iter != m_listPlayer.end(); )
		{
			iterPre = iter++;
			if( (*iterPre) == player )
			{
				m_listPlayer.erase( iterPre );
				--m_inGamePlayerCount;
				break;
			}
		}
	}

	if( m_inGamePlayerCount <= 0 )
		return FALSE;

	return TRUE;

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameProc::DelPlayer()\n������ player�� �������� �ʽ��ϴ�\n\n") );
}

void GameProc::SendAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	std::list<GameSession*>::iterator	iter;

	{
		SSynchronize sync( this );

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
		SSynchronize sync( this );

		//����ִ��� �켱 ���
		packet << m_inGamePlayerCount;

		//����� ������ ����!
		iter = m_listPlayer.begin();
		for( ; iter != m_listPlayer.end(); ++iter )
		{
			if( (*iter) == me )
				continue;

			(*iter)->PackageMyInfo( packet );
		}
	}
}
