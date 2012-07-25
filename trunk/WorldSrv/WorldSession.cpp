#include "WorldSession.h"
#include "SSynchronize.h"


SIMPLEMENT_DYNAMIC(WorldSession)
SIMPLEMENT_DYNCREATE(WorldSession)


SLogger*	WorldSession::m_logger		= &GetLogger;



WorldSession::WorldSession(void)
{
}

WorldSession::~WorldSession(void)
{
}

void WorldSession::OnCreate()
{
	SSession::OnCreate();

	//���� �Ǿ��ٴ� ��Ŷ
	SendPacket( WF_WORLDSRV_CONNECT_OK );
}

void WorldSession::OnDestroy()
{
	//�Ҳ� �ϰ�

	SSession::OnDestroy();
}

void WorldSession::PacketParsing( SPacket& packet )
{
	SSynchronize sync( this );

	switch( packet.GetID() )
	{
	case FW_REQ_FIELD_SPACE:
		ReqFieldInfo();
		break;
	case FW_NOTIFY_READY:
		RecvReady( packet );
		break;
	case FW_NPC_CREATED:
		RecvNpcCreated( packet );
		break;
	case FW_NPC_CHANGE_STATE:
		RecvChangeState( packet );
		break;
	case FW_NPC_MOVE_CONTROL:
		RecvMoveNpcControl( packet );
		break;
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("WorldSession::PacketParsing()\n%d�� ��ŶID�� �������� �ʽ��ϴ�.\n\n"), packet.GetID() );
	}
}

void WorldSession::ReqFieldInfo()
{
	
}

void WorldSession::RecvReady( SPacket &packet )
{
	
}

void WorldSession::RecvNpcCreated( SPacket &packet )
{
	
}

void WorldSession::RecvChangeState( SPacket &packet )
{
	
}

void WorldSession::RecvMoveNpcControl( SPacket &packet )
{
	
}
