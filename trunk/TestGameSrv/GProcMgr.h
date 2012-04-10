#pragma once

#include "SServerObj.h"

class GameProc;

class GProcMgr : public SSingleton <GProcMgr>
{
private:
	friend class SSingleton<GProcMgr>;

private:
	//게임 프로세스 map
	std::map<int, GameProc*>			m_mapGame;
	//방의 개수
	int									m_iGameCount;


	//임계영역을 위한 obj
	SServerObj							m_critical;

public:
	GProcMgr(void);
	~GProcMgr(void);

	//게임 프로세스를 처음에 생성한다
	void Init( int gameCount );

	//게임 프로세스를 모두 삭제
	void Release();

	//하나의 게임을 개설한다.
	GameProc* CreateGameProcess( int roomNo, int playerCount );

	//생성되어 있는 게임 프로세스를 받아 온다
	GameProc* GetGameProcess( int roomNo );
};

#define GetGPMgr	GProcMgr::GetInstance()
