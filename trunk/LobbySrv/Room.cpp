#include "Room.h"
#include "LobbyChar.h"
#include "LobbySession.h"
#include "SLogger.h"
#include "SPacket.h"

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

		m_listPlayer.clear();

		m_nowPleyrCount = m_readyCount = 0;
		m_leader = NULL;

		//m_isPlay = FALSE;
		m_roomState = 0;

		_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 ); 
		
		m_AttectTeam = m_DefenceTeam = 0;

		m_visible = FALSE;
	}
}

void Room::Init( int i )
{
	{
		SSynchronize Sync( this );

		m_roomNum = i;

		m_listPlayer.clear();

		m_nowPleyrCount = m_readyCount = 0;
		m_leader = NULL;

		//m_isPlay = FALSE;
		m_roomState = 0;

		_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 ); 

		m_AttectTeam = m_DefenceTeam = 0;

		m_visible = FALSE;
	}
}

BOOL Room::PossiblePlay()
{
	//��� ready���°� �ƴϸ� �ȵ�
	//������ ī��Ʈ�� ���� �Ѵ�
	if( (m_nowPleyrCount-1) != m_readyCount )
		return FALSE;

	//�ּ��ο����� ������ �ȵ�
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	//���� ������ �ٸ��� �ȵ�
	if( m_AttectTeam != m_DefenceTeam )
		return FALSE;

	return TRUE;
}

BOOL Room::SetPlay()
{
	SSynchronize Sync( this );

	//���� ���������� �����
	m_roomState = ROOM_STATE_PLAYING;

	//���� ��� �÷��̾��� ���ǿ� ǥ���Ѵ�.
	{
		SSynchronize Sync( this );

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//ĳ���͸� ���� �� ���·� ����� �ش�
			(*iter)->SetIsPlay();
		}
	}

	return TRUE;
}

BOOL Room::SetReady()
{
	SSynchronize Sync( this );

	m_roomState = ROOM_STATE_READY;

	return TRUE;
}

BOOL Room::CanInsert()
{
	//����°� �������� TRUE
	if( m_roomState == ROOM_STATE_NORMAL )
		return TRUE;

	//�ο��� �� ���� �ʾ����� TRUE
	if( m_nowPleyrCount < MAX_PLAYER_COUNT )
		return TRUE;

	//�׷��� �ʴٸ� �� ����
	return FALSE;	
}

void Room::SetLeader( LobbyChar* sessionId )
{
	{
		SSynchronize Sync( this );

		m_leader = sessionId;
	}
}

LobbyChar* Room::ChangeLeader()
{
	{
		SSynchronize Sync( this );

		m_leader = *(m_listPlayer.begin());

		return m_leader;
	}
}

BOOL Room::SetPlayerSession( LobbySession* session, LobbyChar* charSpace )
{
	{
		SSynchronize Sync( this );
		
		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			if( *iter == charSpace )
			{
				//�ش� session�� ã�� �ٲ��ش�
				(*iter)->SetSession( session );
				return TRUE;
			}
		}
	}

	return FALSE;
}

void Room::AddPlayerInRoom( LobbyChar* charspace )
{
	{
		SSynchronize Sync( this );

		//�켱 �� ������ �ϰ�
		charspace->SetTeam( GetTeam() );

		//list�� ����
		m_listPlayer.push_back( charspace );

		++m_nowPleyrCount;
	}
}

BOOL Room::DelPlayerInRoom( LobbyChar* charspace )
{
	{
		SSynchronize Sync( this );

		if( m_listPlayer.empty() )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::DelPlayerInRoom()\nlist�� ��� �ֽ��ϴ�.����� ����...\n\n") );
			return TRUE;
		}

		//�� �ο��� ���ش�
		if( charspace->GetTeam() == 0 )
			--m_AttectTeam;
		else
			--m_DefenceTeam;

		//ready���¿����� �װ͵� ����� ��
		if( charspace->GetReady() )
			--m_readyCount;

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//��ġ�ϸ� ������
			if( *iter == charspace )
			{
				m_listPlayer.erase( iter );
				--m_nowPleyrCount;
				break;
			}
		}
	
		//ĳ���Ͱ� ������ FALSE�� return�� ����
		if( m_listPlayer.empty() )
			return FALSE;
		else
			return TRUE;
	}

}

BOOL Room::DelPlayerInRoom( int sessionId )
{
	{
		SSynchronize Sync( this );

		if( m_listPlayer.empty() )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::DelPlayerInRoom()\nlist�� ��� �ֽ��ϴ�.����� ����...\n\n") );
			return TRUE;
		}

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//��ġ�ϸ� ������
			if( (*iter)->GetSessionID() == sessionId )
			{
				m_listPlayer.erase( iter );
				break;
			}
		}

		//ĳ���Ͱ� ������ FALSE�� return�� ����
		if( m_listPlayer.empty() )
			return FALSE;
		else
			return TRUE;

	}
}

void Room::ChangReadyState( BOOL isReady )
{
	SSynchronize Sync( this );

	if( isReady )
		++m_readyCount;
	else
		--m_readyCount;

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("readyCount : %d"), m_readyCount );
}

void Room::ChangeTeam( BOOL isATT )
{
	SSynchronize Sync( this );

	if( isATT == 0 )
	{
		++m_AttectTeam;
		--m_DefenceTeam;
	}
	else
	{
		--m_AttectTeam;
		++m_DefenceTeam;
	}

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_DefenceTeam���� %d���� �˴ϴ�"), m_DefenceTeam );
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_AttectTeam���� %d���� �˴ϴ�"), m_AttectTeam );

	//Ȥ�ö� ������̶� 0 �Ʒ��� �������� ������
	if( m_AttectTeam < 0 )
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeTeam()\natt���� ���� 0���ϰ� �˴ϴ�.\n\n") );
	else if( m_DefenceTeam < 0 )
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeTeam()\ndef���� ���� 0���ϰ� �˴ϴ�.\n\n") );

}

int Room::GetTeam()
{
	SSynchronize Sync( this );

	//�� ���� ���� �ο��̸� �켱������ �������� �����ȴ�.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_DefenceTeam���� %d���� �˴ϴ�"), m_DefenceTeam );
		return ROOM_TEAM_DEF;
	}
	else
	{
		++m_AttectTeam;
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_AttectTeam���� %d���� �˴ϴ�"), m_AttectTeam );
		return ROOM_TEAM_ATT;
	}
}

void Room::PackageRoomInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	packet << m_roomNum;
	packet << m_nowPleyrCount;
	int size = _tcslen( m_tstrRoomTitle ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrRoomTitle, size );
	packet << m_roomState;
}

void Room::SendPacketAllInRoom( SPacket &packet, LobbyChar* itMe /*= NULL */ )
{
	SSynchronize Sync( this );

	if( m_listPlayer.empty() )
		return;

	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		//���� �Ѱ����� ���� ������ �ʾƾ� �Ѵ�
		if( *iter == itMe )
			continue;

		(*iter)->GetSession()->SendPacket( packet );
	}
}

void Room::PackagePlayerInRoom( SPacket &packet, LobbyChar* itme /*= NULL */ )
{
	SSynchronize Sync( this );

	if( m_listPlayer.empty() )
		return;

	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		//���� �Ѱ����� ���� ������ �ʾƾ� �Ѵ�
		if( *iter == itme )
			continue;

		(*iter)->PackageMyInfoForRoom( packet );
	}
}

void Room::PackagePlayerInRoomForGame( SPacket &packet, LobbyChar* itMe /*= NULL */ )
{
	SSynchronize Sync( this );

	if( m_listPlayer.empty() )
		return;

	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		//���� �Ѱ����� ���� ������ �ʾƾ� �Ѵ�
		if( *iter == itMe )
			continue;

		(*iter)->PackageMyInfoForGame( packet );
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

// void RoomMgr::CreateRoomSpace()
// {
// 	//1���� ROOMCOUNT������ ���� ����� ���´�.
// 	for( int i=1; i<=ROOMCOUNT; ++i )
// 	{
// 		Room* tmpRoom = new Room;
// 		tmpRoom->Init( i );
// 
// 		m_mapRoomlist[i] = tmpRoom;
// 	}
// }
// 
// void RoomMgr::Release()
// {
// 	for( int i=1; i<=ROOMCOUNT; ++i )
// 	{
// 		delete m_mapRoomlist[i];
// 		m_mapRoomlist[i] = 0;
// 	}
// 	m_mapRoomlist.clear();
// }
// 
// Room* RoomMgr::OpenRoom( int roomNum, TCHAR* title )
// {
// 	SSynchronize Sync( m_critical );
// 
// 	Room* tmpRoom = FindRoom( roomNum );
// 
// 	if( tmpRoom == NULL )
// 		return NULL;
// 
// 	//�̹� �����Ǿ� ������ �ǵ��� ����
// 	if( tmpRoom->IsOpen() )
// 		return NULL;
// 
// 	tmpRoom->SetTitle( title );
// 
// 	return tmpRoom;
// }
// 
// Room* RoomMgr::OpenRoom( TCHAR* title )
// {
// 	SSynchronize Sync( m_critical );
// 
// 	Room* tmpRoom = FindRoom( roomNum );
// 
// 	if( tmpRoom == NULL )
// 		return NULL;
// 
// 	//�̹� �����Ǿ� ������ �ǵ��� ����
// 	if( tmpRoom->IsOpen() )
// 		return NULL;
// 
// 	tmpRoom->SetTitle( title );
// 
// 	return tmpRoom;
// }
// 
// void RoomMgr::CloseRoom( int roomNum )
// {
// 	{
// 		SSynchronize Sync( m_critical );
// 
// 		if( m_mapRoomlist.find( roomNum ) == m_mapRoomlist.end() )
// 			return;
// 
// 		m_mapRoomlist[roomNum]->Init();
// 	}
// }
// 
// Room* RoomMgr::FindRoom( int roomNum )
// {
// 	if( m_mapRoomlist.find( roomNum ) == m_mapRoomlist.end() )
// 		return NULL;
// 
// 	return m_mapRoomlist[roomNum];
// }
// 
// void RoomMgr::PackageRoomInfoAll( SPacket &packet )
// {
// 	{
// 		SSynchronize Sync( m_critical );
// 
// 		for( int i=1; i<=ROOMCOUNT; ++i )
// 		{
// 			packet << i;				//���ȣ
// 			m_mapRoomlist[i]->PackageRoomInfo( packet );
// 		}
// 	}
// }


void RoomMgr::CreateRoomSpace()
{
	SSynchronize Sync( m_critical );

	m_iOpenRoomCount = 0;

	//�� ������ ����� ����
	m_vecRoom.reserve( ROOMCOUNT );
	for( int i=0; i<ROOMCOUNT; ++i )
	{
		Room* tmpRoom = new Room;
		tmpRoom->Init( i );

		m_vecRoom[i] = tmpRoom;
	}

	m_listOpenRoom.clear();

	m_roomIndexQ.Create( ROOMCOUNT, 0 );
}

void RoomMgr::Release()
{
	SSynchronize Sync( m_critical );

	for( int i=0; i<ROOMCOUNT; ++i )
	{
		delete m_vecRoom[i];
		m_vecRoom[i] = 0;
	}
	m_vecRoom.clear();
	m_listOpenRoom.clear();

	m_roomIndexQ.Release();
}

Room* RoomMgr::OpenRoom( TCHAR* title )
{
	SSynchronize Sync( m_critical );

	int roomNum = m_roomIndexQ.GetIndex();
	if( roomNum < 0 )
		return NULL;

	//�̹� ���� �ִ� ������ �ѹ��� Ȯ���Ѵ�
	if( m_vecRoom[roomNum]->IsOpen() )
		return NULL;

	m_vecRoom[roomNum]->SetTitle( title );
	++m_iOpenRoomCount;

	return m_vecRoom[roomNum];
}

void RoomMgr::CloseRoom( int roomNum )
{
	SSynchronize Sync( m_critical );

	if( roomNum < 0 || roomNum > ROOMCOUNT )
		return;

	--m_iOpenRoomCount;

	m_vecRoom[roomNum]->Init();
	//���ȣ�� ������ ���� ��ȯ���ش�
	m_roomIndexQ.PutIndex( roomNum );
}

Room* RoomMgr::FindRoom( int roomNum )
{
	SSynchronize Sync( m_critical );

	if( roomNum < 0 || roomNum > ROOMCOUNT )
		return NULL;

	return m_vecRoom[roomNum];
}

// void RoomMgr::PackageAllRoomInfo( SPacket &packet )
// {
// 	SSynchronize Sync( m_critical );
// 
// 	for( int i=1; i<=ROOMCOUNT; ++i )
// 	{
// 		m_vecRoom[i]->PackageRoomInfo( packet );
// 	}
// }

void RoomMgr::PackageOpenRoomInfo( SPacket &packet )
{
	SSynchronize Sync( m_critical );

	//���� �ִ� ���� �������� �ִ´�
	packet << m_iOpenRoomCount;

	std::list<Room*>::iterator iter;
	iter = m_listOpenRoom.begin();

	for( ; iter != m_listOpenRoom.end(); ++iter )
	{
		(*iter)->PackageRoomInfo( packet );
	}
}
