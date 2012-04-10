#pragma once

#include "SThread.h"
#include "TestGSession.h"
//#include "SPacket.h"
#include "STime.h"


#define GAMETIME_COUNT					300


class GameProc : public SThread
{
	//player list
	std::list<TestGSession*>	m_listPlayer;
	//방 인원
	int							m_iPlayerCount;
	//현재 접속인원
	int							m_iNowPlayerCount;
	//현재 접속인원중 준비가 완료된 인원
	int							m_iIsReadyPlayerCount;
	//게임 중?
	BOOL						m_isPlay;
	//시간
	STime						m_timer;
	//게임 경과 카운트
	int							m_iTimeCount;

	//쓰레드 이벤트 핸들
	HANDLE						m_eventStart;

public:
	GameProc(void);
	~GameProc(void);

	BOOL		Init();
	void		Release();

	BOOL		Run();
	//======================================
	// Run에서 불려질 게임 실행 함수
	//======================================
	BOOL		GameRun();

	BOOL		CreateGame( int playerCount );
	void		DeleteGame();

	//방인원
	int			GetPlayerCount();
	int			GetNowPlayerCount();

	//지금이 플레이 중인지
	BOOL		IsPlay();

	//플레이저 추가와 삭제
	void		AddPlayer( TestGSession* session );
	void		PlayerCountUp();
	BOOL		DelPlayer( TestGSession* session );

	//게임을 하고 있는 player들에게 패킷을 보낸다.
	void		SendAllPlayer( SPacket& packet );
	void		SendAllPlayer( unsigned int protocolID );

	template <typename T>
	void PackageAllPlayer( T& functor, SPacket& packet )
	{
		if( m_iNowPlayerCount <= 0 )
			return;

		for( std::list<TestGSession*>::iterator iter = m_listPlayer.begin();
			iter != m_listPlayer.end(); ++iter )
		{
			functor( *iter, packet );
		}
	}
};

