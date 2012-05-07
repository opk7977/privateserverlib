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
		tmpGame->Init();
		tmpGame->BeginThread();

		m_mapGameRoom[i] = tmpGame;
	}
}

void GameMgr::Release()
{
	//모두 지우자
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
	if( m_mapGameRoom.find( roomNum ) == m_mapGameRoom.end() )
		return NULL;

	return m_mapGameRoom[roomNum];
}
