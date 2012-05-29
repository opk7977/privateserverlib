#pragma once

#include "SPacket.h"
#include "SampleProtocol.h"

class CharMgr;
class SPacket;
class PacketSender;
class SUDPNet;

class PacketParser : public SSingleton <PacketParser>
{
private:
	friend class SSingleton<PacketParser>;

private:
	SPacket				m_packet;

	//======================================
	// Singleton°´Ã¼
	//======================================
	CharMgr*			m_playerMgr;
	PacketSender*		m_sender;
	SUDPNet*			m_udp;

private:
	PacketParser(void);
	~PacketParser(void);

private:
	void PacketParsing();

	//UDPSERVER_CONNECT_OK
	void RecvConnectOK();

	//UDPSERVER_PLAYER_INFO
	void RecvPlayerInfo();

	//UDPSERVER_PLAYER_DISCONNECT
	void RecvPlayerDisconnect();

	//UDP_CHAR_INFO
	void RecvUDPCharInfo();

	
public:
	void Init();
	void Release();

	void PacketProcess();

};

#define GetParser PacketParser::GetInstance()

