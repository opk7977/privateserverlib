#include "Room.h"
#include "LobbyChar.h"
#include "LobbySession.h"
#include "SLogger.h"
#include "SPacket.h"

#include "DataLeader.h"


//Room
SLogger* Room::m_logger = &GetLogger;

Room::Room(void)
{
}

Room::~Room(void)
{
}

void Room::Init()
{
	SSynchronize Sync( this );

	m_listPlayer.clear();

	_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 ); 
	m_nowPleyrCount = m_readyCount = 0;
	m_leader = NULL;
	m_roomState = 0;
	
	m_AttectTeam = m_DefenceTeam = 0;
	m_visible = FALSE;
}

void Room::Init( int i )
{
	SSynchronize Sync( this );

	m_roomNum = i;

	Init();
}

BOOL Room::PossiblePlay()
{
	SSynchronize Sync( this );

	//���(-����) ready���°� �ƴϸ� ������ �� ����
	if( (m_nowPleyrCount-1) != m_readyCount )
		return FALSE;

	//�ּ��ο����� ������ ������ �� ����
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	return TRUE;
}

void Room::SetPlay()
{
	SSynchronize Sync( this );

	//���� ���������� �����
	m_roomState = ROOM_STATE_PLAYING;

	//���� ��� �÷��̾��� ���ǿ� ǥ���Ѵ�.
	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		//ĳ���͸� ���� �� ���·� ����� �ش�
		(*iter)->SetIsPlay();
	}
}

void Room::SetReady()
{
	SSynchronize Sync( this );

	m_roomState = ROOM_STATE_READY;
}

BOOL Room::CanInsert()
{
	//����°� normal�� �ƴϸ� ��� �� �� ����
	if( m_roomState != ROOM_STATE_NORMAL )
		return FALSE;

	//�ο��� �� á���� ��� �� �� ����
	if( m_nowPleyrCount >= MAX_PLAYER_COUNT )
		return FALSE;

	//�׷��� �ʴٸ� ��� �� �� ����
	return TRUE;
}

void Room::SetLeader( LobbyChar* sessionId )
{
	SSynchronize Sync( this );

	m_leader = sessionId;
}

LobbyChar* Room::ChangeLeader()
{
	SSynchronize Sync( this );

	m_leader = *(m_listPlayer.begin());

	return m_leader;
}

BOOL Room::SetPlayerSession( LobbySession* session, LobbyChar* charSpace )
{
	SSynchronize Sync( this );
	
	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		if( *iter == charSpace )
		{
			//�ش� session�� ã���� player�� session������ �ٲ��ش�
			(*iter)->SetSession( session );
			return TRUE;
		}
	}

	return FALSE;
}

void Room::AddPlayerInRoom( LobbyChar* charspace )
{
	SSynchronize Sync( this );

	//�켱 �� ������ �ϰ�
	charspace->SetTeam( GetTeam() );

	//list�� ����
	m_listPlayer.push_back( charspace );

	++m_nowPleyrCount;
}

BOOL Room::DelPlayerInRoom( LobbyChar* charspace )
{
	SSynchronize Sync( this );

	if( m_listPlayer.empty() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Room::DelPlayerInRoom()\nlist�� ��� �ֽ��ϴ�.����� ����...\n\n") );
		return FALSE;
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
		//��ġ�ϸ� ����� ������
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

BOOL Room::DelPlayerInRoom( int sessionId )
{
	SSynchronize Sync( this );

	if( m_listPlayer.empty() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Room::DelPlayerInRoom()\nlist�� ��� �ֽ��ϴ�.����� ����...\n\n") );
		return FALSE;
	}

	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		//��ġ�ϸ� ����� ������
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

void Room::ChangReadyCount( BOOL isReady )
{
	SSynchronize Sync( this );

	if( isReady )
		++m_readyCount;
	else
		--m_readyCount;

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("readyCount : %d"),
					m_readyCount );
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

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("Attect���� %d���� �ǰ� Defence���� %d���� �˴ϴ�.\n\n"),
					m_AttectTeam,
					m_DefenceTeam );

	//Ȥ�ö� ������̶� 0 �Ʒ��� �������� ������
	if( m_AttectTeam < 0 )
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("Room::ChangeTeam()\natt���� ���� 0���ϰ� �˴ϴ�.\n\n") );
	else if( m_DefenceTeam < 0 )
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("Room::ChangeTeam()\ndef���� ���� 0���ϰ� �˴ϴ�.\n\n") );

}

int Room::GetTeam()
{
	SSynchronize Sync( this );

	//�� ���� ���� �ο��̸� �켱������ �������� �����ȴ�.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_DefenceTeam���� %d���� �˴ϴ�"), m_DefenceTeam );
		return ROOM_TEAM_DEF;
	}
	else
	{
		++m_AttectTeam;
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_AttectTeam���� %d���� �˴ϴ�"), m_AttectTeam );
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
	//--------------------------------------
	// ����鿡�� ������ ������ ���� ����
	//--------------------------------------
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
	//--------------------------------------
	// ���Ӽ����� ������ ������ ���� ����
	//--------------------------------------
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
	m_document = &GetDocument;
}

RoomMgr::~RoomMgr()
{
	Release();
}

void RoomMgr::CreateRoomSpace()
{
	SSynchronize Sync( this );

	m_roomCount = m_document->RoomCount;
	m_iOpenRoomCount = 0;

	//�� ������ ����� ����
	for( int i=1; i<=m_roomCount; ++i )
	{
		Room* tmpRoom = new Room;
		tmpRoom->Init( i );

		m_mapRoom[i] = tmpRoom;
	}

	m_listOpenRoom.clear();

	m_roomIndexQ.Create( m_roomCount, 1 );
}

void RoomMgr::Release()
{
	SSynchronize Sync( this );
	
	m_roomIndexQ.Release();
	m_listOpenRoom.clear();

	for( int i=1; i<=m_roomCount; ++i )
	{
		delete m_mapRoom[i];
		m_mapRoom[i] = 0;
	}
	m_mapRoom.RemoveAll();
}

Room* RoomMgr::OpenRoom( TCHAR* title )
{
	SSynchronize Sync( this );

	int roomNum = m_roomIndexQ.GetIndex();
	if( roomNum < 0 )
		return NULL;

	//�̹� ���� �ִ� ������ �ѹ��� Ȯ���Ѵ�
	if( m_mapRoom[roomNum]->IsOpen() )
		return NULL;

	m_mapRoom[roomNum]->SetTitle( title );
	++m_iOpenRoomCount;

	return m_vecRoom[roomNum];
}

void RoomMgr::CloseRoom( int roomNum )
{
	SSynchronize Sync( this );

	if( roomNum <= 0 || roomNum >= m_roomCount )
		return;

	--m_iOpenRoomCount;
	//�� �ʱ�ȭ
	m_mapRoom[roomNum]->Init();

	//���ȣ�� ������ ���� ��ȯ���ش�
	m_roomIndexQ.PutIndex( roomNum );
}

Room* RoomMgr::FindRoom( int roomNum )
{
	SSynchronize Sync( this );

	if( roomNum <= 0 || roomNum >= m_roomCount )
		return NULL;

	return m_mapRoom[roomNum];
}

void RoomMgr::PackageOpenRoomInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	//���� �ִ� ���� �������� �ִ´�
	packet << m_iOpenRoomCount;

	std::list<Room*>::iterator iter;
	iter = m_listOpenRoom.begin();

	for( ; iter != m_listOpenRoom.end(); ++iter )
	{
		(*iter)->PackageRoomInfo( packet );
	}
}
