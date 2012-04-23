#pragma once

#include "SSessionObj.h"

class GameSession : public SSessionObj
{
public:
	GameSession(void);
	~GameSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

};
