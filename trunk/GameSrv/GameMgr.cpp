#include "GameMgr.h"
#include "GameProc.h"

GameMgr::GameMgr(void)
{
}

GameMgr::~GameMgr(void)
{
}

void GameMgr::CreateGameProc( int count )
{
	//입력받은 수만큼 게임 proc이 생성된다
	for( int i=1; i<=count; ++i )
	{
		GameProc* tmpGame = new GameProc;
		tmpGame->Init( i );
		tmpGame->BeginThread();

		m_mapGameRoom[i] = tmpGame;
	}

	//======================================
	// test를 위한 0번 방기본 셋팅
	//======================================
	m_zeroRoom = new GameProc;
	m_zeroRoom->Init(0);

	m_zeroRoom->SetPlayerCount( 4 );
	m_zeroRoom->SetGameMode( 0 );
	m_zeroRoom->SetGamePlayTime( 120 );
	m_zeroRoom->SetGamePlayCount( 1 );

	m_zeroRoom->StartGame();
	//======================================
}

void GameMgr::Release()
{
	//모두 지우자
	if( m_zeroRoom != NULL )
		delete m_zeroRoom;

	if( m_mapGameRoom.empty() )
		return;

	std::map<int, GameProc*>::iterator iter, iterPre;
	iter = m_mapGameRoom.begin();
	for( ; iter != m_mapGameRoom.end() ; )
	{
		iterPre = iter++;
		delete iterPre->second;
	}
	m_mapGameRoom.clear();
}

GameProc* GameMgr::FindGame( int roomNum )
{
	//0번이면 test를 위해 zeroRoom을 넘겨 준다
	if( roomNum == 0 )
		return m_zeroRoom;

	if( m_mapGameRoom.find( roomNum ) == m_mapGameRoom.end() )
		return NULL;

	return m_mapGameRoom[roomNum];
}
