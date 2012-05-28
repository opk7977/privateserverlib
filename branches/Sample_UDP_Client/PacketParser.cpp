#include "PacketParser.h"
#include "SPacketQueue.h"
#include "CharMgr.h"

PacketParser::PacketParser(void)
{
	m_playerMgr = &GetCharMgr;
}

PacketParser::~PacketParser(void)
{
	Release();
}

void PacketParser::Init()
{

}

void PacketParser::Release()
{

}

void PacketParser::PacketProcess()
{
	int count = GetPacketQ.GetDataCount();

	if( count <= 0 )
		return;

	for( int i=0; i<count; ++i )
	{
		GetPacketQ.GetPacket( m_packet );

		//패킷 처리
		PacketParsing();
	}
}

void PacketParser::PacketParsing()
{
	switch( m_packet.GetID() )
	{
	case UDPSERVER_CONNECT_OK:
		RecvConnectOK();
		break;
	case UDPSERVER_PLAYER_INFO:
		RecvPlayerInfo();
		break;
	case UDPSERVER_PLAYER_DISCONNECT:
		RecvPlayerDisconnect();
		break;
	case UDP_CHAR_INFO:
		RecvUDPCharInfo();
		break;
	default:
		break;
	}
}

void PacketParser::RecvConnectOK()
{
	//받아온 id셋팅
}

void PacketParser::RecvPlayerInfo()
{
	//player를 생성하고
	//ip와 port번호로 udp정보 추가
}

void PacketParser::RecvPlayerDisconnect()
{
	//id로 캐릭터 지우고
	//udp도 지워준다
}

void PacketParser::RecvUDPCharInfo()
{
	//해당 캐릭커의 정보를 갱신
}
