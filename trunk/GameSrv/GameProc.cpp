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

	//������ �ʱ�ȭ
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
		//���� ��ȣ�� ��ٸ���.
		WaitForSingleObject( m_hStartGame, INFINITE );
		//���� loop�� �ѹ��� ���ƾ� �ϴϱ� �ٷ� �ٲ� �ش�
		ResetEvent( m_hStartGame );

		//���� ���� ���� ��� player�� �����ϱ⸦ ��ٸ���.
		PreStartGame();

		//���� loop�� ����
// 		while(1)
// 		{
// 			if( !GameRun() )
// 			{
// 				//������ �ٽ� �����ؾ� �Ѵ�.
// 			}
// 			else
// 			{
// 				//���� �Ǽ��� ��� ��ħ
// 				//���� ������.
// 				break;
// 			}
//		}
		//�켱 1�� ����
		GameRun();
		
		//���� ����
		//EndGame();

		//������ �����
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("GameProc::Run()\n%d�� ������ ����˴ϴ�. ���� Proc�� �Ͻ������� �����ϴ�.\n\n"), m_id );
	}

	return TRUE;
}

BOOL GameProc::GameRun()
{
	float frameTime = 0.f;

	//�ð��� �� �Ǳ� ������ loop
	while( m_nowIsPlaying )
	{
		//======================================
		// �ð� ó��
		//======================================
		m_timer.ProcessTime();

		frameTime += m_timer.GetElapsedTime();
		if( frameTime >= 1.0f )
		{
			//�켱 �ʱ�ȭ
			frameTime = 0.f;

			//�ð� ���̰�
			if( --m_nowPlayTimeCount <= 0 )
				m_nowIsPlaying = FALSE;

			//�ð� ��Ŷ ������
			SPacket sendPacket( SC_GAME_TIME_COUNTDOWN );
			sendPacket << m_nowPlayTimeCount;
			SendAllPlayerInGame( sendPacket );
		}
		//======================================

		//
		//
		//
	}

	//���� ������ �ٲ� �ش�...������..���⼭ ���ִ°� �°���?....
	//���� �Ǽ��� ���� �Ǽ��� �� ���� �ʾ����� FALSE�� return ���ش�.

	return TRUE;
}

void GameProc::AddReadyCount()
{
	//�غ�Ϸ� �ο��� ���� ���� �ο��� ���� ���� ������ ������ �� �ִ�.
	if( ++m_readyCount == m_playerCount )
		SetEvent( m_hStartEvent );
}

BOOL GameProc::StartGame()
{
	//���� �ð��� ����
 	m_nowPlayTimeCount = m_playTime;
 	m_nowIsPlaying = TRUE;

	SetEvent( m_hStartGame );

	return TRUE;
}

BOOL GameProc::PreStartGame()
{
// 	float basictime = 0.1f;
// 	float timeCount = 0.f;
	//�켱 ����
	ResetEvent( m_hStartEvent );

	//��� player�� �غ�Ϸᰡ �Ǳ⸦ ��ٷ��� �Ѵ�
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

	//���� ���� packet�� ������.
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_START_GAME );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

void GameProc::EndGame()
{
	SSynchronize sync( this );

	//�ٽ� ������� ���ϰ� �̺�Ʈ ����
	ResetEvent( m_hStartGame );
	//while�� ���� ������ ����flagǮ��
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

	//�켱 ��� ���� �ְ�
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
