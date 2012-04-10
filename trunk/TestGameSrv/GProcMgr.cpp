#include "GProcMgr.h"
#include "GameProc.h"

#include "SSynchronize.h"


GProcMgr::GProcMgr(void)
{
}

GProcMgr::~GProcMgr(void)
{
	Release();
}

void GProcMgr::Init( int gameCount )
{
	m_iGameCount = gameCount;

	GameProc* tmpGame;

	for( int i=1; i<=m_iGameCount; ++i )
	{
		tmpGame = new GameProc;
		m_mapGame[i] = tmpGame;
		m_mapGame[i]->BeginThread();
	}
}

void GProcMgr::Release()
{
	std::map<int, GameProc*>::iterator iter, iterPre;

	{
		SSynchronize sync( &m_critical );

		iter= m_mapGame.begin();
		while( iter != m_mapGame.end() )
		{
			iterPre = iter++;
			delete iterPre->second;
			iterPre->second = 0;
		}
	}
}

GameProc* GProcMgr::CreateGameProcess( int roomNo, int playerCount )
{
	if( roomNo < 1 || roomNo > m_iGameCount )
		return NULL;

	if( !m_mapGame[roomNo]->CreateGame( playerCount ) )
		return NULL;

	return m_mapGame[roomNo];
}

GameProc* GProcMgr::GetGameProcess( int roomNo )
{
	{
		SSynchronize sync( &m_critical );

		if( roomNo < 1 || roomNo > m_iGameCount )
			return NULL;

		if( m_mapGame[roomNo]->IsPlay() )
			return m_mapGame[roomNo];
	}
	return NULL;
}
