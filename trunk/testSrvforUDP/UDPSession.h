#pragma once

enum SERVER_PACKET_ID
{
	// int			-		id
	UDPSERVER_CONNECT_OK = 10,

	// int			-		id
	// int			-		ip������ ũ��
	// char			-		ip
	// int			-		port��ȣ
	UDPSERVER_ASK_INFO,

	// int			-		playerCount
	//--------------------------------------
	// int			-		id
	// int			-		ip������ ũ��
	// char			-		ip
	// int			-		port��ȣ
	//-------------------------------------->> �ݺ�
	UDPSERVER_PLAYER_INFO,

	// int			-		id��ȣ
	UDPSERVER_PLAYER_DISCONNECT,
};

#include "SSessionObj.h"

class Player;

class UDPSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(UDPSession)
	SDECLARE_DYNCREATE(UDPSession)

private:
	Player*				m_player;

	//======================================
	// singleTon ��ü
	//======================================
	static PlayerMgr*	m_playerMgr;

public:
	UDPSession(void);
	~UDPSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	void RecvASKInfo( SPacket& packet );

	void SendConnectOK( int id );
	void SendPlayerInfo();
	void SendPlayerDisconnect();
};
