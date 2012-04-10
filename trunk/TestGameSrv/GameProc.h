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
	//�� �ο�
	int							m_iPlayerCount;
	//���� �����ο�
	int							m_iNowPlayerCount;
	//���� �����ο��� �غ� �Ϸ�� �ο�
	int							m_iIsReadyPlayerCount;
	//���� ��?
	BOOL						m_isPlay;
	//�ð�
	STime						m_timer;
	//���� ��� ī��Ʈ
	int							m_iTimeCount;

	//������ �̺�Ʈ �ڵ�
	HANDLE						m_eventStart;

public:
	GameProc(void);
	~GameProc(void);

	BOOL		Init();
	void		Release();

	BOOL		Run();
	//======================================
	// Run���� �ҷ��� ���� ���� �Լ�
	//======================================
	BOOL		GameRun();

	BOOL		CreateGame( int playerCount );
	void		DeleteGame();

	//���ο�
	int			GetPlayerCount();
	int			GetNowPlayerCount();

	//������ �÷��� ������
	BOOL		IsPlay();

	//�÷����� �߰��� ����
	void		AddPlayer( TestGSession* session );
	void		PlayerCountUp();
	BOOL		DelPlayer( TestGSession* session );

	//������ �ϰ� �ִ� player�鿡�� ��Ŷ�� ������.
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

