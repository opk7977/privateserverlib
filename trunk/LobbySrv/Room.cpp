#include "Room.h"
#include "LobbyChar.h"
#include "LobbySession.h"
#include "SLogger.h"
#include "SPacket.h"

#include "DataLeader.h"


//Room
SLogger* Room::m_logger		= &GetLogger;
CharMgr* Room::m_charMgr	= &GetCharMgr;

Room::Room(void)
{
	
}

Room::~Room(void)
{
}

void Room::Init()
{
	SSynchronize Sync( this );

	m_listPlayer.Clear();

	_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 );
	m_stageMap = 0;
	m_readyCount = 0;
	m_leader = NULL;
	m_roomState = 0;

	m_stageMap = 0;
	m_gameMode = 0;
	m_playTime = 300;
	m_playCount = 1;
	
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
	if( (m_listPlayer.GetItemCount()-1) != m_readyCount )
		return FALSE;

	//�ּ��ο����� ������ ������ �� ����
	if( m_listPlayer.GetItemCount() < MIN_PLAYER_COUNT )
		return FALSE;

	return TRUE;
}

void Room::SetPlay()
{
	SSynchronize Sync( this );

	//���� ���������� �����
	m_roomState = ROOM_STATE_PLAYING;

	//���� ��� �÷��̾��� ���ǿ� ǥ���Ѵ�.
	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
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
	if( m_listPlayer.GetItemCount() >= MAX_PLAYER_COUNT )
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

	m_leader = *(m_listPlayer.GetHeader());

	return m_leader;
}

// BOOL Room::SetPlayerSession( LobbySession* session, LobbyChar* charSpace )
// {
// 	SSynchronize Sync( this );
// 	
// // 	std::list<LobbyChar*>::iterator iter;
// // 	iter = m_listPlayer.begin();
// 
// 
// 	for( ; iter != m_listPlayer.end(); ++iter )
// 	{
// 		if( *iter == charSpace )
// 		{
// 			//�ش� session�� ã���� player�� session������ �ٲ��ش�
// 			(*iter)->SetSession( session );
// 			return TRUE;
// 		}
// 	}
// 
// 	return FALSE;
// }

void Room::AddPlayerInRoom( LobbyChar* charspace )
{
	SSynchronize Sync( this );

	//�켱 �� ������ �ϰ�
	charspace->SetTeam( GetTeam() );

// 	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 			_T("Room::AddPlayerInRoom()\n%s���� %e������ �߰��˴ϴ�.\n\n"), charspace->GetID(),  );

	//list�� ����
	m_listPlayer.AddItem( charspace );
}

BOOL Room::DelPlayerInRoom( LobbyChar* charspace )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
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

	m_listPlayer.DelItem( charspace );

	//ĳ���Ͱ� ������ FALSE�� return�� ����
	if( m_listPlayer.IsEmpty() )
		return FALSE;
	else
		return TRUE;
}

BOOL Room::DelPlayerInRoom( int sessionId )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("Room::DelPlayerInRoom()\nlist�� ��� �ֽ��ϴ�.����� ����...\n\n") );
		return FALSE;
	}

	LobbyChar* tmpChar = m_charMgr->FindCharAsSessionId( sessionId );

	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
				_T("Room::DelPlayerInRoom()\nsessionID %d�� ĳ���� ������ ���� ���� �ʽ��ϴ�..\n\n"), sessionId );
		return TRUE;
	}

	return DelPlayerInRoom( tmpChar );
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

BOOL Room::ChangeTeam( BOOL isATT )
{
	SSynchronize Sync( this );

	//�ٲٷ��� ���� �ο��� �� á���� �ٲ� �� ����.
	if( isATT == 0 )
	{
		if( m_AttectTeam >= MAX_TIME_COUNT )
			return FALSE;
	}
	else
	{
		if( m_DefenceTeam >= MAX_TIME_COUNT )
			return FALSE;
	}


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

	return TRUE;
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
	//packet << m_nowPleyrCount;
	packet << m_listPlayer.GetItemCount();
	int size = _tcslen( m_tstrRoomTitle ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrRoomTitle, size );
	packet << m_roomState;
	//���Ӱ��� ���
	packet << m_stageMap;
	packet << m_gameMode;
	packet << m_playTime;
	packet << m_playCount;
}

void Room::PackageRoomModeInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	//���Ӱ��� ���
	packet << m_gameMode;
	packet << m_playTime;
	packet << m_playCount;
}

void Room::SendPacketAllInRoom( SPacket &packet, LobbyChar* itMe /*= NULL */ )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return;

	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
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

	if( m_listPlayer.IsEmpty() )
		return;

	//�켱 �ο��� ��´�
	packet << m_listPlayer.GetItemCount();

	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
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

	if( m_listPlayer.IsEmpty() )
		return;

	//�켱 �ο��� ��´�.
	packet << m_listPlayer.GetItemCount();

	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
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

	//�� ������ ����� ����
	for( int i=1; i<=m_roomCount; ++i )
	{
		Room* tmpRoom = new Room;
		tmpRoom->Init( i );

		m_mapRoom[i] = tmpRoom;
	}

	m_listOpenRoom.Clear();

	m_roomIndexQ.Create( m_roomCount, 1 );
}

void RoomMgr::Release()
{
	SSynchronize Sync( this );
	
	m_roomIndexQ.Release();
	m_listOpenRoom.Clear();

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
	m_listOpenRoom.AddItem( m_mapRoom[roomNum] );

	return m_mapRoom[roomNum];
}

void RoomMgr::CloseRoom( int roomNum )
{
	SSynchronize Sync( this );

	if( roomNum <= 0 || roomNum > m_roomCount )
		return;

	//���� �� list������ �����ش�
	m_listOpenRoom.DelItem( m_mapRoom[roomNum] );

	//�� �ʱ�ȭ
	m_mapRoom[roomNum]->Init();

	//���ȣ�� ������ ���� ��ȯ���ش�
	m_roomIndexQ.PutIndex( roomNum );
}

Room* RoomMgr::FindRoom( int roomNum )
{
	SSynchronize Sync( this );

	if( roomNum <= 0 || roomNum > m_roomCount )
		return NULL;

	return m_mapRoom[roomNum];
}

void RoomMgr::PackageOpenRoomInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	//���� �ִ� ���� �������� �ִ´�
	packet << m_listOpenRoom.GetItemCount();

	std::list<Room*>::iterator iter = m_listOpenRoom.GetHeader();
	for( ; !m_listOpenRoom.IsEnd( iter ); ++iter )
	{
		(*iter)->PackageRoomInfo( packet );
	}
}
