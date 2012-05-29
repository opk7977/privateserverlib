#include "SNetwork.h"
#include "PacketParser.h"
#include "SPacketQueue.h"
#include "PacketSender.h"
#include "SUDPNet.h"
#include "CharMgr.h"

PacketParser::PacketParser(void)
{
	m_playerMgr = &GetCharMgr;
	m_sender	= &GetSender;
	m_udp		= &GetUNet;
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
	GetNetwork.SetConnectOK();
	//�޾ƿ� id����
	int id;
	m_packet >> id;

	//���� �߰�
	m_playerMgr->AddMe( id );

	m_sender->SendASKInfo( id, 4321 );
}

void PacketParser::RecvPlayerInfo()
{
	int count;
	int _id, size, port;
	char ip[20] = {0,};

	m_packet >> count;

	for( int i=0; i<count; ++i )
	{
		m_packet >> _id;
		m_packet >> size;
		m_packet.GetData( ip, size );
		m_packet >> port;


		//ĳ���� ����
		m_playerMgr->AddChar( _id );

		//��Ʈ��ũ�� �߰�
		m_udp->AddSockAddr( _id, ip, port );
	}
}

void PacketParser::RecvPlayerDisconnect()
{
	int _id;
	m_packet >> _id;

	//id�� ĳ���� �����
	m_playerMgr->DelChar( _id );

	//udp�� �����ش�
	m_udp->DelSockAddr( _id );
}

void PacketParser::RecvUDPCharInfo()
{
	//�ش� ĳ��Ŀ�� ������ ����
	int id;
	NVECTOR3 pos, dir;

	m_packet >> id;
	m_packet >> pos.x >> pos.y >> pos.z;
	m_packet >> dir.x >> dir.y >> dir.z;

	NObject* tmpObj;
	tmpObj = m_playerMgr->FindChar( id );
	if( tmpObj == NULL )
		return;

	tmpObj->SetPosition( pos.x, pos.y, pos.z );
	tmpObj->SetDirection( dir.x, dir.y, dir.z );
}
