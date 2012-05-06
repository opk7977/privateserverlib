#include "LobbyMgr.h"
#include "LobbyChar.h"
#include "LobbySession.h"

SServerObj* LobbyMgr::m_critical = new SServerObj;


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
	m_iPlayerCountInLobby = 0;
}

void LobbyMgr::Release()
{
	m_listPlayerInLobby.clear();
	m_iPlayerCountInLobby = 0;
}

void LobbyMgr::SendPacketAllInLobby( SPacket& packet, LobbyChar* mySession /*= NULL*/ )
{
	{
		SSynchronize sync( m_critical );

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
}

void LobbyMgr::PackageDataAllInLobby( SPacket& packet )
{
	{
		SSynchronize sync( m_critical );

		if( m_listPlayerInLobby.empty() )
			return;

		packet << m_iPlayerCountInLobby;

		std::list<LobbyChar*>::iterator iter = m_listPlayerInLobby.begin();

		for( ; iter != m_listPlayerInLobby.end(); ++iter )
			(*iter)->PackageMyInfo( packet );
	}
}

void LobbyMgr::AddUser( LobbyChar* _session )
{
	{
		SSynchronize sync( m_critical );

		m_listPlayerInLobby.push_back( _session );
		++m_iPlayerCountInLobby;
	}
}

void LobbyMgr::MinusUser( LobbyChar* _session )
{
	{
		SSynchronize sync( m_critical );

		if( m_listPlayerInLobby.empty() )
			return;

		m_listPlayerInLobby.remove( _session );
		--m_iPlayerCountInLobby;
	}
}
