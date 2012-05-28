#include "PacketSender.h"
#include "SNetwork.h"
#include "SUDPNet.h"
#include "SPacket.h"

PacketSender::PacketSender(void)
{
	m_network	= &GetNetwork;
	m_udp		= &GetUNet;
}

PacketSender::~PacketSender(void)
{
}

BOOL PacketSender::SendASKInfo( int _id, int _port )
{
	SPacket sendPacket( UDPSERVER_ASK_INFO );

	sendPacket << _id;
	sendPacket << _port;

	return m_network->PilePacket( &sendPacket );
}

BOOL PacketSender::SendCHARInfo( int _id, float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	SPacket sendPacket( UDP_CHAR_INFO );

	sendPacket << _id;
	sendPacket << posX << posY << posZ;
	sendPacket << dirX << dirY << dirZ;

	//여기서 UDP로 보내기
	m_udp->SendPacketAllClient( sendPacket );

	return TRUE;
}

