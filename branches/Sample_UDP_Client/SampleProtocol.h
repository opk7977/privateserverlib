#pragma once

enum SERVER_PACKET_ID
{
	// int			-		id
	UDPSERVER_CONNECT_OK = 10,

	// int			-		id
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

enum CLIENT_UDP_PACKET
{
	// int			-		id
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		dirX
	// float		-		dirY
	// float		-		dirZ
	UDP_CHAR_INFO = 100,
};