#include "Room.h"
#include "LobbyChar.h"
#include "LobbySession.h"
//#include "SSessionMgr.h"
#include "SLogger.h"
#include "SPacket.h"

// RoomChar::RoomChar()
// {
// 	Init();
// }
// 
// RoomChar::RoomChar( const RoomChar& p )
// {
// 	m_iocpKey = p.m_iocpKey;
// 	m_charTeam = p.m_charTeam;
// 	m_ready = p.m_ready;
// 	_tcsncpy_s( m_charID, 50, p.m_charID, _tcslen(p.m_charID) );
// }
// 
// RoomChar::~RoomChar()
// {
// }
// 
// void RoomChar::Init()
// {
// 	m_iocpKey = 0;
// 	ZeroMemory( m_charID, 50 );
// 	m_charTeam = m_ready = -1;
// }
// 
// void RoomChar::SetIocp( int key )
// {
// 	m_iocpKey = key;
// }
// 
// int RoomChar::GetIocp()
// {
// 	return m_iocpKey;
// }
// 
// void RoomChar::SetID( TCHAR *uId )
// {
// 	_tcsncpy_s( m_charID, 50, uId, _tcslen(uId) );
// }
// 
// TCHAR* RoomChar::GetID()
// {
// 	return m_charID;
// }
// 
// void RoomChar::SetTeam( int team )
// {
// 	m_charTeam = team;
// }
// 
// int RoomChar::GetTeam()
// {
// 	return m_charTeam;
// }
// 
// void RoomChar::SetReady( int ready )
// {
// 	m_ready = ready;
// }
// 
// int RoomChar::GetReady()
// {
// 	return m_ready;
// }
// 


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

		m_isPlay = FALSE;

		_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 ); 
		
		m_AttectTeam = m_DefenceTeam = 0;

		m_visible = FALSE;
	}
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
	if( m_AttectTeam != m_DefenceTeam )
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

// void Room::SetPlayerIocp( int sessionId, int iocpKey )
// {
// 	{
// 		SSynchronize Sync( this );
// 
// 		//존재하지 않으면 돌아 간다.
// 		if( m_mapPlayer.find(sessionId) == m_mapPlayer.end() )
// 			return;
// 
// 		m_mapPlayer[sessionId].SetIocp( iocpKey );
// 	}
// }

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
				*iter->SetSession( session );
				return TRUE;
			}
		}
	}

	return FALSE;
}

// int Room::AddPlayerInRoom( int sessionId, int iocpKey, TCHAR* charID )
// {
// 	RoomChar tmpChar;
// 
// 	{
// 		SSynchronize Sync( this );
// 
// 		//존재하면 돌아 간다.
// 		if( m_mapPlayer.find(sessionId) != m_mapPlayer.end() )
// 			return -1;
// 
// 		//RoomChar tmpChar;
// 		tmpChar.SetIocp( iocpKey );
// 		tmpChar.SetID( charID );
// 		tmpChar.SetTeam( GetTeam() );
// 		tmpChar.SetReady(ROOM_READY_NON);
// 
// 		m_mapPlayer[sessionId] = tmpChar;
// 
// 		++m_nowPleyrCount;
// 	}
// 
// 	return tmpChar.GetTeam();
// }

void Room::AddPlayerInRoom( LobbyChar* charspace )
{
	{
		SSynchronize Sync( this );

		m_listPlayer.push_back( charspace );
	}
}

// BOOL Room::DelPlayerInRoom( int sessionId )
// {
// 	SSynchronize Sync( this );
// 
// 	//존재하지 않으면 돌아 간다.
// 	std::map<int, RoomChar>::iterator iter;
// 	iter = m_mapPlayer.find(sessionId);
// 
// 	if( iter == m_mapPlayer.end() )
// 		return TRUE;
// 
// 	//team에 대한 count를 줄어 준다
// 	if( iter->second.GetTeam() == ROOM_TEAM_ATT )
// 		--m_AttectTeam;
// 	else
// 		--m_DefenceTeam;
// 
// 	m_mapPlayer.erase( sessionId );
// 
// 	if( --m_nowPleyrCount <= 0 )
// 		return FALSE;
// 
// 	return TRUE;
// }

BOOL Room::DelPlayerInRoom( LobbyChar* charspace )
{
	{
		SSynchronize Sync( this );

		std::list<LobbyChar*>::iterator iter;
		iter = m_listPlayer.begin();

		for( ; iter != m_listPlayer.end(); ++iter )
		{
			//일치하면 나간다
			if( *iter == charspace )
			{
				m_listPlayer.erase( iter );
				return TRUE;
			}
		}
	}

	//캐릭터가 없음
	return FALSE;

}

// int Room::ChangeReadyState( int sessionId )
// {
// 	SSynchronize Sync( this );
// 
// 	if( m_mapPlayer.find( sessionId ) == m_mapPlayer.end() )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeReadyState()\n캐릭터 정보가 없습니다.\n\n") );
// 		return -1;
// 	}
// 
// 	if( m_mapPlayer[sessionId].GetReady() == ROOM_READY_NON )
// 	{
// 		m_mapPlayer[sessionId].SetReady( ROOM_READY_OK );
// 
// 		if( ++m_readyCount > MAX_PLAYER_COUNT )
// 			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeReadyState()\n레디 인원이 최대 인원은 넘었습니다.\n\n") );
// 	}
// 	else
// 	{
// 		m_mapPlayer[sessionId].SetReady( ROOM_READY_NON );
// 
// 		if( --m_readyCount < 0 )
// 			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeReadyState()\n레디 인원이 0보다 적습니다.\n\n") );
// 	}
// 
// 	return m_mapPlayer[sessionId].GetReady();
// }

void Room::ChangReadyState( BOOL isReady )
{
	SSynchronize Sync( this );

	if( isReady )
		++m_readyCount;
	else
		--m_readyCount;
}

// int Room::TeamChange( int sessionID )
// {
// 	SSynchronize Sync( this );
// 
// 	//존재하지 않으면 돌아 간다.
// 	if( m_mapPlayer.find(sessionID) == m_mapPlayer.end() )
// 		return -1;
// 
// 	if( m_mapPlayer[sessionID].GetTeam() == ROOM_TEAM_ATT )
// 	{
// 		m_mapPlayer[sessionID].SetTeam( ROOM_TEAM_DEF );
// 		--m_AttectTeam;
// 		++m_DefenceTeam;
// 		return ROOM_TEAM_DEF;
// 	}
// 	else
// 	{
// 		m_mapPlayer[sessionID].SetTeam( ROOM_TEAM_ATT );
// 		++m_AttectTeam;
// 		--m_DefenceTeam;
// 		return ROOM_TEAM_ATT;
// 	}
// }

void Room::ChangeTeam( BOOL isATT )
{
	SSynchronize Sync( this );

	if( isATT )
	{
		++m_AttectTeam;
		--m_DefenceTeam;
	}
	else
	{
		--m_AttectTeam;
		++m_DefenceTeam;
	}

	//혹시라도 어느팀이라도 0 아래로 내려가면 오류다
	if( m_AttectTeam < 0 )
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeTeam()\natt팀의 수가 0이하가 됩니다.\n\n") );
	else if( m_DefenceTeam < 0 )
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("Room::ChangeTeam()\ndef팀의 수가 0이하가 됩니다.\n\n") );

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

// void Room::PackageAllPlayerInRoom( SPacket &packet )
// {
// 	SSynchronize Sync( this );
// 
// 	//우선 인원수를 담고
// 	packet << m_nowPleyrCount;
// 
// 	LobbySession* tmpSession;
// 
// 	std::map<int, RoomChar>::iterator iter;
// 	iter = m_mapPlayer.begin();
// 	for( ; iter != m_mapPlayer.end(); ++iter )
// 	{
// 		tmpSession = (LobbySession*)GetSessionMgr.GetSession( (iter->second).GetIocp() );
// 		if( tmpSession == NULL )
// 			continue;
// 
// 		tmpSession->PackageMyInfo( packet );
// 
// 		packet << (iter->second).GetTeam();
// 		packet << (iter->second).GetReady();
// 	}
// }

// void Room::SendPacketAllInRoom( SPacket &packet, LobbySession* mySession /*= NULL*/ )
// {
// 	SSynchronize Sync( this );
// 
// 	LobbySession* tmpSession;
// 	std::map<int, RoomChar>::iterator iter;
// 	iter = m_mapPlayer.begin();
// 	for( ; iter != m_mapPlayer.end(); ++iter )
// 	{
// 		tmpSession = (LobbySession*)GetSessionMgr.GetSession( (iter->second).GetIocp() );
// 
// 		if( tmpSession == NULL )
// 			continue;
// 
// 		if( tmpSession == mySession )
// 			continue;
// 
// 		tmpSession->SendPacket( packet );
// 	}
// }

void Room::SendPacketAllInRoom( SPacket &packet, LobbyChar* itMe /*= NULL */ )
{
	SSynchronize Sync( this );

	std::list<LobbyChar*>::iterator iter;
	iter = m_listPlayer.begin();

	for( ; iter != m_listPlayer.end(); ++iter )
	{
		//나를 넘겼으면 나는 보내지 않아야 한다
		if( *iter == itMe )
			continue;

		iter->GetSession()->SendPacket( packet );
	}
}

int Room::GetTeam()
{
	SSynchronize Sync( this );

	//두 팀이 같은 인원이면 우선권으로 공격팀에 배정된다.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
		return ROOM_TEAM_DEF;
	}
	else
	{
		++m_AttectTeam;
		return ROOM_TEAM_ATT;
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

Room* RoomMgr::OpenRoom( int roomNum, TCHAR* title )
{
	SSynchronize Sync( m_critical );

	Room* tmpRoom = FindRoom( roomNum );

	if( tmpRoom == NULL )
		return NULL;

	//이미 생성되어 있으면 되돌아 간다
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


