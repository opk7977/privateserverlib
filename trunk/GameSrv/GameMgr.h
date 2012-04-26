#pragma once

#include "SServerStd.h"

class GameProc;

class GameMgr : public SSingleton <GameMgr>
{
private:
	friend class SSingleton<GameMgr>;

private:
	std::map<int, GameProc*>			m_mapGameRoom;

public:
	GameMgr(void);
	~GameMgr(void);

	//��ó�� ����proc�� ������ �ش�
	void CreateGameProc( int count );
	void Release();

	GameProc* FindGame( int roomNum );

};

#define GetGameMgr GameMgr::GetInstance()

