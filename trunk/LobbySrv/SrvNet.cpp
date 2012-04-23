#include "SrvNet.h"
#include "SLogger.h"
#include "LobbySession.h"

SrvNet::SrvNet(void) : m_GameSrv(NULL)
{
}

SrvNet::~SrvNet(void)
{
}

void SrvNet::SetSession( LobbySession* session )
{
	m_GameSrv = session;
}

BOOL SrvNet::SendToGameServer( SPacket &packet )
{
	if( m_GameSrv == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("SrvNet::SendToGameServer()\n서버와의 연결이 되어 있지 않습니다.\n\n") );
		return FALSE;
	}

	if( m_GameSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("SrvNet::SendToGameServer()\n보낸패킷이 패킷의 크기가 다릅니다\n\n") );
		return FALSE;
	}

	return TRUE;
}
