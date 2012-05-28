#include "PlayerMgr.h"
#include "UDPSession.h"

#include "SSynchronize.h"

//--------------------------------------
// player
//--------------------------------------
Player::Player(void)
{
}

Player::~Player(void)
{
}

void Player::Init( int _id )
{
	m_id = _id;
	Init();
}

void Player::Init()
{
	m_session = NULL;
}

void Player::SetID( int _id )
{
	m_id = _id;
}

int Player::GetID() const
{
	return m_id;
}

void Player::SetSession( UDPSession* session )
{
	m_session = session;
}

UDPSession* Player::GetSession() const
{
	return m_session;
}

void Player::SetIP( char* ip )
{
	int size = strlen( ip );
	if( size > 15 )
		return;

	CopyMemory( m_ip, ip, size );
}

char* Player::GetIP()
{
	return &m_ip[0];
}

void Player::SetPort( int port )
{
	m_port = port;
}

int Player::GetPort() const
{
	return m_port;
}

void Player::PackageMyInfo( SPacket* packet )
{
	SSynchronize Sync( this );

	(*packet) << m_id;
	int size = strlen( m_ip );
	(*packet) << size;
	packet->PutData( m_ip, size );
	(*packet) << m_port;
}

//--------------------------------------
// playerMgr
//--------------------------------------
PlayerMgr::PlayerMgr(void)
{
}

PlayerMgr::~PlayerMgr(void)
{
}

BOOL PlayerMgr::Init( int playerCount )
{
	SSynchronize Sync( this );

	for( int i=1; i<=playerCount; ++i )
	{
		Player* tmpPlayer = new Player;
		tmpPlayer->Init( i );

		m_playerSpace[i] = tmpPlayer;
	}

	m_indexQ.Create( playerCount, 1 );

	return TRUE;
}

void PlayerMgr::Release()
{
	SSynchronize Sync( this );

	if( m_playerSpace.empty() )
		return;

	m_listPlayer.Clear();

	std::map<int, Player*>::iterator iter = m_playerSpace.begin();
	for( ; iter != m_playerSpace.end(); ++iter )
	{
		delete iter->second;
		iter->second = 0;
	}
	m_playerSpace.clear();
}

Player* PlayerMgr::AddPlayer()
{
	SSynchronize Sync( this );

	int index = m_indexQ.GetIndex();
	if( index < 0 )
		return NULL;

	m_listPlayer.AddItem( m_playerSpace[index] );

	return m_playerSpace[index];
}

BOOL PlayerMgr::DelPlayer( Player* player )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return FALSE;

	m_listPlayer.DelItem( player );

	int index = player->GetID();
	m_indexQ.PutIndex( index );

	player->Init();

	return TRUE;
}

Player* PlayerMgr::FindPlayer( int _id )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return NULL;

	std::list<Player*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd(iter); ++iter )
	{
		if( (*iter)->GetID() == _id )
			return (*iter);
	}

	//찾는 player가 없음
	return NULL;
}

void PlayerMgr::SendAllPlayer( SPacket* packet, Player* itme/* = NULL*/ )
{
	std::list<Player*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd(iter); ++iter )
	{
		if( *iter == itme )
			continue;

		(*iter)->GetSession()->SendPacket( *packet );
	}
}

void PlayerMgr::PackageAllPlayer( SPacket* packet, Player* itme/* = NULL*/ )
{
	//총 인원을 넣고
	(*packet) << m_listPlayer.GetItemCount();

	std::list<Player*>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd(iter); ++iter )
	{
		if( *iter == itme )
			continue;

		(*iter)->PackageMyInfo( packet );
	}
}