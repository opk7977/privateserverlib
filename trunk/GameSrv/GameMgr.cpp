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
	//�Է¹��� ����ŭ ���� proc�� �����ȴ�
	for( int i=1; i<=count; ++i )
	{
		GameProc* tmpGame = new GameProc;
		tmpGame->Init( i );
		tmpGame->BeginThread();

		m_mapGameRoom[i] = tmpGame;
	}

	//======================================
	// test�� ���� 0�� ��⺻ ����
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
	//��� ������
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
	//0���̸� test�� ���� zeroRoom�� �Ѱ� �ش�
	if( roomNum == 0 )
		return m_zeroRoom;

	if( m_mapGameRoom.find( roomNum ) == m_mapGameRoom.end() )
		return NULL;

	return m_mapGameRoom[roomNum];
}
