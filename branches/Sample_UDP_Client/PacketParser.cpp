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
	GetNetwork.SetConnectOK();
	//받아온 id셋팅
	int id;
	m_packet >> id;

	//나를 추가
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


		//캐릭터 생성
		m_playerMgr->AddChar( _id );

		//네트워크에 추가
		m_udp->AddSockAddr( _id, ip, port );
	}
}

void PacketParser::RecvPlayerDisconnect()
{
	int _id;
	m_packet >> _id;

	//id로 캐릭터 지우고
	m_playerMgr->DelChar( _id );

	//udp도 지워준다
	m_udp->DelSockAddr( _id );
}

void PacketParser::RecvUDPCharInfo()
{
	//해당 캐릭커의 정보를 갱신
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
