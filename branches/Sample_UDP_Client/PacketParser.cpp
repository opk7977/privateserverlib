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

		//��Ŷ ó��
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
	//�޾ƿ� id����
}

void PacketParser::RecvPlayerInfo()
{
	//player�� �����ϰ�
	//ip�� port��ȣ�� udp���� �߰�
}

void PacketParser::RecvPlayerDisconnect()
{
	//id�� ĳ���� �����
	//udp�� �����ش�
}

void PacketParser::RecvUDPCharInfo()
{
	//�ش� ĳ��Ŀ�� ������ ����
}
