#pragma once

enum SERVER_PACKET_ID
{
	// int			-		id
	UDPSERVER_CONNECT_OK = 10,

	// int			-		id
	// int			-		port번호
	UDPSERVER_ASK_INFO,

	// int			-		playerCount
	//--------------------------------------
	// int			-		id
	// int			-		ip데이터 크기
	// char			-		ip
	// int			-		port번호
	//-------------------------------------->> 반복
	UDPSERVER_PLAYER_INFO,

	// int			-		id번호
	UDPSERVER_PLAYER_DISCONNECT,
};

#include "SSessionObj.h"

class Player;
class PlayerMgr;

class UDPSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(UDPSession)
	SDECLARE_DYNCREATE(UDPSession)

private:
	Player*				m_player;

	//세션 ID용 번호
	static int			m_sessionID;

	//======================================
	// singleTon 객체
	//======================================
	static PlayerMgr*	m_playerMgr;

public:
	UDPSession(void);
	~UDPSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	void RecvASKInfo( SPacket& packet );

	void SendConnectOK();

	void SendPlayerInfo();
	void SendMyInfo();

	void SendPlayerDisconnect();
};
