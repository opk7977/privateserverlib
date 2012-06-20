#include "LobbyChar.h"
#include "LobbySession.h"
#include "Room.h"

#include "DataLeader.h"

LobbyChar::LobbyChar(void)
: m_myRoom(NULL)
{
}

LobbyChar::~LobbyChar(void)
{
}

void LobbyChar::SetSession( LobbySession* session )
{
	m_session = session;
}

LobbySession* LobbyChar::GetSession() const
{
	return m_session;
}

void LobbyChar::SetRoom( Room* room )
{
	m_myRoom = room;
}

Room* LobbyChar::GetRoom() const
{
	return m_myRoom;
}

void LobbyChar::Init()
{
	SSynchronize Sync( this );

	m_session	= NULL;
	m_myRoom	= NULL;
	m_isPlay	= FALSE;
	m_sessionId	= 0;
	ZeroMemory( m_tstrId, 30 );
	m_myTeam	= -1;
	m_ready		= FALSE;
}

void LobbyChar::Init( int i )
{
	m_vecIndex = i;

	Init();
}

void LobbyChar::SetIsPlay( BOOL isPlay /*= TRUE */ )
{
	m_isPlay = isPlay;
}

BOOL LobbyChar::GetIsPlay() const
{
	return m_isPlay;
}

void LobbyChar::SetSessionID( int session )
{
	m_sessionId = session;
}

int LobbyChar::GetSessionID() const
{
	return m_sessionId;
}

void LobbyChar::SetID( TCHAR* _id )
{
	_tcsncpy_s( m_tstrId, 30, _id, _tcslen( _id ) );
}

TCHAR* LobbyChar::GetID()
{
	return m_tstrId;
}

void LobbyChar::SetTeam( int Team )
{
	m_myTeam = Team;
}

int LobbyChar::GetTeam() const
{
	return m_myTeam;
}

void LobbyChar::SetReady( BOOL ready )
{
	m_ready = ready;
}

BOOL LobbyChar::GetReady() const
{
	return m_ready;
}

void LobbyChar::SetCharData( int rankID, int rankPoint, int AccumulKillCount, int AccumulDeathCount )
{
	SSynchronize sync( this );

	m_rankID			= rankID;
	m_rankPoint			= rankPoint;
	m_AccumulatedKill	= AccumulKillCount;
	m_AccumulatedDeath	= AccumulDeathCount;
}

int LobbyChar::GetRankID() const
{
	return m_rankID;
}

int LobbyChar::GetRankPoint() const
{
	return m_rankPoint;
}

int LobbyChar::GetAccumulatedKillCount() const
{
	return m_AccumulatedKill;
}

int LobbyChar::GetAccumulatedDeathCount() const
{
	return m_AccumulatedDeath;
}

void Room::SetNormal()
{
	SSynchronize sync( this );

	m_roomState		= ROOM_STATE_NORMAL;
	m_playerCount	= 0;
}

void LobbyChar::PackageMyInfo( SPacket& packet )
{
	SSynchronize Sync( this );

	packet << m_sessionId;
	int size = _tcslen( m_tstrId ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrId, size );
}

void LobbyChar::PackageMyInfoForLobby( SPacket& packet )
{
	SSynchronize Sync( this );

	packet << m_sessionId;
	int size = _tcslen( m_tstrId ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrId, size );
	//자신이 속해 있는 방
	if( m_myRoom == NULL )
		packet << 0;
	else
		packet << m_myRoom->GetRoomNum();
	packet << m_isPlay;

	packet << m_rankID;
	packet << m_rankPoint;
	packet << m_AccumulatedKill;
	packet << m_AccumulatedDeath;
}

void LobbyChar::PackageMyInfoForRoom( SPacket& packet )
{
	SSynchronize Sync( this );

	packet << m_sessionId;
	int size = _tcslen( m_tstrId ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrId, size );
	packet << m_myTeam;
	packet << m_ready;
	
	packet << m_rankID;
	packet << m_rankPoint;
	packet << m_AccumulatedKill;
	packet << m_AccumulatedDeath;
}

void LobbyChar::PackageMyInfoForGame( SPacket& packet )
{
	SSynchronize Sync( this );

	packet << m_sessionId;
	int size = _tcslen( m_tstrId ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrId, size );
	packet << m_myTeam;

	packet << m_rankID;
// 	packet << m_rankPoint;
// 	packet << m_AccumulatedKill;
// 	packet << m_AccumulatedDeath;
}

//==============================================================

CharMgr::CharMgr()
{
	m_document = &GetDocument;
}

CharMgr::~CharMgr()
{
	Release();
}

void CharMgr::Init()
{
	SSynchronize sync( this );

	m_IndexQ.Create( m_document->SessionCount, 0);

	m_vecCharSpace.reserve( m_document->SessionCount );

	for( int i=0; i<m_document->SessionCount; ++i )
	{
		LobbyChar* tmpChar = new LobbyChar;
		tmpChar->Init(i);

		m_vecCharSpace.push_back( tmpChar );
	}

	m_waitChar.RemoveAll();
}

void CharMgr::Release()
{
	SSynchronize sync( this );

	m_waitChar.RemoveAll();
	for( int i=0; i<m_document->SessionCount; ++i )
	{
		delete m_vecCharSpace[i];
	}
	m_vecCharSpace.clear();
}

LobbyChar* CharMgr::GetCharSPace()
{
	SSynchronize sync( this );

	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	//접속 list에 추가 해 준다
	m_listPlayer.AddItem( index );

	return m_vecCharSpace[index];
}

void CharMgr::ReturnCharSpace( LobbyChar* charspace )
{
	SSynchronize sync( this );

	//받아온 공간에 문제가 있으면 return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	//list에서 제거해 준다
	m_listPlayer.DelItem( index );

	m_IndexQ.PutIndex( index );
}

void CharMgr::AddWaitChar( int sessionId, LobbyChar* charspace )
{
	SSynchronize sync( this );

	if( sessionId <= 0 )
		return;

	m_waitChar[sessionId] = charspace;
}

void CharMgr::DelWaitChar( int sessionId )
{
	SSynchronize sync( this );

	if( sessionId <= 0 )
		return;

	m_waitChar.RemoveKey( sessionId );
}

LobbyChar* CharMgr::GetWaitCharInfo( int sessionID )
{
	SSynchronize sync( this );

	if( sessionID <= 0 )
		return NULL;

	LobbyChar* tmpChar = NULL;
	if( !m_waitChar.Lookup( sessionID, tmpChar ) )
		return NULL;

	return tmpChar;
}

void CharMgr::ReturnCharSpace( int sessionID )
{
	SSynchronize sync( this );

	//받아온 session번호에 문제가 있으면 return
	if( sessionID < 0 || sessionID >= 100 )
		return;

	LobbyChar* tmpChar = FindCharAsSessionId( sessionID );

	ReturnCharSpace( tmpChar );
}

LobbyChar* CharMgr::FindCharAsSessionId( int session )
{
	SSynchronize sync( this );

	std::vector<LobbyChar*>::iterator iter;
	iter = m_vecCharSpace.begin();
	for( ; iter != m_vecCharSpace.end(); ++iter )
	{
		//sessionid와 같은 애를 찾으면 바로 return
		if( (*iter)->GetSessionID() == session )
			return (*iter);
	}

	//없으면 NULL을 return
	return NULL;
}

void CharMgr::PackageAllCharacter( SPacket& packet )
{
	SSynchronize Sync( this );

	//우선 모든 인원을 넣는다.
	packet << m_listPlayer.GetItemCount();

	std::list<int>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//해당 캐릭터의 정보를 넣는다.
		m_vecCharSpace[(*iter)]->PackageMyInfoForLobby( packet );
	}
}


