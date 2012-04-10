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

	//�����尡 �ױ� ��ٸ���.
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
		//���� ��ȣ�� ��ٸ���.
		WaitForSingleObject( m_eventStart, INFINITE );

		//player�� ��� �����ϱ⸦ ��ٸ���.
		if( m_iPlayerCount != m_iIsReadyPlayerCount )
			continue;

		//���� ���� packet�� ��ο��� ������
		SendAllPlayer( SC_TGAME_STARTGAME );

		//�����ϱ�
		if( GameRun() )
		{
			//�ð��� ����Ǿ� ������ ������.
			//player�� ��� ������ ��ٸ���.
// 			while(1)
// 			{
// 				//���� �ִ� player�� 0���� �۰ų� ������ while���� ����������.
// 				if( m_iNowPlayerCount <= 0 )
// 					break;
// 			}

			//���� ���� ��ƾ�� ó���Ѵ�.
		}
		//����� ��� ���� ������ ���� �Ǿ���.
		//return�� FALSE�� player�� ������ �� ������Ŵϱ� �׳� �ʱ�ȭ��

		//������ ������ �ٽ� �ʱ�ȭ
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
// 			//�ʸ��� player�鿡�� timeCountDown��Ŷ�� ����
// 			SendAllPlayer( SC_TGAME_COUNTDWON_TIME );
// 
// 			//�ʸ� ����
// 			--m_iTimeCount;
// 		}

		//player�� ��� ������ ���� ����
		if( m_iNowPlayerCount <= 0 )
			return FALSE;

		//�ʸ� ����ؼ� �ʰ� ��� �� ������ ���� ����
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
