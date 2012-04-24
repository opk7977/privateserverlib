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
};
