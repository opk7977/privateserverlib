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

	//정보는 초기화
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
		//시작 신호를 기다린다.
		WaitForSingleObject( m_hStartGame, INFINITE );
		//게임 loop는 한번만 돌아야 하니까 바로 바꿔 준다
		ResetEvent( m_hStartGame );

		//게임 시작 전에 모든 player가 접속하기를 기다린다.
		//PreStartGame();
		WaitForSingleObject( m_hStartEvent, INFINITE );
		//======================================
		// 게임 시작 packet을 보낸다.
		//======================================
		SendStartPacket();
		//======================================

		//게임 loop를 돈다
		BOOL isEnd = TRUE;
		while( isEnd )
		{
			//게임
			GameRun();

			//끝난게 사람이 없어서 끝난거면 그냥 종료
			if( m_listPlayer.IsEmpty() )
				break;

			//게임 종료
			BOOL result = ResetGame();

			//게임 종료 대기
			WaitTimeLogic();

			if( result )
			{
				// 게임 새로 시작하는 패킷을 보낸다
				SendRestartPacket();
				continue;
			}
			else
			{
				// 로비로 가라는 패킷을 보낸다.
				SendGotoLobbyPacket();
				isEnd = FALSE;
			}
		}

		//게임이 종료됨
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("GameProc::Run()\n%d번 게임이 종료됩니다. 게임 Proc이 일시적으로 닫힙니다.\n\n"), m_id );
	}

	return TRUE;
}

void GameProc::GameRun()
{
	float frameTime		= 0.f;
	float lifeUpTime	= 0.f;

	SPacket sendPacket;

	//시간이 다 되기 전까지 loop
	while( m_nowPlayTimeCount > 0 )
	{
		Sleep(0);
		//======================================
		// 시간 처리
		//======================================
		m_timer.ProcessTime();

		frameTime += m_timer.GetElapsedTime();
		if( frameTime >= 1.0f )
		{
			//우선 초기화
			frameTime = 0.f;

			//시간 줄이고
			if( --m_nowPlayTimeCount < 0 )
				break;

			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("보낸 시간: %d\n"), m_nowPlayTimeCount );

			//시간 패킷 보내기
			sendPacket.PacketClear();
			sendPacket.SetID( SC_GAME_TIME_COUNTDOWN );
			sendPacket << m_nowPlayTimeCount;
			SendAllPlayerInGame( sendPacket );
		}

		//======================================
		// hp를 1씩 올려 준다.
		//======================================
		lifeUpTime += m_timer.GetElapsedTime();
		if( lifeUpTime >= 0.5f )
		{
			//우선 초기화
			lifeUpTime = 0.f;

			//부상당한 캐릭터를 모두 1씩 올려 준다
			PlayerHeal();

			//그애들만 보낸다.
			SendPlayerHeal();
		}
	}

	//게임 셋팅을 바꿔 준다...새로이..여기서 해주는게 맞겠지?....
	//게임 판수에 따라 판수를 다 하지 않았으면 FALSE를 return 해준다.
}

void GameProc::AddReadyCount()
{
	//준비완료 인원이 게임 실행 인원과 같아 지면 게임을 시작할 수 있다.
	if( ++m_readyCount == m_playerCount )
		SetEvent( m_hStartEvent );
}

BOOL GameProc::StartGame()
{
	//게임 시간을 셋팅
 	m_nowPlayTimeCount	= m_playTime;
 	m_nowIsPlaying		= TRUE;

	SetEvent( m_hStartGame );

	return TRUE;
}

// BOOL GameProc::PreStartGame()
// {
// 	//모든 player가 준비완료가 되기를 기다려야 한다
// 	//while( m_playerCount != m_listPlayer.GetItemCount() )
// 	WaitForSingleObject( m_hStartEvent, INFINITE );
// 
// 	//게임 시작 packet을 보낸다.
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
		++m_AttWinCount;		//어택팀이 이김
	else if( m_AttKillCount < m_DefKillCount )
		++m_DefKillCount;		//디펜스팀이 이김
	else
		++m_TieCount;			//비김

	//총 수에 넣어 준다
	m_AttKillAllCount	+= m_AttKillCount;
	m_DefKillAllCount	+= m_DefKillCount;

	//======================================
	// 게임 타임아웃 packet을 보낸다.
	//======================================
	SPacket sendPacket( SC_TIME_OUT );
	SendAllPlayerInGame( sendPacket );
	//======================================

	//초기화
	m_AttKillCount		= m_DefKillCount = 0;
	m_nowPlayTimeCount	= m_playTime;
	ClearItem();
	//////////////////////////////////////////////////////////////////////////
	//게임내에서 사용된 item도 초기화 해 줘야 함
	//////////////////////////////////////////////////////////////////////////

	//게임을 모두 끝냈으면 FALSE를 return하여 게임을 종료 할 수 있게 한다.
	if( --m_playCount <= 0 )
		return FALSE;

	return TRUE;
}

void GameProc::EndGame()
{
// 	//======================================
// 	// 게임 종료 packet을 보낸다.
// 	//======================================
// 	int result;
// 	if( m_AttWinCount > m_DefWinCount )
// 		result = 0;		//red팀 승리
// 	else if( m_AttWinCount < m_DefWinCount )
// 		result = 1;		//blue팀 승리
// 	else
// 		result = -1;	//무승부
// 
// 	SPacket sendPacket( SC_GAME_END );
// 	sendPacket << 0;				//타임 아웃
// 	sendPacket << result;
// 	sendPacket << m_AttKillAllCount;
// 	sendPacket << m_DefKillAllCount;
// 	sendPacket << m_AttWinCount;
// 	sendPacket << m_DefWinCount;
// 	sendPacket << m_TieCount;
// 	SendAllPlayerInGame( sendPacket );
// 	//======================================

	//게임을 종료 하기 위해 약간의 시간을 두고 캐릭터들에게 로비로 돌아가라는 신호를 보낸다.
}

void GameProc::WaitTimeLogic()
{
	float frameTime = 0.f;
	float waitTime	= WAIT_GAME_END_TIME;

	//시간이 다 되기 전까지 loop
	while(1)
	{
		//======================================
		// 시간 처리
		//======================================
		m_timer.ProcessTime();

		frameTime += m_timer.GetElapsedTime();
		if( frameTime >= 1.0f )
		{
			//우선 초기화
			frameTime = 0.f;

			//시간 줄이고 다 줄었으면 return
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

	//없으면 NULL
	return NULL;
}

void GameProc::PlayerHeal()
{
	std::list<GameSession*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		if( (*iter)->GetMyInfo()->HPUpOnePoint() )
		{
			//hp를 올린애는 list에 넣어 준다
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

	//로비의 ip와 port번호를 넣는다.
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
		//나는 안보내도 됨
		if( (*iter) == me )
			continue;

		//자신의 팀이면 보낸다
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
