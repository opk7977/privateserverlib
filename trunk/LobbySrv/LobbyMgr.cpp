#include "LobbyMgr.h"
//#include "SPacket.h"
#include "LobbySession.h"

#include "SSynchronize.h"


SServerObj* LobbyMgr::m_critical = new SServerObj;


LobbyMgr::LobbyMgr(void)
{
	Init();
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

void LobbyMgr::SendPacketAllInLobby( SPacket& packet, LobbySession* mySession )
{
	{
		SSynchronize sync( m_critical );
		
		std::list<LobbySession*>::iterator iter = m_listPlayerInLobby.begin();

		for( ; iter != m_listPlayerInLobby.end(); ++iter )
		{
			//나한테는 보내지 않아도 된다.
			if( *iter == mySession )
				continue;

			(*iter)->SendPacket( packet );
		}
	}
}

void LobbyMgr::PackageDataAllInLobby( SPacket& packet )
{
	{
		SSynchronize sync( m_critical );

		std::list<LobbySession*>::iterator iter = m_listPlayerInLobby.begin();

		for( ; iter != m_listPlayerInLobby.end(); ++iter )
			(*iter)->PackageMyInfo( packet );
	}
}

void LobbyMgr::AddUser( LobbySession* _session )
{
	{
		SSynchronize sync( m_critical );

		m_listPlayerInLobby.push_back( _session );
		++m_iPlayerCountInLobby;
	}
}

void LobbyMgr::MinusUser( LobbySession* _session )
{
	{
		SSynchronize sync( m_critical );

		m_listPlayerInLobby.remove( _session );
	}
}
