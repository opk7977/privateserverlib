#include "stdafx.h"

#include "PacketParser.h"
#include "SPacketQueue.h"

#include "LogDataBase.h"

#include "LogSrvProtocol.h"

PacketParser::PacketParser(void)
{
	m_database	= &GetDB;
	m_packetQ	= &GetPacketQ;
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
	int count = m_packetQ->GetDataCount();

	if( count <= 0 )
		return;

	for( int i=0; i<count; ++i )
	{
		m_packetQ->GetPacket( m_packet );

		//패킷 처리
		PacketParsing();
	}
}

void PacketParser::PacketParsing()
{
	//한번정도 id만이라도 확인해 주자;
	if( m_packet.GetID() != RECV_SERVER_LOG )
		return;

	//MessageBox( NULL, _T("연결되었담"), _T("로그인 서버와"), MB_OK );

	int ServerID, Loglv, size;
	TCHAR Log[255]={0,};

	m_packet >> ServerID;
	m_packet >> Loglv;
	m_packet >> size;
	m_packet.GetData( Log, size );

 	m_database->InsertLog( ServerID, Loglv, Log );
}
