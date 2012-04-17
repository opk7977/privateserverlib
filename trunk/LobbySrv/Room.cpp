#include "Room.h"
#include "LobbySession.h"
#include "SSessionMgr.h"

//Room

Room::Room(void)
{
}

Room::~Room(void)
{
}

void Room::Init()
{
	m_mapPlayerlist.clear();

	m_nowPleyrCount = 0;
	m_leader = 0;

	m_visible = FALSE;
	m_isPlay = FALSE;

	_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 ); 
	
	m_AttectTeam = m_DefenceTeam = 0;
}

BOOL Room::PossiblePlay()
{
	//모두 ready상태가 아니면 안됨
	if( m_nowPleyrCount != m_readyCount )
		return FALSE;

	//최소인원보다 적으면 안됨
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	//팀의 비율이 다르면 안됨
	if( m_AttectTeam == m_DefenceTeam )
		return FALSE;

	return TRUE;
}

BOOL Room::SetPlay()
{
	//방을 게임중으로 만들고

	//방의 모든 플레이어의 세션에 표시한다.

	//방의 모든 플레이어에게 게임서버로 이동하라는 페킷을 보낸다.

	return TRUE;
}

void Room::SetLeader( int sessionId )
{
	m_leader = sessionId;
}

void Room::SetPlayerIndex( int sessionId, int Index )
{
	//존재하지 않으면 돌아 간다.
	if( m_mapPlayerlist.find(sessionId) == m_mapPlayerlist.end() )
		return;

	m_mapPlayerlist[sessionId] = Index;
}

void Room::AddPlayerInRoom( int sessionId, int index )
{
	//존재하면 돌아 간다.
	if( m_mapPlayerlist.find(sessionId) != m_mapPlayerlist.end() )
		return;

	m_mapPlayerlist[sessionId] = index;

	++m_nowPleyrCount;
}

BOOL Room::DelPlayerInRoom( int sessionId )
{
	//존재하지 않으면 돌아 간다.
	if( m_mapPlayerlist.find(sessionId) == m_mapPlayerlist.end() )
		return TRUE;

	m_mapPlayerlist.erase( sessionId );

	if( --m_nowPleyrCount <= 0 )
		return FALSE;

	return TRUE;
}

void Room::ChangeReadyState( BOOL isReady /*= TRUE */ )
{
	if( isReady )
		++m_readyCount;
	else
		--m_readyCount;
}

void Room::TeamCount( int team, BOOL isCountUp /*= TRUE */ )
{
	if( team == 0 )
		isCountUp == TRUE ? ++m_AttectTeam : --m_AttectTeam;
	else
		isCountUp == TRUE ? ++m_DefenceTeam : --m_DefenceTeam;
}

void Room::PackageRoomInfo( SPacket &packet )
{
	packet << m_nowPleyrCount;
	int size = _tcslen( m_tstrRoomTitle ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrRoomTitle, size );
	packet << m_isPlay;
}

void Room::SendPacketAllInRoom( SPacket &packet, LobbySession* mySession )
{
	LobbySession* tmpSession;

	for( int i=1; i<=m_nowPleyrCount; ++i )
	{
		tmpSession = (LobbySession*)GetSessionMgr.GetSession( m_mapPlayerlist[i] );
		if( tmpSession == NULL )
			continue;

		if( tmpSession == mySession )
			continue;

		tmpSession->SendPacket( packet );
	}
}

int Room::GetTeam()
{
	//두 팀이 같은 인원이면 우선권으로 공격팀에 배정된다.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
		return 0;
	}
	else
	{
		++m_AttectTeam;
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////

//RoomMgr

RoomMgr::RoomMgr()
{
}

RoomMgr::~RoomMgr()
{
	Release();
}

void RoomMgr::CreateRoomSpace()
{
	//1-6까지의 방을 만들어 놓는다.
	for( int i=1; i<=ROOMCOUNT; ++i )
	{
		Room* tmpRoom = new Room;
		tmpRoom->Init();

		m_mapRoomlist[i] = tmpRoom;
	}
}

void RoomMgr::Release()
{
	for( int i=1; i<=ROOMCOUNT; ++i )
	{
		delete m_mapRoomlist[i];
		m_mapRoomlist[i] = 0;
	}
	m_mapRoomlist.clear();
}

BOOL RoomMgr::OpenRoom( int roomNum, int SessionID, int iocpHandle, TCHAR* title )
{
	Room* tmpRoom = FindRoom( roomNum );

	if( tmpRoom == NULL )
		return FALSE;

	tmpRoom->SetTitle( title );
	tmpRoom->AddPlayerInRoom( SessionID, iocpHandle );
	tmpRoom->SetLeader( SessionID );

	return TRUE;
}

void RoomMgr::CloseRoom( int roomNum )
{
	if( m_mapRoomlist.find( roomNum ) == m_mapRoomlist.end() )
		return;

	m_mapRoomlist[roomNum]->Init();
}

Room* RoomMgr::FindRoom( int roomNum )
{
	if( m_mapRoomlist.find( roomNum ) == m_mapRoomlist.end() )
		return NULL;

	return m_mapRoomlist[roomNum];
}

void RoomMgr::PackageRoomInfoAll( SPacket &packet )
{
	for( int i=1; i<=ROOMCOUNT; ++i )
	{
		packet << i;
		m_mapRoomlist[i]->PackageRoomInfo( packet );
	}
}


