#include "GameMgr.h"
#include "GameProc.h"

#include "SSynchronize.h"

GameMgr::GameMgr(void)
{
}

GameMgr::~GameMgr(void)
{
}

void GameMgr::CreateGameProc( int count )
{
	SSynchronize sync( this );

	//�Է¹��� ����ŭ ���� proc�� �����ȴ�
	for( int i=1; i<=count; ++i )
	{
		GameProc* tmpGame = new GameProc;
		tmpGame->Init( i );
		tmpGame->BeginThread();

		m_mapGameRoom[i] = tmpGame;
	}
}

void GameMgr::Release()
{
	SSynchronize sync( this );

	//��� ������
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
	SSynchronize sync( this );

	//0���̸� test�� ���� zeroRoom�� �Ѱ� �ش�
	if( m_mapGameRoom.find( roomNum ) == m_mapGameRoom.end() )
		return NULL;

	return m_mapGameRoom[roomNum];
}
