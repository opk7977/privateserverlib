#include "UDPSession.h"
#include "PlayerMgr.h"
#include "SSynchronize.h"

SIMPLEMENT_DYNAMIC(UDPSession)
SIMPLEMENT_DYNCREATE(UDPSession)

PlayerMgr* UDPSession::m_playerMgr = &GetPlayerMgr;

UDPSession::UDPSession(void)
{
}

UDPSession::~UDPSession(void)
{
}

void UDPSession::OnCreate()
{
	SSynchronize Sync( this );

	SSession::OnCreate();

	m_player = m_playerMgr->AddPlayer();

	if( m_player == NULL )
	{
		SSession::OnDestroy();
		return;
	}

	SendConnectOK();
}

void UDPSession::OnDestroy()
{
	SendPlayerDisconnect();

	SSession::OnDestroy();
}

void UDPSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	case :
		SendPlayerInfo( packet );
		break;
	default:
		;
	}
}

//--------------------------------------------------------------

void UDPSession::RecvASKInfo( SPacket& packet )
{
}

//--------------------------------------------------------------

void UDPSession::SendConnectOK( int id )
{
}

void UDPSession::SendPlayerInfo()
{
}

void UDPSession::SendPlayerDisconnect()
{
}
