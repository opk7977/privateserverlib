#include "StdAfx.h"
#include "RoomMgr.h"

#include "CharMgr.h"

#include "Lobby.h"

RoomObj::RoomObj()
{
	if( !m_playerListInRoom.empty() )
		m_playerListInRoom.clear();

	m_playerCount = 0;

	m_roomState = 0;
}

RoomObj::~RoomObj()
{
}

void RoomObj::Init()
{
	_tcsncpy_s( m_Title, 50, _T("empty room"), 10 );
	//방타이틀을 바꿔 준다.
	GetRoomMgr.m_pLobby->m_RoomTitle[m_roomNum-1].SetWindowText( m_Title );

	if( !m_playerListInRoom.empty() )
		m_playerListInRoom.clear();

	m_playerCount = 0;

	m_roomState = 0;

	//버튼 상태를 바꿔 준다
	GetRoomMgr.m_pLobby->m_btnNewRoom[m_roomNum-1].EnableWindow( TRUE );
	GetRoomMgr.m_pLobby->m_btnEnterRoom[m_roomNum-1].EnableWindow( FALSE );
}

void RoomObj::SetRoomTitle( TCHAR* title )
{
	_tcsncpy_s( m_Title, 50, title, _tcslen(title) );

	GetRoomMgr.m_pLobby->m_RoomTitle[m_roomNum-1].SetWindowText( m_Title );
}

void RoomObj::AddPlayer( Character* character )
{
	if( m_playerCount >= MAX_PLAYER_COUNT )
		return;

	m_playerListInRoom.push_back( character );
	++m_playerCount;

	TCHAR tmp[5];
	wsprintf( tmp, _T("%d"), m_playerCount ); 

	GetRoomMgr.m_pLobby->m_RoomPlayerCount[m_roomNum-1].SetWindowText( tmp );
}

void RoomObj::DelPlayer( Character* character )
{
	if( m_playerListInRoom.empty() )
		return;

	m_playerListInRoom.remove( character );
	--m_playerCount;

	TCHAR tmp[5];
	wsprintf( tmp, _T("%d"), m_playerCount ); 

	GetRoomMgr.m_pLobby->m_RoomPlayerCount[m_roomNum-1].SetWindowText( tmp );
}

void RoomObj::SetPlayerCount( int count )
{
	m_playerCount = count;
	TCHAR tmp[5];
	wsprintf( tmp, _T("%d"), m_playerCount ); 
	GetRoomMgr.m_pLobby->m_RoomPlayerCount[m_roomNum-1].SetWindowText( tmp );

	if( m_playerCount > 0 )
		GetRoomMgr.m_pLobby->m_btnEnterRoom[m_roomNum-1].EnableWindow( TRUE );
	else
		GetRoomMgr.m_pLobby->m_btnNewRoom[m_roomNum-1].EnableWindow( TRUE );
}

Character* RoomObj::FindChar( int sessionId )
{
	if( m_playerListInRoom.empty() )
		return NULL;

	std::list<Character*>::iterator iter;
	iter = m_playerListInRoom.begin();
	for( ; iter != m_playerListInRoom.end(); ++iter )
	{
		if( (*iter)->GetSessionID() == sessionId )
			return (*iter);
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////


RoomMgr::RoomMgr(void)
{
}

RoomMgr::~RoomMgr(void)
{
	Release();
}

void RoomMgr::CreateRoom()
{
	for( int i=1; i<=ROOMCOUNT; ++i )
	{
		RoomObj* tmpRoom = new RoomObj;
		tmpRoom->SetRoomNum( i );

		m_mapRoom[i] = tmpRoom;
	}
}

void RoomMgr::Release()
{
	for( int i=1; i<=ROOMCOUNT; ++i )
	{
		delete m_mapRoom[i];
		m_mapRoom[i] = 0;
	}
	m_mapRoom.clear();
}

void RoomMgr::ClearAllRoom()
{
	for( int i=1; i<=ROOMCOUNT; ++i )
	{
		m_mapRoom[i]->Init();

		GetRoomMgr.m_pLobby->m_btnNewRoom[i-1].EnableWindow( FALSE );
		GetRoomMgr.m_pLobby->m_btnEnterRoom[i-1].EnableWindow( FALSE );
	}
}

RoomObj* RoomMgr::FindRoom( int roomNo )
{
	if( m_mapRoom.find( roomNo ) == m_mapRoom.end() )
		return NULL;

	return m_mapRoom[roomNo];
}
