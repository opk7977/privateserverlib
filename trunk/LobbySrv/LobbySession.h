#pragma once

#include "SSessionObj.h"

class LobbySession : public SSessionObj
{
private:
	int		m_SessionId;
	TCHAR	m_tstrId[30];
	int		RoomNo;

public:
	SDECLARE_DYNAMIC(LobbySession)
	SDECLARE_DYNCREATE(LobbySession)

public:
	LobbySession(void);
	~LobbySession(void);
};
