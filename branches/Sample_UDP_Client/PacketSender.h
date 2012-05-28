#pragma once

#include "SServerStd.h"
#include "SampleProtocol.h"


class SNetwork;
class SUDPNet;

class PacketSender : public SSingleton <PacketSender>
{
private:
	friend class SSingleton<PacketSender>;

private:
	SNetwork*	m_network;
	SUDPNet*	m_udp;

private:
	PacketSender(void);
	~PacketSender(void);

public:
	//UDPSERVER_ASK_INFO
	BOOL SendASKInfo( int _id, int _port );

	//UDP_CHAR_INFO
	BOOL SendCHARInfo( int _id, float posX, float posY, float posZ, float dirX, float dirY, float dirZ );
};

#define GetSender PacketSender::GetInstance()


