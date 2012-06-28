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
	m_playerCount = 0;

	_tcsncpy_s( m_tstrRoomTitle, 50, _T("Empty Room"), 10 );
	m_readyCount = 0;
	m_leader = NULL;
	m_roomState = ROOM_STATE_NORMAL;

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

	//모두(-방장) ready상태가 아니면 시작할 수 없음
	if( (m_listPlayer.GetItemCount()-1) > m_readyCount )
		return FALSE;

	//최소인원보다 적으면 시작할 수 없음
	if( m_listPlayer.GetItemCount() < MIN_PLAYER_COUNT )
		return FALSE;

	//팀인원이 0인경우도 시작 할 수 없음
// 	if( m_AttectTeam <= 0 || m_DefenceTeam <= 0 )
// 		return FALSE;

	return TRUE;
}

void Room::SetPlay()
{
	SSynchronize Sync( this );

	//방을 게임중으로 만들고
	m_roomState = ROOM_STATE_PLAYING;
	//레디상태 초기화
	m_readyCount = 0;

	//방의 모든 플레이어의 세션에 표시한다.
	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//캐릭터를 게임 중 상태로 만들어 준다
		(*iter)->SetIsPlay();
		//캐릭터의 상태는 모두 초기와
		(*iter)->SetReady( FALSE );
	}
}

void Room::ResetSession()
{
	SSynchronize Sync( this );

	//방의 모든 플레이어의 세션 정보를 초기화
	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		(*iter)->SetSession( NULL );
	}
}

void Room::SetReady()
{
	SSynchronize Sync( this );

	m_roomState = ROOM_STATE_READY;

	//우선 설정을 하는게 아니니까 넘어가는 인원을 받아 둔다
	m_playerCount = m_listPlayer.GetItemCount();
}

void Room::SetNormal()
{
	SSynchronize sync( this );

	m_roomState		= ROOM_STATE_NORMAL;
	m_playerCount	= 0;
}

BOOL Room::CanInsert()
{
	//방상태가 normal이 아니면 들어 올 수 없음
	if( m_roomState != ROOM_STATE_NORMAL )
		return FALSE;

	//인원이 꽉 찼으면 들어 올 수 없음
	if( m_listPlayer.GetItemCount() >= MAX_PLAYER_COUNT )
		return FALSE;

	//그렇지 않다면 들어 올 수 있음
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

void Room::AddPlayerInRoom( LobbyChar* charspace )
{
	SSynchronize Sync( this );

	//우선 팀 설정을 하고
	charspace->SetTeam( GetTeam() );

	//list에 저장
	m_listPlayer.AddItem( charspace );
}

BOOL Room::DelPlayerInRoom( LobbyChar* charspace )
{
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("Room::DelPlayerInRoom()\nlist가 비어 있습니다.지울수 엄써...\n\n") );
		return FALSE;
	}

	//팀 인원을 빼준다
	if( charspace->GetTeam() == 0 )
		--m_AttectTeam;
	else
		--m_DefenceTeam;

	//ready상태였으면 그것도 빼줘야 함
	if( charspace->GetReady() )
		--m_readyCount;

	m_listPlayer.DelItem( charspace );

	//캐릭터가 없으면 FALSE를 return해 주자
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("Room::DelPlayerInRoom()\nlist가 비어 있습니다.지울수 엄써...\n\n") );
		return FALSE;
	}

	LobbyChar* tmpChar = m_charMgr->FindCharAsSessionId( sessionId );

	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("Room::DelPlayerInRoom()\nsessionID %d번 캐릭터 정보가 존재 하지 않습니다..\n\n"), sessionId );
		return TRUE;
	}

	return DelPlayerInRoom( tmpChar );
}

BOOL Room::DelPlayerInRoomAtPlaying( int team )
{
	SSynchronize sync( this );

	//팀 인원을 빼준다
	if( team == 0 )
		--m_AttectTeam;
	else
		--m_DefenceTeam;

	if( --m_playerCount <= 0 )
		return FALSE;

	return TRUE;
}

void Room::ChangReadyCount( BOOL isReady )
{
	SSynchronize Sync( this );

	if( isReady )
		++m_readyCount;
	else
		--m_readyCount;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("readyCount : %d"),
					m_readyCount );
#endif
}

BOOL Room::ChangeTeam( BOOL isATT )
{
	SSynchronize Sync( this );

	//바꾸려는 팀의 인원이 다 찼으면 바꿀 수 없다.
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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("Attect팀이 %d명이 되고 Defence팀이 %d명이 됩니다.\n\n"),
					m_AttectTeam,
					m_DefenceTeam );
#endif

	//혹시라도 어느팀이라도 0 아래로 내려가면 오류다
	if( m_AttectTeam < 0 )
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("Room::ChangeTeam()\natt팀의 수가 0이하가 됩니다.\n\n") );
	else if( m_DefenceTeam < 0 )
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("Room::ChangeTeam()\ndef팀의 수가 0이하가 됩니다.\n\n") );

	return TRUE;
}

int Room::GetTeam()
{
	SSynchronize Sync( this );

	//두 팀이 같은 인원이면 우선권으로 공격팀에 배정된다.
	if( m_AttectTeam > m_DefenceTeam )
	{
		++m_DefenceTeam;
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_DefenceTeam팀이 %d명이 됩니다"), m_DefenceTeam );
#endif
		return ROOM_TEAM_DEF;
	}
	else
	{
		++m_AttectTeam;
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("m_AttectTeam팀이 %d명이 됩니다"), m_AttectTeam );
#endif
		return ROOM_TEAM_ATT;
	}
}

void Room::PackageRoomInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	packet << m_roomNum;
	//packet << m_nowPleyrCount;
	if( m_roomState == ROOM_STATE_NORMAL )
		packet << m_listPlayer.GetItemCount();
	else
		packet << m_playerCount;
	int size = _tcslen( m_tstrRoomTitle ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrRoomTitle, size );
	packet << m_roomState;
	//게임관련 모드
	packet << m_stageMap;
	packet << m_gameMode;
	packet << m_playTime;
	packet << m_playCount;
}

void Room::PackageRoomModeInfo( SPacket &packet )
{
	SSynchronize Sync( this );

	//게임관련 모드
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
		//나를 넘겼으면 나는 보내지 않아야 한다
		if( *iter == itMe )
			continue;

		//세젼 정보가 제대로 있지 않으면 넘어 간다
		if( (*iter)->GetSession() == NULL )
			continue;

		(*iter)->GetSession()->SendPacket( packet );
	}
}

void Room::PackagePlayerInRoom( SPacket &packet, LobbyChar* itme /*= NULL */ )
{
	//--------------------------------------
	// 사람들에게 정보를 보내기 위해 포장
	//--------------------------------------
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return;

	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//나를 넘겼으면 나는 보내지 않아야 한다
		if( *iter == itme )
			continue;

		(*iter)->PackageMyInfoForRoom( packet );
	}
}

void Room::PackagePlayerInRoomForGame( SPacket &packet, LobbyChar* itMe /*= NULL */ )
{
	//--------------------------------------
	// 게임서버에 정보를 보내기 위해 포장
	//--------------------------------------
	SSynchronize Sync( this );

	if( m_listPlayer.IsEmpty() )
		return;

	std::list<LobbyChar*>::iterator iter = m_listPlayer.GetHeader();

	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
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

	//방 공간을 만들어 놓자
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

	//이미 열려 있는 방인지 한번더 확인한다
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

	//열린 방 list에서는 지워준다
	m_listOpenRoom.DelItem( m_mapRoom[roomNum] );

	//방 초기화
	m_mapRoom[roomNum]->Init();

	//방번호는 재사용을 위해 반환해준다
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

	//열려 있는 방의 갯수부터 넣는다
	packet << m_listOpenRoom.GetItemCount();

	std::list<Room*>::iterator iter = m_listOpenRoom.GetHeader();
	for( ; !m_listOpenRoom.IsEnd( iter ); ++iter )
	{
		(*iter)->PackageRoomInfo( packet );
	}
}
