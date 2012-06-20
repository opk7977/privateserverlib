#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"
#include "DataLeader.h"
#include "DBSrvMgr.h"

#include "CharMgr.h"
#include "ItemMgr.h"

#include "SPacket.h"
#include "SLogger.h"
#include "SSynchronize.h"

#include "SLogger.h"

SLogger*	GameProc::m_logger		= &GetLogger;
DataLeader* GameProc::m_document	= &GetDocument;
DBSrvMgr*	GameProc::m_dbMgr		= &GetDBSrv;

GameProc::GameProc(void)
{
	m_hStartGame	= ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hStartEvent	= ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hReturnResult	= ::CreateEvent( NULL, TRUE, FALSE, NULL );

	m_logger = &GetLogger;

	//��� �����Լ��� ����
	m_GameResult[GAME_MODE_DEATH_MATCH]		= &GameProc::GameEnd_DeathMatch;
	m_GameResult[GAME_MODE_BOOM_MISSION]	= &GameProc::GameEnd_BoomMission;
	m_GameResult[GAME_MODE_HACKING_MISSION]	= &GameProc::GameEnd_HackingMission;

	m_timer.Init();
}

GameProc::~GameProc(void)
{
	WaitForSingleObject( GetThreadHandle(), 1000 );
}

void GameProc::Init( int i )
{
	m_id	= i;
	//�� ���� udp��Ʈ ��ȣ
	m_port	= m_document->GameSrvPortNum+i+10;

	Init();
}

void GameProc::Init()
{
	SSynchronize sync( this );

	//������ �ʱ�ȭ
	m_listPlayer.Clear();
	//ClearItem();
	m_nowIsPlaying		= FALSE;
	m_playerCount		= 8;	//�⺻...
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
}

BOOL GameProc::Run()
{
	while(1)
	{
		//======================================
		// ���� ��ȣ�� ��ٸ���.
		//======================================
		WaitForSingleObject( m_hStartGame, INFINITE );
		//���� loop�� �ѹ��� ���ƾ� �ϴϱ� �ٷ� �ٲ� �ش�
		ResetEvent( m_hStartGame );
		//======================================

		//======================================
		// ��� player�� �����ϱ⸦ ��ٸ���.
		//======================================
		WaitForSingleObject( m_hStartEvent, INFINITE );
		//--------------------------------------
		// ���� ���� packet�� ������.
		//--------------------------------------
		SendStartPacket();
		//======================================

		//���� loop�� ����
		BOOL isEnd = FALSE;
		while( !isEnd )
		{
			//======================================
			// ����
			//======================================
			GameRun();

			//======================================
			// ����� ��� ������ �����Ÿ� �׳� ����
			//======================================
			if( m_listPlayer.IsEmpty() )
				break;

			//======================================
			// �켱�� ������ ����
			// ���� Ÿ�Ӿƿ� packet�� ������.
			//======================================
			SPacket sendPacket( SC_TIME_OUT );
			SendAllPlayerInGame( sendPacket );
			//======================================

			//======================================
			// ���� ��(?)�� ����
			// ���� Ƚ���� ���� �ִٸ� TRUE�� return
			//======================================
			if( ResetGame() )
			{
				//======================================
				// ���� �ٽ� ����! ���� �Ǽ��� ����
				// ��� ��� �Ŀ� �ٽ� ���� ��Ŷ�� ������.
				//======================================
				WaitTimeLogic( WAIT_GAME_END_TIME );

				SendRestartPacket();
				continue;
			}
			else
			{
				//======================================
				// ���� ��/ DB������ ������.
				// ĳ���͸��� ���и� ���� rankPoint�� ����
				//======================================
				int whosWin = -1;
				if( m_AttWinCount > m_DefWinCount )
				{
					//�������� �̰���
					whosWin = 0;
				}
				else if( m_AttWinCount < m_DefWinCount )
				{
					//������� �̰���
					whosWin = 1;
				}
				else
				{
					//���
					whosWin = 3;
				}

				(this->*m_GameResult[m_gameMode])(whosWin);
				// ������ ��� rankPoint�� DB�� �����͸� ������
				SendToDBGameResult();
				isEnd = TRUE;
			}
		}
		//DB�������� �κ񼭹��� �����Ͱ� ���������� ����Ǳ������
		//�ð��� ��ٷ���� �Ѵ�
		//�κ񿡼� ��ȣ�� ���⸦ ��ٸ���.
		WaitForSingleObject( m_hReturnResult, INFINITE );

		//���� �����⸦ 5������ ��ٷȴٰ� ������ ��ȣ�� Ŭ��鿡�� �����Ѵ�.
		WaitTimeLogic( 5 );

		//Ŭ��鿡�� �κ�� ���ư���� ��Ŷ�� ������.
		SendGotoLobbyPacket();

		//������ �����
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("GameProc::Run()\n%d�� ������ ����˴ϴ�. ���� Proc�� �Ͻ������� �����ϴ�.\n\n"), m_id );
#endif
	}

	return TRUE;
}

void GameProc::GameRun()
{
	float frameTime		= 0.f;
	float lifeUpTime	= 0.f;

	SPacket sendPacket;

	//======================================
	// ���� play�ð����� loop
	//======================================
	while( m_nowPlayTimeCount > 0 )
	{
		//======================================
		// 0.5�� ���� ó��
		//======================================
		lifeUpTime += m_timer.GetElapsedTime();
		if( lifeUpTime >= 0.5f )
		{
			//�켱 �ʱ�ȭ
			lifeUpTime = 0.f;

			//======================================
			// hp�� 1�� �÷� �ش�.
			//======================================
			//�λ���� ĳ���͸� ��� 1�� �÷� �ش�
			PlayerHeal();
			//�׾ֵ鸸 ������.
			SendPlayerHeal();

			//======================================
			// ���� ����
			//======================================
		}

		//======================================
		// 1�� ���� ó��
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

			//======================================
			// �ð� ó��
			//======================================
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("���� �ð�: %d\n"), m_nowPlayTimeCount );
			//�ð� ��Ŷ ������
			sendPacket.PacketClear();
			sendPacket.SetID( SC_GAME_TIME_COUNTDOWN );
			sendPacket << m_nowPlayTimeCount;
			SendAllPlayerInGame( sendPacket );

			// ���� ���ڰ� 1�� ������ �����ٸ�			//
			// �浹 üũ ���� ����üũ���� �� ��� �Ѵ�	//

			//======================================
			// ���� �浹üũ
			//======================================
			
		}
	}
}

void GameProc::AddReadyCount()
{
	//�غ�Ϸ� �ο��� ���� ���� �ο��� ���� ���� ������ ������ �� �ִ�.

	SSynchronize sync( this );

	if( ++m_readyCount == m_playerCount )
		SetEvent( m_hStartEvent );
}

BOOL GameProc::StartGame()
{
	//���� �ð��� ����
 	m_nowPlayTimeCount	= m_playTime;
 	m_nowIsPlaying		= TRUE;
	
	m_AttKillCount		= m_DefKillCount = 0;
	m_isWin				= -1;

	SetEvent( m_hStartGame );

	return TRUE;
}

BOOL GameProc::ResetGame()
{
// 	//======================================
// 	// ���� Ÿ�Ӿƿ� packet�� ������.
// 	//======================================
// 	SPacket sendPacket( SC_TIME_OUT );
// 	SendAllPlayerInGame( sendPacket );
// 	//======================================

	//======================================
	// ����Ʈ ����!
	//======================================
	switch( m_gameMode )
	{
	case GAME_MODE_DEATH_MATCH:
		{
			//ų /���� ��
			if( m_AttKillCount > m_DefKillCount )
				++m_AttWinCount;		//�������� �̱�
			else if( m_AttKillCount < m_DefKillCount )
				++m_DefWinCount;		//���潺���� �̱�
			else
				++m_TieCount;			//���
		}
		break;
	case GAME_MODE_BOOM_MISSION:
	case GAME_MODE_HACKING_MISSION:
		{
			// m_isWinȮ��
			if( m_isWin == GAME_TEAM_ATT )
				++m_AttWinCount;
			else if( m_isWin == GAME_TEAM_DEF )
				++m_DefWinCount;
			else
				++m_TieCount;
		}
		break;
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::ResetGame()\n���� ��尡 ��ȿ���� �ʽ��ϴ�\n\n") );
		return FALSE;
	}
	
	//�� ų/ ���� ī���͸� �������ش�.
	m_AttKillAllCount	+= m_AttKillCount;
	m_DefKillAllCount	+= m_DefKillCount;


	//������ ��� �������� FALSE�� return�Ͽ� ������ ���� �� �� �ְ� �Ѵ�.
	if( --m_playCount <= 0 )
		return FALSE;

	//������ �� �ؾ� �ϴ� ���� �ٽ� ������ �ʱ�ȭ
	m_nowPlayTimeCount	= m_playTime;
	m_nowIsPlaying		= TRUE;
	m_AttKillCount		= m_DefKillCount = 0;
	m_isWin				= -1;
	m_SendList.Clear();

	//ĳ������ HP���� ��� reset!
	CharacterRestart();
	//////////////////////////////////////////////////////////////////////////
	//���ӳ����� ���� item�� �ʱ�ȭ �� ��� ��
	//////////////////////////////////////////////////////////////////////////
	//ClearItem();

	return TRUE;
}

void GameProc::EndGame()
{
	//���� ����
	m_nowPlayTimeCount = 0;
// 	ResetEvent( m_hReturnResult );
}

void GameProc::WaitTimeLogic( int waitTime /*= WAIT_GAME_END_TIME */)
{
	float frameTime = 0.f;

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

void GameProc::GameEnd_DeathMatch( int winnerTeam )
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	CharObj* tmpObj = NULL;
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpObj = (*iter)->GetMyInfo();

		if( tmpObj->GetTeam() == winnerTeam )
		{
			//�̱���
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * DEATHMATCH_WINNER_KILL_POINT );
		}
		else
		{
			//����/ Ȥ�� �������
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
			//�̱���
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_WINNER_KILL_POINT + MISSION_WINNER_INCREASE_POINT );
		}
		else
		{
			//����/ Ȥ�� �������
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
			//�̱���
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_WINNER_KILL_POINT + MISSION_WINNER_INCREASE_POINT );
		}
		else
		{
			//����/ Ȥ�� �������
			tmpObj->SetRankPoint( tmpObj->GetKillCount() * MISSION_LOSER_KILL_POINT );
		}
	}
}

BOOL GameProc::SendToDBGameResult()
{
	SPacket sendPacket( GAME_TO_DB_UPDATE_USERDATA );
	//��� ĳ������ rankPoint������ ����!

	//���ݰ����� ���ӹ�ȣ(���ȣ)�� ���
	sendPacket << m_id;
	//�켱 �ο������� ���
	sendPacket << m_listPlayer.GetItemCount();

	//������ ��´�.
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		(*iter)->GetMyInfo()->PackageMyInfoForDB( sendPacket );
	}

	//DB������ �����͸� ������.
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

void GameProc::CharacterRestart()
{
	SSynchronize sync( this );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; m_listPlayer.IsEnd( iter ); ++iter )
	{
		//HPȸ��!
		(*iter)->GetMyInfo()->SetAlive();
	}
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

void GameProc::SendPacketToMyTeam( int team, SPacket& packet, GameSession* me/* = NULL*/ )
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
