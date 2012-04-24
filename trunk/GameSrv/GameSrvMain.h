#pragma once

#include "SServerStd.h"

#define SessionSpace	100

class GameSrvMain
{
public:
	GameSrvMain(void);
	~GameSrvMain(void);

	BOOL Init();
	void Release();
};
