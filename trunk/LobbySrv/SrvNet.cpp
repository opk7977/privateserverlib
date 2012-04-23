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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("SrvNet::SendToGameServer()\n�������� ������ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_GameSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("SrvNet::SendToGameServer()\n������Ŷ�� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�\n\n") );
		return FALSE;
	}

	return TRUE;
}
