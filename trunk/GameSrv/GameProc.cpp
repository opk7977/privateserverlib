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
	//������ �ʱ�ȭ
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
		//���� ��ȣ�� ��ٸ���.
		WaitForSingleObject( m_hStartGame, INFINITE );
		//���� loop�� �ѹ��� ���ƾ� �ϴϱ� �ٷ� �ٲ� �ش�
		ResetEvent( m_hStartGame );


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

	//��� player�� �����ϱ⸦ ��ٷ��� �Ѵ�
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
	//�ٽ� ������� ���ϰ� �̺�Ʈ ����
	ResetEvent( m_hStartGame );
	//while�� ���� ������ ����flagǮ��
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::AddPlayer()\n������ player���� �ѽ��ϴ�\n\n") );
		return;
	}

	m_listPlayer.AddItem( player );
}

BOOL GameProc::DelPlayer( GameSession* player )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::DelPlayer()\nlist�� player�� �������� �ʽ��ϴ�\n\n") );
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

	//������ NULL
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
