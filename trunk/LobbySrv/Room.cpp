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
	//��� ready���°� �ƴϸ� �ȵ�
	if( m_nowPleyrCount != m_readyCount )
		return FALSE;

	//�ּ��ο����� ������ �ȵ�
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	//���� ������ �ٸ��� �ȵ�
	if( m_AttectTeam == m_DefenceTeam )
		return FALSE;

	return TRUE;
}

BOOL Room::SetPlay()
{
	//���� ���������� �����

	//���� ��� �÷��̾��� ���ǿ� ǥ���Ѵ�.

	//���� ��� �÷��̾�� ���Ӽ����� �̵��϶�� ��Ŷ�� ������.

	return TRUE;
}

void Room::SetLeader( int sessionId )
{
	m_leader = sessionId;
}

void Room::SetPlayerIndex( int sessionId, int iocpKey )
{
	//�������� ������ ���� ����.
	if( m_mapPlayerlist.find(sessionId) == m_mapPlayerlist.end() )
		return;

	m_mapPlayerlist[sessionId] = iocpKey;
}

int Room::AddPlayerInRoom( int sessionId, int iocpKey )
{
	//�����ϸ� ���� ����.
	if( m_mapPlayerlist.find(sessionId) != m_mapPlayerlist.end() )
		return -1;

	m_mapPlayerlist[sessionId] = iocpKey;

	++m_nowPleyrCount;

	return GetTeam();
}

BOOL Room::DelPlayerInRoom( int sessionId )
{
	//�������� ������ ���� ����.
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

void Room::TeamChange( int basicTeam )
{
	if( basicTeam == 0 )
	{
		//���ݿ��� �����
		++m_DefenceTeam;
		--m_AttectTeam;
	}
	else
	{
		//���񿡼� ��������
		++m_AttectTeam;
		--m_DefenceTeam;
	}
}

void Room::PackageRoomInfo( SPacket &packet )
{
	packet << m_nowPleyrCount;
	int size = _tcslen( m_tstrRoomTitle ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrRoomTitle, size );
	packet << m_isPlay;
}

void Room::SendPacketAllInRoom( SPacket &packet, LobbySession* mySession /*= NULL*/ )
{
	LobbySession* tmpSession;

	std::map<int, int>::iterator iter;
	iter = m_mapPlayerlist.begin();
	for( ; iter != m_mapPlayerlist.end(); ++iter )
	{
		tmpSession = (LobbySession*)GetSessionMgr.GetSession( iter->second );
		if( tmpSession == NULL )
			continue;

		if( tmpSession == mySession )
			continue;

		tmpSession->SendPacket( packet );
	}
}

void Room::PackageAllPlayerInRoom( SPacket &packet )
{
	//�켱 �ο����� ���
	packet << m_nowPleyrCount;

	LobbySession* tmpSession;

	std::map<int, int>::iterator iter;
	iter = m_mapPlayerlist.begin();
	for( ; iter != m_mapPlayerlist.end(); ++iter )
	{
		tmpSession = (LobbySession*)GetSessionMgr.GetSession( iter->second );
		if( tmpSession == NULL )
			continue;

		tmpSession->PackageMyInfo( packet, TRUE );
	}
}

int Room::GetTeam()
{
	//�� ���� ���� �ο��̸� �켱������ �������� �����ȴ�.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
		return 1;
	}
	else
	{
		++m_AttectTeam;
		return 0;
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
	//1-6������ ���� ����� ���´�.
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

int RoomMgr::OpenRoom( int roomNum, int SessionID, int iocpHandle, TCHAR* title )
{
	Room* tmpRoom = FindRoom( roomNum );

	if( tmpRoom == NULL )
		return -1;

	tmpRoom->SetTitle( title );
	int team = tmpRoom->AddPlayerInRoom( SessionID, iocpHandle );
	tmpRoom->SetLeader( SessionID );

	return team;
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


