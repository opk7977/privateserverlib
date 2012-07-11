#include "GameProc.h"
#include "GameSession.h"
#include "GameProtocol.h"
#include "DataLeader.h"
#include "DBSrvMgr.h"

#include "CharMgr.h"
#include "ItemMgr.h"
#include "MineItem.h"
#include "CharMgr.h"

#include "SPacket.h"
#include "SLogger.h"
#include "SSynchronize.h"

#include "SLogger.h"

SLogger*		GameProc::m_logger		= &GetLogger;
DataLeader*		GameProc::m_document	= &GetDocument;
DBSrvMgr*		GameProc::m_dbMgr		= &GetDBSrv;
ItemMgr*		GameProc::m_itemMgr		= &GetItemMgr;
CharMgr*		GameProc::m_charMgr		= &GetCharMgr;

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

	//���ڸ� ���� Sync��ü
	m_mineCritical = new SServerObj;
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

	//�������� �ʱ�ȭ
	MineClear();

	//������ �ʱ�ȭ
	m_listPlayer.Clear();
	
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

	//test
	m_isSelectCountDown	= FALSE;
	m_isGameCountDown	= FALSE;
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
		// ���� �غ� �Ϸ� �Ǿ��ٴ� ��ȣ�� ������
		//--------------------------------------
		//SendReadyPacket();
		SendStartPacket();
		//--------------------------------------
		// ���� �� �ð� �ݴϴ�.
		//--------------------------------------
		//CountDownLogin( 29, SENDTIME_TEN, LASTTIME_THREE );
		CountDownLogic( 19 );
		//--------------------------------------
		// ���� ���� packet�� ������.
		//--------------------------------------
		//SendStartPacket();
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
		//m_nowIsPlaying�� FALSE�̸� �ֵ��� �� ������ �����Ŵϱ� �κ��� ����� ��ٸ��� �ʴ´�
		if( !m_nowIsPlaying )
			continue;

		WaitForSingleObject( m_hReturnResult, INFINITE );

		//���� �����⸦ 5������ ��ٷȴٰ� ������ ��ȣ�� Ŭ��鿡�� �����Ѵ�.
		WaitTimeLogic( WAIT_GAME_END_TIME );

		//ĳ���� ���°� �κ�� ���ư��� �غ����̶�� ǥ�ø� �� �ش�
		//SetGotoLobby();

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
	float TimeOneSec			= 0.f;


	float ElapsedTime;

	SPacket sendPacket;

	//======================================
	// ���� play�ð����� loop
	//======================================
	while( m_nowPlayTimeCount > 0 )
	{
		m_timer.ProcessTime();
		ElapsedTime = m_timer.GetElapsedTime();

		//======================================
		// 1�� ���� ó��_�ð� ó��
		//======================================
// 		if( m_isGameCountDown )
// 		{
			TimeOneSec += ElapsedTime;
			if( TimeOneSec >= 1.0f )
			{
				//�켱 �ʱ�ȭ
				TimeOneSec = 0.f;

				//�ð� ���̰�
				if( --m_nowPlayTimeCount < 0 )
					break;

				//--------------------------------------
				// �ð� ó��
				//--------------------------------------
				//m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("���� �ð�: %d\n"), m_nowPlayTimeCount );
				//�ð� ��Ŷ ������
				sendPacket.PacketClear();
				sendPacket.SetID( SC_GAME_TIME_COUNTDOWN );
				sendPacket << m_nowPlayTimeCount;
				SendAllPlayerInGame( sendPacket );
			}
// 		}
		//======================================
		// ������ ������ ó��
		//======================================

		//======================================
		// HP
		//======================================
		CountUpCharHP( ElapsedTime );

		//======================================
		// ����/ ��ĵ ��ġ
		//--------------------------------------
		CountSkillPoints( ElapsedTime );

		//======================================
		// ����ó��
		//--------------------------------------
		// ĳ���� ���������� �ֵ� �ð��� ���̰�
		// �ð��� �� �Ǹ� ������ Ǯ�� �ش�
		//--------------------------------------
		CountDownCharInvincible();
		//======================================

		//======================================
		// ���� ó��
		//--------------------------------------
		// ���� ������ �ֵ� �ð� ���̱�
		//--------------------------------------
		CountDownRunningMine();
		//--------------------------------------
		// ������ ���� �浹üũ
		//--------------------------------------
		ExplosionMineCrashCheck();
		//--------------------------------------
		// ĳ���Ϳ� �浹üũ
		//--------------------------------------
		MineCrashCheck();
		//======================================
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
// 	m_SendList.Clear();

	//ĳ������ HP���� ��� reset!
	CharacterRestart();
	//�������� reset!
	MineReset();

	return TRUE;
}

void GameProc::EndGame()
{
	//���� ����
	m_nowPlayTimeCount = 0;
// 	ResetEvent( m_hReturnResult );
}

void GameProc::WaitTimeLogic( int waitTime )
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
			if( --frameTime <= 0 )
				return;
		}
	}
}

// void GameProc::CountDownLogin( int waitTime, SendTime sendTime, LastTime lastTime )
// {
// 	float framTime = 0.f;
// 	TCHAR TmpChar[64]={0,};
// 
// 	while(1)
// 	{
// 		//======================================
// 		// �ð� ó��
// 		//======================================
// 		m_timer.ProcessTime();
// 
// 		framTime += m_timer.GetElapsedTime();
// 		if( framTime > 1.0f )
// 		{
// 			//�켱 �ʱ�ȭ
// 			framTime = 0.f;
// 
// 			//������ �ð������� ���� �ð��� ������������ ������
// 			if( (waitTime % sendTime) == 0 )
// 			{
// 				ZeroMemory( TmpChar, 64*2 );
// 				_stprintf_s( TmpChar, 64, _T("[Notice] %d�� ���ҽ��ϴ�."), waitTime );
// 
// 				SendNotice( TmpChar );
// 			}
// 
// 			//������ ī��Ʈ�� �׳� ���縦 ������
// 			if( waitTime <= lastTime )
// 			{
// 				SendTimeRemain( waitTime );
// 			}
// 
// 			//�ð� ���̰� �� �پ����� return
// 			if( --waitTime <= 0 )
// 				return;
// 		}
// 	}
// }

void GameProc::CountDownLogic( int waitTime )
{
	float framTime = 0.f;
	TCHAR TmpChar[64]={0,};
	int m_wait = waitTime;

	SendTimeRemain( waitTime );

	while(1)
	{
		//======================================
		// �ð� ó��
		//======================================
		m_timer.ProcessTime();

// 		if( !m_isSelectCountDown )
// 			m_wait = waitTime;

		framTime += m_timer.GetElapsedTime();
		if( framTime > 1.0f )
		{
			//�켱 �ʱ�ȭ
			framTime = 0.f;

			//�ð� ���̰� �� �پ����� return
			if( --m_wait <= 0 )
				return;
			
			SendTimeRemain( m_wait );
		}
	}
}

void GameProc::MineClear()
{
	SSynchronize sync( m_mineCritical );

	if( m_mapMine.IsEmpty() )
		return;

	POSITION pos = m_mapMine.GetStartPosition();
	MineItem* tmp;
	while( pos )
	{
		tmp = m_mapMine.GetNextValue( pos );
		//��� ������ �ݳ��ϰ� ����!
		m_itemMgr->ReturnMineSpace( tmp );
	}
	//�� ����
	m_mapMine.RemoveAll();
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
	SSynchronize PlayerSync( &m_listPlayer );
	SSynchronize MineSync( m_mineCritical );

	if( m_listPlayer.GetItemCount() >= m_playerCount )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::AddPlayer()\n������ player���� �ѽ��ϴ�\n\n") );
		return;
	}

	//���ڰ����� ����� �ش�
	MineItem* tmp = m_itemMgr->GetMineItemSpace();
	if( tmp == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::AddPlayer()\n���� ������ �޾� �� �� �����ϴ�.\n\n") );
		return;
	}

	tmp->Init();
	tmp->SetMineSpace( player->GetSessionID(), player->GetMyInfo()->GetTeam() );
	m_mapMine[player->GetSessionID()] = tmp;

	m_listPlayer.AddItem( player );
}

BOOL GameProc::DelPlayer( GameSession* player )
{
	SSynchronize PlayerSync( &m_listPlayer );
	SSynchronize MineSync( m_mineCritical );

	if( m_listPlayer.IsEmpty() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameProc::DelPlayer()\nlist�� player�� �������� �ʽ��ϴ�\n\n") );
		return FALSE;
	}

	m_listPlayer.DelItem( player );
	m_mapMine.RemoveKey( player->GetSessionID() );

	if( m_listPlayer.IsEmpty() )
		return FALSE;

	return TRUE;
}

void GameProc::CharacterRestart()
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; m_listPlayer.IsEnd( iter ); ++iter )
	{
		//HPȸ��!
		(*iter)->GetMyInfo()->SetAlive();
	}
}

BOOL GameProc::MineResetTarget( int sessionID )
{
	SSynchronize sync( m_mineCritical );

	MineItem* tmpMine = NULL;

	if( !m_mapMine.Lookup( sessionID, tmpMine ) )
	{
		return FALSE;
	}

	tmpMine->Reset();

	return TRUE;
}

void GameProc::MineReset()
{
	//SSynchronize Sync( &m_boomSoon );
	SSynchronize sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();
	while( pos )
	{
		MineItem* tmp = m_mapMine.GetNextValue( pos );
		tmp->Reset();
	}
}

CharObj* GameProc::FindChar( int sessionID )
{
	SSynchronize Sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter)->GetSessionID() == sessionID )
			return (*iter)->GetMyInfo();
	}

	//������ NULL
	return NULL;
}

MineItem* GameProc::FindMine( int sessionID )
{
	//SSynchronize sync( &m_boomSoon );
	SSynchronize sync( m_mineCritical );

	MineItem* tmpMine = NULL;
	if( !m_mapMine.Lookup( sessionID, tmpMine ) )
	{
		return NULL;
	}

	return tmpMine;
}

void GameProc::CountUpCharHP( float elaps )
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	//��� ĳ���͸� ���鼭 �÷��� ��ų ��ġ�� �÷��ش�
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//������ ��ȭ�� �ִٸ� TRUE�� return�޾� hp�� �����Ѵ�
		if( (*iter)->GetMyInfo()->HPUpOnePoint( elaps ) )
			SendHPCountUp( (*iter) );
	}
}

BOOL GameProc::SendHPCountUp( GameSession* session )
{
	SPacket sendPacket( SC_GAME_REMAIN_HP );

	sendPacket << session->GetMyInfo()->GetHP();

	session->SendPacket( sendPacket );

	return TRUE;
}

void GameProc::CountSkillPoints( float elaps )
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	//��� ĳ���͸� ���鼭 �÷��� ��ų ��ġ�� �÷��ش�
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//������ ��ȭ�� �ִٸ� TRUE�� return�޾� ��ų ����Ʈ�� �����Ѵ�
		if( (*iter)->GetMyInfo()->CountSkillPoint( elaps ) )
			SendSkillPoint( (*iter) );
	}
}

BOOL GameProc::SendSkillPoint( GameSession* session )
{
	CharObj* tmpChar = session->GetMyInfo();

	int hidePoint = tmpChar->GetHidePoint();
	int scanPoint = tmpChar->GetScanPoint();
	if( tmpChar->GetSkillState() == SKILL_HIDE )
	{
		//������ Hide�����̰�
		if( hidePoint <= 0 )
		{
			//����Ʈ�� ��� �Һ��ߴٸ� ��ų ���
			tmpChar->SetSkillNone();
			SendHideOff( session );
		}
	}
	else if( tmpChar->GetSkillState() == SKILL_SCAN )
	{
		//������ scan�����̰�
		if( scanPoint <= 0 )
		{
			//����Ʈ�� ��� �Һ��ߴٸ� ��ų ���
			tmpChar->SetSkillNone();
			SendScanOff( session );
		}
	}

	//�׸��� skill�� ����Ʈ�� ����
	SPacket sendPacket( SC_GAME_SKILL_POINT );

	sendPacket << hidePoint;
	sendPacket << scanPoint;

	session->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameProc::SendHideOff( GameSession* session )
{
	//======================================
	// �ش� ���ǿ��� �� ���� Ǯ�ȴٰ� �˸�
	//======================================
	SPacket sendPacket( SC_GAME_TIMEOUT_HIDE );
	session->SendPacket( sendPacket );

	//======================================
	// �ٸ� ��ο��� ������ Ǯ�ȴٴ� ���� �˸�
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_CHAR_INVISIBLE_HIDE );
	sendPacket << session->GetMyInfo()->GetSessionID();
	SendAllPlayerInGame( sendPacket, session );

	return TRUE;
}

BOOL GameProc::SendScanOff( GameSession* session )
{
	//======================================
	// �ش� ���ǿ��� �� ��ĵ Ǯ�ȴٰ� �˸�
	//======================================
	SPacket sendPacket( SC_GAME_TIMEOUTL_SCAN );
	session->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameProc::SettingMine( GameSession* session, float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	SSynchronize sync( m_mineCritical );

	MineItem* tmpMine;

	//sessionIDȮ��
	if( !m_mapMine.Lookup( session->GetSessionID(), tmpMine ) )
	{
		session->SendPacket( SC_GAME_LAY_MINE_FAILD );
		return TRUE;
	}

	//���ڸ� �̹� ����ߴٸ� ����� �� ����
	if( !tmpMine->CanUse() )
	{
		session->SendPacket( SC_GAME_LAY_MINE_FAILD );
		return TRUE;
	}

	//�ƴϸ� �������
	tmpMine->SetMine( posX, posY, posZ, dirX, dirY, dirZ );

	//��ġ ����
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("GameProc::SettingMine()\nĳ���� %s�� %.2f/%.2f/%.2f/��ġ�� ���ڼ�ġ.\n\n"),
		session->GetMyInfo()->GetID(), posX, posY, posZ );


	//���� ��ġ ����
	session->SendGameLayMine( posX, posY, posZ, dirX, dirY, dirZ );

	//�����ĺ��� ����üũ�ϴ� flag�� �����Ѵ�.
	tmpMine->SetInstall();

	return TRUE;
}

// BOOL GameProc::SettingMine( int sessionId, float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
// {
// 	SSynchronize sync( m_mineCritical );
// 
// 	MineItem* tmpMine;
// 
// 	//sessionIDȮ��
// 	if( !m_mapMine.Lookup( sessionId, tmpMine ) )
// 		return FALSE;
// 
// 	//���ڸ� �̹� ����ߴٸ� ����� �� ����
// 	if( !tmpMine->CanUse() )
// 		return FALSE;
// 
// 	//�ƴϸ� �������
// 	tmpMine->SetMine( posX, posY, posZ, dirX, dirY, dirZ );
// 
// 	//��ġ ����
// 
// 	return TRUE;
// }

void GameProc::CountDownRunningMine()
{
	float elapsed = m_timer.GetElapsedTime();

	SSynchronize Sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();

	while( pos )
	{
		MineItem* tmpMine = m_mapMine.GetNextValue( pos );

		//��ġ�� ���ڰ� �´°�?
		if( !tmpMine->IsInstall() )
			continue;

		//�������ΰ� �ƴϸ� ���Ѵ�
		if( !tmpMine->IsRun() )
			continue;

		//�ð��� �ٿ� �ش�
		tmpMine->DownTimer( elapsed );
	}
}

void GameProc::ExplosionMineCrashCheck()
{
	SSynchronize sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();
	std::list<GameSession*>::iterator iterChar;
	
	while( pos )
	{
		MineItem* tmpMine = m_mapMine.GetNextValue( pos );

		//��ġ�� ���ڰ� �´°�?
		if( !tmpMine->IsInstall() )
			continue;

		//�������ΰ� �ƴϸ� ���Ѵ�
		if( !tmpMine->IsRun() )
			continue;

		//���� ������ ��?
		if( !tmpMine->IsBoom() )
			continue;

		//�ƴϸ� �Ͷ߸��� ����
		//���ӿ� �ִ� ��ο��� ����
		SendGameExplosionMine( tmpMine );

		//���� �浹üũ����
		for( iterChar = m_listPlayer.GetHeader(); !m_listPlayer.IsEnd( iterChar ); ++iterChar )
		{
			//���ڸ�����
			CharObj* tmpChar = (*iterChar)->GetMyInfo();
			//���� �ִ� �н�
			if( tmpChar->IsDie() )
				continue;

			//������ �ֵ� �н�
			if( tmpChar->IsInvincible() )
				continue;

			POINT3 charpos = tmpChar->GetPos();

			//���������� ã��
			CharObj* mineMaster = m_charMgr->FindCharAsSessionId( tmpMine->GetSessionID() );
			
			int damege = tmpMine->IsBoomCollision( charpos.m_X, charpos.m_Y, charpos.m_Z );

			if( damege > 0 )
			{
				tmpChar->DownHP( damege );

				//���� �� ��ο��� ��ź���� �������� �޾Ҵٴ� ��Ŷ�� ������
				//������ �� ��ź �¾Ҵٴ� ��Ŷ�� ������.
				SendGameCharDamegedByMine( mineMaster->GetSessionID(), tmpChar );
				

				if( tmpChar->IsDie() )
				{
					//�ٸ����̸�
					if( tmpChar->GetTeam() != mineMaster->GetTeam() )
					{
						//����ĳ���� �ݴ� ���� kill���� �÷� �ְ�
						(*iterChar)->GetMyGame()->AddKillCount( mineMaster->GetTeam() );

						//���� ������ kill���� �÷� �ش�
						mineMaster->KillCountUp();
					}

					//������ ���������� �ʱ�ȭ �� ��� �Ѵ�
					MineResetTarget( tmpChar->GetSessionID() );

					//�����Ϳ� ���� ��Ŷ
					SendGameCharDieByMine( mineMaster->GetSessionID(), tmpChar );
				}
			}
		}
		//�������� ����data�� ������ �ش�.
		//list���� �����ش�
		tmpMine->SetExplosion();
	}
}

void GameProc::MineCrashCheck()
{
	SSynchronize sync( m_mineCritical );

	POSITION pos = m_mapMine.GetStartPosition();
	std::list<GameSession*>::iterator iterChar;

	while( pos )
	{
		MineItem* tmpMine = m_mapMine.GetNextValue( pos );

		//��ġ�Ǿ� �ִ� ���ڰ� ����?
		if( !tmpMine->IsInstall() )
			continue;

		//�̹� �������� ���ڸ� �Ѿ� ����
		if( tmpMine->IsRun() )
			continue;

		for( iterChar= m_listPlayer.GetHeader(); !m_listPlayer.IsEnd( iterChar ); ++iterChar )
		{
			//���ڹ����� �޾� ����
			CharObj* tmpChar = (*iterChar)->GetMyInfo();

			//���� �ִ� �н�
			if( tmpChar->IsDie() )
				continue;

			//�� ���̸� ����
			if( tmpMine->GetTeam() == tmpChar->GetTeam() )
				continue;
			
			POINT3 charpos= tmpChar->GetPos();

			//���ڿ� ĳ������ �浹 üũ
			if( tmpMine->IsCollision( charpos.m_X, charpos.m_Y, charpos.m_Z ) )
			{
				//�浹
				//���ڽ���flog�� ����
				tmpMine->SetRun();

				//���� ���� ��Ŷ�� ������.
				SendGameRunMine( tmpMine );

				//�̹� ����� ���� �ٸ� ĳ���� ó���� ���ص� �ȴ�
				break;
			}
		}
	}
}

void GameProc::CountDownCharInvincible()
{
	//ĳ���͸� ���鼭 �ð��� �ٿ� �ش�
	float elapsed = m_timer.GetElapsedTime();

	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	CharObj* tmpChar;
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		tmpChar = (*iter)->GetMyInfo();

		//������ �ƴϸ� ����
		if( !tmpChar->IsInvincible() )
			continue;

		//�����̸� �ð� �ٿ�
		tmpChar->CountDownInvincibleTime( elapsed );

		//�ٽ� �˻��ؼ� ������ Ǯ������ ��Ŷ�� ���� �ش�
		if( !tmpChar->IsInvincible() )
			SendEndInvincible( tmpChar );
	}
}

void GameProc::SendEndInvincible( CharObj* uChar )
{
	SSynchronize Sync( &m_listPlayer );

	//======================================
	// �ٸ� �ֵ鿡��
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_END_INVINCIBLE );
	sendPacket << uChar->GetSessionID();

	SendAllPlayerInGame( sendPacket, uChar->GetSession() );

	//======================================
	// ������
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_END_INVINCIBLE );

	uChar->GetSession()->SendPacket( sendPacket );
}

BOOL GameProc::SendReadyPacket()
{
	SPacket sendPacket( SC_GAME_GAME_READY );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendStartPacket()
{
	SPacket sendPacket( SC_GAME_START_GAME );
	
	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendTimeRemain( int remainTime )
{
	SPacket sendPacket( SC_GAME_TIME_REMAIN );

	sendPacket << remainTime;

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendNotice( TCHAR* notice )
{
	SPacket sendPacket( SC_GAME_NOTICE );

	int size = _tcslen( notice ) * sizeof( TCHAR );

	sendPacket << size;
	sendPacket.PutData( notice, size );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendRestartPacket()
{
	SPacket sendPacket( SC_GAME_RESTART );

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

void GameProc::SetGotoLobby()
{
	SSynchronize sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		(*iter)->SetGotoGame();
	}
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

BOOL GameProc::SendGameRunMine( MineItem* mine )
{
	SPacket sendPacket( SC_GAME_RUN_MINE );

	sendPacket << mine->GetSessionID();
	sendPacket << mine->GetX();
	sendPacket << mine->GetY();
	sendPacket << mine->GetZ();

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGameExplosionMine( MineItem* mine )
{
	SPacket sendPacket( SC_GAME_EXPLOSION_MINE );

	sendPacket << mine->GetSessionID();
	sendPacket << mine->GetX();
	sendPacket << mine->GetY();
	sendPacket << mine->GetZ();

	SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGameCharDamegedByMine( int mineSessionID, CharObj* damegedChar )
{
	//�ٸ� �ֵ����� ����
	SPacket sendPacket( SC_GAME_CHARACTER_DAMEGED_BY_MINE );
	sendPacket << damegedChar->GetSessionID();

	SendAllPlayerInGame( sendPacket, damegedChar->GetSession() );

	//������ ����
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DAMEGED_BY_MINE );
	sendPacket << mineSessionID;
	sendPacket << damegedChar->GetHP();

	damegedChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameProc::SendGameCharDieByMine( int masterID, CharObj* dieChar )
{
	//�ٸ� �ֵ����� ����
	SPacket sendPacket( SC_GAME_CHARACTER_DIE_BY_MINE );
	sendPacket << masterID;
	sendPacket << dieChar->GetSessionID();

	SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//������ ����
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE_BY_MINE );
	sendPacket << masterID;

	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

void GameProc::SendAllPlayerInGame( SPacket& packet, GameSession* me /*= NULL */ )
{
	SSynchronize Sync( &m_listPlayer );
	
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
	SSynchronize Sync( &m_listPlayer );

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
	SSynchronize Sync( &m_listPlayer );

	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter) == me )
			continue;

		(*iter)->PackageMyNetInfo( packet );
	}
}
