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

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// 받은 패킷 처리 함수
	//======================================


	//======================================
	// 보내는 패킷 생성함수
	//======================================
};
