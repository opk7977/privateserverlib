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
	//모두 ready상태가 아니면 안됨
	//방장의 카운트는 빼야 한다
	if( (m_nowPleyrCount-1) != m_readyCount )
		return FALSE;

	//최소인원보다 적으면 안됨
	if( m_nowPleyrCount < MIN_PLAYER_COUNT )
		return FALSE;

	//팀의 비율이 다르면 안됨
	if( m_AttectTeam != m_DefenceTeam )
		return FALSE;

	return TRUE;
}

BOOL Room::SetPlay()
{
	SSynchronize Sync( this );

	//방을 게임중으로 만들고
	m_roomState = ROOM_STATE_PLAYING;

	//방의 모든 플레이어의 세션에 표시한다.
	{
		SSynchronize Sync( this );

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//캐릭터를 게임 중 상태로 만들어 준다
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
	//방상태가 괜찮으면 TRUE
	if( m_roomState == ROOM_STATE_NORMAL )
		return TRUE;

	//인원이 다 차지 않았으면 TRUE
	if( m_nowPleyrCount < MAX_PLAYER_COUNT )
		return TRUE;

	//그렇지 않다면 못 들어옴
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
				//해당 session을 찾아 바꿔준다
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

		//우선 팀 설정을 하고
		charspace->SetTeam( GetTeam() );

		//list에 저장
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
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::DelPlayerInRoom()\nlist가 비어 있습니다.지울수 엄써...\n\n") );
			return TRUE;
		}

		//팀 인원을 빼준다
		if( charspace->GetTeam() == 0 )
			--m_AttectTeam;
		else
			--m_DefenceTeam;

		//ready상태였으면 그것도 빼줘야 함
		if( charspace->GetReady() )
			--m_readyCount;

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//일치하면 나간다
			if( *iter == charspace )
			{
				m_listPlayer.erase( iter );
				--m_nowPleyrCount;
				break;
			}
		}
	
		//캐릭터가 없으면 FALSE를 return해 주자
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
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::DelPlayerInRoom()\nlist가 비어 있습니다.지울수 엄써...\n\n") );
			return TRUE;
		}

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//일치하면 나간다
			if( (*iter)->GetSessionID() == sessionId )
			{
				m_listPlayer.erase( iter );
				break;
			}
		}

		//캐릭터가 없으면 FALSE를 return해 주자
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

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_DefenceTeam팀이 %d명이 됩니다"), m_DefenceTeam );
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_AttectTeam팀이 %d명이 됩니다"), m_AttectTeam );

	//혹시라도 어느팀이라도 0 아래로 내려가면 오류다
	if( m_AttectTeam < 0 )
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeTeam()\natt팀의 수가 0이하가 됩니다.\n\n") );
	else if( m_DefenceTeam < 0 )
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeTeam()\ndef팀의 수가 0이하가 됩니다.\n\n") );

}

int Room::GetTeam()
{
	SSynchronize Sync( this );

	//두 팀이 같은 인원이면 우선권으로 공격팀에 배정된다.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_DefenceTeam팀이 %d명이 됩니다"), m_DefenceTeam );
		return ROOM_TEAM_DEF;
	}
	else
	{
		++m_AttectTeam;
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_AttectTeam팀이 %d명이 됩니다"), m_AttectTeam );
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
		//나를 넘겼으면 나는 보내지 않아야 한다
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
		//나를 넘겼으면 나는 보내지 않아야 한다
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
		//나를 넘겼으면 나는 보내지 않아야 한다
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
// 	//1부터 ROOMCOUNT까지의 방을 만들어 놓는다.
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
// 	//이미 생성되어 있으면 되돌아 간다
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
// 	//이미 생성되어 있으면 되돌아 간다
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
// 			packet << i;				//방번호
// 			m_mapRoomlist[i]->PackageRoomInfo( packet );
// 		}
// 	}
// }


void RoomMgr::CreateRoomSpace()
{
	SSynchronize Sync( m_critical );

	m_iOpenRoomCount = 0;

	//방 공간을 만들어 놓자
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

	//이미 열려 있는 방인지 한번더 확인한다
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
	//방번호는 재사용을 위해 반환해준다
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

	//열려 있는 방의 갯수부터 넣는다
	packet << m_iOpenRoomCount;

	std::list<Room*>::iterator iter;
	iter = m_listOpenRoom.begin();

	for( ; iter != m_listOpenRoom.end(); ++iter )
	{
		(*iter)->PackageRoomInfo( packet );
	}
}
