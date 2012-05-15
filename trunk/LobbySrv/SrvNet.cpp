#include "SrvNet.h"
#include "SLogger.h"
#include "LobbySession.h"

SrvNet::SrvNet(void) : m_GameSrv(NULL)
{
	m_logger = &GetLogger;
}

SrvNet::~SrvNet(void)
{
}

BOOL SrvNet::SendToGameServer( SPacket &packet )
{
	if( m_GameSrv == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("SrvNet::SendToGameServer()\n�������� ������ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_GameSrv->SendPacket( packet ) != packet.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("SrvNet::SendToGameServer()\n������Ŷ�� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�\n\n") );
		return FALSE;
	}

	return TRUE;
}
