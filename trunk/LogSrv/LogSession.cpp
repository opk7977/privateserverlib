#include "StdAfx.h"
#include "LogSession.h"

#include "SSessionMgr.h"
#include "LogDataBase.h"
#include "SPacketQueue.h"

SIMPLEMENT_DYNAMIC(LogSession)
SIMPLEMENT_DYNCREATE(LogSession)

LogSession::LogSession(void)
{
	m_database	= &GetDB;
	m_packetQ	= &GetPacketQ;
}

LogSession::~LogSession(void)
{
}

void LogSession::OnCreate()
{
	SSession::OnCreate();

	SendPacket( LOG_SERVER_CONNECT_OK );
}

void LogSession::OnDestroy()
{
	SSession::OnDestroy();
}

void LogSession::PacketParsing( SPacket& packet )
{
	//한번정도 id만이라도 확인해 주자;
	if( packet.GetID() != RECV_SERVER_LOG )
	{
		//
		return;
	}

	m_packetQ->PutPacket( packet.GetDataBufferPtr(), packet.GetPacketSize() );

// 	int ServerID, Loglv, size;
// 	TCHAR Log[255]={0,};
// 
// 	packet >> ServerID;
// 	packet >> Loglv;
// 	packet >> size;
// 	packet.GetData( Log, size );
// 
// 	m_database->InsertLog( ServerID, Loglv, Log );
}

