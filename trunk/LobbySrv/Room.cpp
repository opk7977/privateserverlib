#include "Room.h"
#include "LobbySession.h"
#include "SSessionMgr.h"
#include "SLogger.h"

/*#include "SSynchronize.h"*/

RoomChar::RoomChar()
{
	Init();
}

RoomChar::RoomChar( const RoomChar& p )
{
	m_iocpKey = p.m_iocpKey;
	m_charTeam = p.m_charTeam;
	m_ready = p.m_ready;
	_tcsncpy_s( m_charID, 50, p.m_charID, _tcslen(p.m_charID) );
}

RoomChar::~RoomChar()
{
}

void RoomChar::Init()
{
	m_iocpKey = 0;
	ZeroMemory( m_charID, 50 );
	m_charTeam = m_ready = -1;
}

void RoomChar::SetIocp( int key )
{
	m_iocpKey = key;
}

int RoomChar::GetIocp()
{
	return m_iocpKey;
}

void RoomChar::SetID( TCHAR *uId )
{
	_tcsncpy_s( m_charID, 50, uId, _tcslen(uId) );
}

TCHAR* RoomChar::GetID()
{
	return m_charID;
}

void RoomChar::SetTeam( int team )
{
	m_charTeam = team;
}

int RoomChar::GetTeam()
{
	return m_charTeam;
}

void RoomChar::SetReady( int ready )
{
	m_ready = ready;
}

int RoomChar::GetReady()
{
	return m_ready;
}



//Room

Room::Room(void)
{
}

Room::~Room(void)
{
}

void Room::Init()
{
	{
		SSynchronize Sync( this );

		//m_mapPlayerlist.clear();
		m_mapPlayer.clear();

		m_nowPleyrCount = m_readyCount = 0;
		m_leader = 0;

		m_isPlay = FALSE;

		_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 ); 
		
		m_AttectTeam = m_DefenceTeam = 0;

		m_visible = FALSE;
	}
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
	{
		SSynchronize Sync( this );

		m_leader = sessionId;
	}
}

int Room::ChangeLeader()
{
	{
		SSynchronize Sync( this );

		LobbySession* tmpSession = (LobbySession*)GetSessionMgr.GetSession( (m_mapPlayer.begin()->second).GetIocp() );
		if( tmpSession == NULL )
			return -1;

		m_leader = tmpSession->GetSessionID();

		return m_leader;
	}
}

void Room::SetPlayerIocp( int sessionId, int iocpKey )
{
	{
		SSynchronize Sync( this );

		//�������� ������ ���� ����.
		if( m_mapPlayer.find(sessionId) == m_mapPlayer.end() )
			return;

		m_mapPlayer[sessionId].SetIocp( iocpKey );
	}
}

int Room::AddPlayerInRoom( int sessionId, int iocpKey, TCHAR* charID )
{
	RoomChar tmpChar;

	{
		SSynchronize Sync( this );

		//�����ϸ� ���� ����.
		if( m_mapPlayer.find(sessionId) != m_mapPlayer.end() )
			return -1;

		//RoomChar tmpChar;
		tmpChar.SetIocp( iocpKey );
		tmpChar.SetID( charID );
		tmpChar.SetTeam( GetTeam() );
		tmpChar.SetReady(0);

		m_mapPlayer[sessionId] = tmpChar;

		++m_nowPleyrCount;
	}

	return tmpChar.GetTeam();
}

BOOL Room::DelPlayerInRoom( int sessionId )
{
	SSynchronize Sync( this );

	//�������� ������ ���� ����.
	if( m_mapPlayer.find(sessionId) == m_mapPlayer.end() )
		return TRUE;

	m_mapPlayer.erase( sessionId );

	if( --m_nowPleyrCount <= 0 )
		return FALSE;

	return TRUE;
}

int Room::ChangeReadyState( int sessionId )
{
	SSynchronize Sync( this );

	if( m_mapPlayer.find( sessionId ) == m_mapPlayer.end() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeReadyState()\nĳ���� ������ �����ϴ�.\n\n") );
		return -1;
	}

	if( m_mapPlayer[sessionId].GetReady() == 0 )
	{
		m_mapPlayer[sessionId].SetReady( 1 );

		if( ++m_readyCount > MAX_PLAYER_COUNT )
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeReadyState()\n���� �ο��� �ִ� �ο��� �Ѿ����ϴ�.\n\n") );
	}
	else
	{
		m_mapPlayer[sessionId].SetReady( 0 );

		if( --m_readyCount < 0 )
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeReadyState()\n���� �ο��� 0���� �����ϴ�.\n\n") );
	}

	return m_mapPlayer[sessionId].GetReady();
}

int Room::TeamChange( int sessionID )
{
	SSynchronize Sync( this );

	//�������� ������ ���� ����.
	if( m_mapPlayer.find(sessionID) == m_mapPlayer.end() )
		return -1;

	if( m_mapPlayer[sessionID].GetTeam() == 0 )
	{
		m_mapPlayer[sessionID].SetTeam( 1 );
		--m_AttectTeam;
		++m_DefenceTeam;
		return 1;
	}
	else
	{
		m_mapPlayer[sessionID].SetTeam( 0 );
		++m_AttectTeam;
		--m_DefenceTeam;
		return 0;
	}
}

void Room::PackageRoomInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	packet << m_nowPleyrCount;
	int size = _tcslen( m_tstrRoomTitle ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrRoomTitle, size );
	packet << m_isPlay;
}

void Room::SendPacketAllInRoom( SPacket &packet, LobbySession* mySession /*= NULL*/ )
{
	SSynchronize Sync( this );

	LobbySession* tmpSession;
	std::map<int, RoomChar>::iterator iter;
	iter = m_mapPlayer.begin();
	for( ; iter != m_mapPlayer.end(); ++iter )
	{
		tmpSession = (LobbySession*)GetSessionMgr.GetSession( (iter->second).GetIocp() );

		if( tmpSession == NULL )
			continue;

		if( tmpSession == mySession )
			continue;

		tmpSession->SendPacket( packet );
	}
}

void Room::PackageAllPlayerInRoom( SPacket &packet )
{
	SSynchronize Sync( this );

	//�켱 �ο����� ���
	packet << m_nowPleyrCount;

	LobbySession* tmpSession;

	std::map<int, RoomChar>::iterator iter;
	iter = m_mapPlayer.begin();
	for( ; iter != m_mapPlayer.end(); ++iter )
	{
		tmpSession = (LobbySession*)GetSessionMgr.GetSession( (iter->second).GetIocp() );
		if( tmpSession == NULL )
			continue;

		tmpSession->PackageMyInfo( packet );

		packet << (iter->second).GetTeam();
		packet << (iter->second).GetReady();
	}
}

int Room::GetTeam()
{
	SSynchronize Sync( this );

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
	m_critical = new SServerObj;
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

Room* RoomMgr::OpenRoom( int roomNum, TCHAR* title )
{
	SSynchronize Sync( m_critical );

	Room* tmpRoom = FindRoom( roomNum );

	if( tmpRoom == NULL )
		return NULL;

	//�̹� �����Ǿ� ������ �ǵ��� ����
	if( tmpRoom->IsOpen() )
		return NULL;

	tmpRoom->SetTitle( title );

	return tmpRoom;
}

void RoomMgr::CloseRoom( int roomNum )
{
	{
		SSynchronize Sync( m_critical );

		if( m_mapRoomlist.find( roomNum ) == m_mapRoomlist.end() )
			return;

		m_mapRoomlist[roomNum]->Init();
	}
}

Room* RoomMgr::FindRoom( int roomNum )
{
	if( m_mapRoomlist.find( roomNum ) == m_mapRoomlist.end() )
		return NULL;

	return m_mapRoomlist[roomNum];
}

void RoomMgr::PackageRoomInfoAll( SPacket &packet )
{
	{
		SSynchronize Sync( m_critical );

		for( int i=1; i<=ROOMCOUNT; ++i )
		{
			packet << i;
			m_mapRoomlist[i]->PackageRoomInfo( packet );
		}
	}
}


