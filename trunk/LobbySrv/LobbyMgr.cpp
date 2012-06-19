#include "LobbyMgr.h"
#include "LobbyChar.h"
#include "LobbySession.h"

LobbyMgr::LobbyMgr(void)
: m_iPlayerCountInLobby(0)
{
}

LobbyMgr::~LobbyMgr(void)
{
	Release();
}

void LobbyMgr::Init()
{
	SSynchronize sync( this );

	m_iPlayerCountInLobby = 0;
}

void LobbyMgr::Release()
{
	SSynchronize sync( this );

	m_listPlayerInLobby.clear();
	m_iPlayerCountInLobby = 0;
}

void LobbyMgr::SendPacketAllInLobby( SPacket& packet, LobbyChar* mySession /*= NULL*/ )
{
	SSynchronize sync( this );

	if( m_listPlayerInLobby.empty() )
		return;
	
	std::list<LobbyChar*>::iterator iter = m_listPlayerInLobby.begin();

	for( ; iter != m_listPlayerInLobby.end(); ++iter )
	{
		//mySession이 NULL이 아니면 나한테는 보내지 말라는 것.
		if( *iter == mySession )
			continue;

		(*iter)->GetSession()->SendPacket( packet );
	}
}

void LobbyMgr::PackageDataAllInLobby( SPacket& packet )
{
	SSynchronize sync( this );

	if( m_listPlayerInLobby.empty() )
		return;

	packet << m_iPlayerCountInLobby;

	std::list<LobbyChar*>::iterator iter = m_listPlayerInLobby.begin();

	for( ; iter != m_listPlayerInLobby.end(); ++iter )
		(*iter)->PackageMyInfoForLobby( packet );
}

void LobbyMgr::AddUser( LobbyChar* _session )
{
	SSynchronize sync( this );

	m_listPlayerInLobby.push_back( _session );
	++m_iPlayerCountInLobby;
}

void LobbyMgr::MinusUser( LobbyChar* _session )
{
	SSynchronize sync( this );

	if( m_listPlayerInLobby.empty() )
		return;

	m_listPlayerInLobby.remove( _session );
	--m_iPlayerCountInLobby;
}
