#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"
#include "DataLeader.h"

#include "CharMgr.h"
#include "ItemMgr.h"

#include "SPacket.h"
#include "SLogger.h"
#include "SSynchronize.h"

#include "SLogger.h"

SLogger*	GameProc::m_logger		= &GetLogger;
DataLeader* GameProc::m_document	= &GetDocument;

GameProc::GameProc(void)
{
	m_hStartGame	= ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hStartEvent	= ::CreateEvent( NULL, TRUE, FALSE, NULL );

	m_logger = &GetLogger;

	m_timer.Init();
}

GameProc::~GameProc(void)
{
	WaitForSingleObject( GetThreadHandle(), 1000 );
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
	//ClearItem();
	m_nowIsPlaying		= FALSE;
	m_playerCount		= 8;
	m_AttKillCount		= m_DefKillCount	= 0;
	m_AttKillAllCount	= m_DefKillAllCount = 0;
	m_AttWinCount		= m_DefWinCount		= 0;
	m_nowPlayTimeCount	= 0;
	m_TieCount			= 0;
	m_readyCount		= 0;

	ResetEvent( m_hStartGame );
	ResetEvent( m_hStartEvent );
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
		//PreStartGame();
		WaitForSingleObject( m_hStartEvent, INFINITE );
		//======================================
		// ���� ���� packet�� ������.
		//======================================
		SendStartPacket();
		//======================================

		//���� loop�� ����
		BOOL isEnd = TRUE;
		while( isEnd )
		{
			//����
			GameRun();

			//������ ����� ��� �����Ÿ� �׳� ����
			if( m_listPlayer.IsEmpty() )
				break;

			//���� ����
			BOOL result = ResetGame();

			//���� ���� ���
			WaitTimeLogic();

			if( result )
			{
				// ���� ���� �����ϴ� ��Ŷ�� ������
				SendRestartPacket();
				continue;
			}
			else
			{
				// �κ�� ����� ��Ŷ�� ������.
				SendGotoLobbyPacket();
				isEnd = FALSE;
			}
		}

		//������ �����
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("GameProc::Run()\n%d�� ������ ����˴ϴ�. ���� Proc�� �Ͻ������� �����ϴ�.\n\n"), m_id );
	}

	return TRUE;
}

void GameProc::GameRun()
{
	float frameTime		= 0.f;
	float lifeUpTime	= 0.f;

	SPacket sendPacket;

	//�ð��� �� �Ǳ� ������ loop
	while( m_nowPlayTimeCount > 0 )
	{
		Sleep(0);
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
			if( --m_nowPlayTimeCount < 0 )
				break;

			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("���� �ð�: %d\n"), m_nowPlayTimeCount );

			//�ð� ��Ŷ ������
			sendPacket.PacketClear();
			sendPacket.SetID( SC_GAME_TIME_COUNTDOWN );
			sendPacket << m_nowPlayTimeCount;
			SendAllPlayerInGame( sendPacket );
		}

		//======================================
		// hp�� 1�� �÷� �ش�.
		//======================================
		lifeUpTime += m_timer.GetElapsedTime();
		if( lifeUpTime >= 0.5f )
		{
			//�켱 �ʱ�ȭ
			lifeUpTime = 0.f;

			//�λ���� ĳ���͸� ��� 1�� �÷� �ش�
			PlayerHeal();

			//�׾ֵ鸸 ������.
			SendPlayerHeal();
		}
	}

	//���� ������ �ٲ� �ش�...������..���⼭ ���ִ°� �°���?....
	//���� �Ǽ��� ���� �Ǽ��� �� ���� �ʾ����� FALSE�� return ���ش�.
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
 	m_nowPlayTimeCount	= m_playTime;
 	m_nowIsPlaying		= TRUE;

	SetEvent( m_hStartGame );

	return TRUE;
}

// BOOL GameProc::PreStartGame()
// {
// 	//��� player�� �غ�Ϸᰡ �Ǳ⸦ ��ٷ��� �Ѵ�
// 	//while( m_playerCount != m_listPlayer.GetItemCount() )
// 	WaitForSingleObject( m_hStartEvent, INFINITE );
// 
// 	//���� ���� packet�� ������.
// 	SPacket sendPacket;
// 	sendPacket.SetID( SC_GAME_START_GAME );
// 
// 	SendAllPlayerInGame( sendPacket );
// 
// 	return TRUE;
// }

BOOL GameProc::ResetGame()
{
	if( m_AttKillCount > m_DefKillCount )
		++m_AttWinCount;		//�������� �̱�
	else if( m_AttKillCount < m_DefKillCount )
		++m_DefKillCount;		//���潺���� �̱�
	else
		++m_TieCount;			//���

	//�� ���� �־� �ش�
	m_AttKillAllCount	+= m_AttKillCount;
	m_DefKillAllCount	+= m_DefKillCount;

	//======================================
	// ���� Ÿ�Ӿƿ� packet�� ������.
	//======================================
	SPacket sendPacket( SC_TIME_OUT );
	SendAllPlayerInGame( sendPacket );
	//======================================

	//�ʱ�ȭ
	m_AttKillCount		= m_DefKillCount = 0;
	m_nowPlayTimeCount	= m_playTime;
	ClearItem();
	//////////////////////////////////////////////////////////////////////////
	//���ӳ����� ���� item�� �ʱ�ȭ �� ��� ��
	//////////////////////////////////////////////////////////////////////////

	//������ ��� �������� FALSE�� return�Ͽ� ������ ���� �� �� �ְ� �Ѵ�.
	if( --m_playCount <= 0 )
		return FALSE;

	return TRUE;
}

void GameProc::EndGame()
{
// 	//======================================
// 	// ���� ���� packet�� ������.
// 	//======================================
// 	int result;
// 	if( m_AttWinCount > m_DefWinCount )
// 		result = 0;		//red�� �¸�
// 	else if( m_AttWinCount < m_DefWinCount )
// 		result = 1;		//blue�� �¸�
// 	else
// 		result = -1;	//���º�
// 
// 	SPacket sendPacket( SC_GAME_END );
// 	sendPacket << 0;				//Ÿ�� �ƿ�
// 	sendPacket << result;
// 	sendPacket << m_AttKillAllCount;
// 	sendPacket << m_DefKillAllCount;
// 	sendPacket << m_AttWinCount;
// 	sendPacket << m_DefWinCount;
// 	sendPacket << m_TieCount;
// 	SendAllPlayerInGame( sendPacket );
// 	//======================================

	//������ ���� �ϱ� ���� �ణ�� �ð��� �ΰ� ĳ���͵鿡�� �κ�� ���ư���� ��ȣ�� ������.
}

void GameProc::WaitTimeLogic()
{
	float frameTime = 0.f;
	float waitTime	= WAIT_GAME_END_TIME;

	//�ð��� �� �Ǳ� ������ loop
	while(1)
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

			//�ð� ���̰� �� �پ����� return
			if( --waitTime <= 0 )
				return;
		}
	}
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

	//������ NULL
	return NULL;
}

void GameProc::PlayerHeal()
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter)->GetMyInfo()->HPUpOnePoint() )
		{
			//hp�� �ø��ִ� list�� �־� �ش�
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

	//�κ��� ip�� port��ȣ�� �ִ´�.
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

void GameProc::SendPacketToMyTeam( int team, SPacket& packet, GameSession* me )
{
	SSynchronize Sync( this );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//���� �Ⱥ����� ��
		if( (*iter) == me )
			continue;

		//�ڽ��� ���̸� ������
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
