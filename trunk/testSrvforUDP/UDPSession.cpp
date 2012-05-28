#include "UDPSession.h"
#include "PlayerMgr.h"
#include "SSynchronize.h"

SIMPLEMENT_DYNAMIC(UDPSession)
SIMPLEMENT_DYNCREATE(UDPSession)



PlayerMgr*	UDPSession::m_playerMgr	= &GetPlayerMgr;
int			UDPSession::m_sessionID	= 1;


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

	m_player->SetSession( this );

	SendConnectOK();
}

void UDPSession::OnDestroy()
{
	SSynchronize Sync( this );

	SendPlayerDisconnect();

	m_playerMgr->DelPlayer( m_player );

	SSession::OnDestroy();
}

void UDPSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	case UDPSERVER_ASK_INFO:
		RecvASKInfo( packet );
		break;
	default:
		;
	}
}

//--------------------------------------------------------------

void UDPSession::RecvASKInfo( SPacket& packet )
{
	SSynchronize Sync( this );

	int _id, port;

	packet >> _id;
	packet >> port;

	SOCKADDR_IN	sockAddr;
	int size = sizeof( sockAddr );
	if( getpeername( GetSocket(), (SOCKADDR*)&sockAddr, &size ) == SOCKET_ERROR )
	{
		SSession::OnDestroy();
		return;
	}

	if( m_player == NULL )
	{
		SSession::OnDestroy();
		return;
	}

	m_player->SetID( _id );
	m_player->SetIP( inet_ntoa( sockAddr.sin_addr ) );
	m_player->SetPort( port );

	SendPlayerInfo();
}


//--------------------------------------------------------------

void UDPSession::SendConnectOK()
{
	SPacket sendPacket( UDPSERVER_CONNECT_OK );
	sendPacket << m_sessionID++;

	SendPacket( sendPacket );
}

void UDPSession::SendPlayerInfo()
{
	SPacket sendPacket( UDPSERVER_PLAYER_INFO );
	
	m_playerMgr->PackageAllPlayer( &sendPacket, m_player );

	SendPacket( sendPacket );
}

void UDPSession::SendPlayerDisconnect()
{
	SPacket sendPacket( UDPSERVER_PLAYER_DISCONNECT );
	sendPacket << m_player->GetID();

	m_playerMgr->SendAllPlayer( &sendPacket, m_player );
}
