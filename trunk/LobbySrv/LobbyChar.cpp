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

void Room::SetNormal()
{
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
	//�ڽ��� ���� �ִ� ��
	if( m_myRoom == NULL )
		packet << 0;
	else
		packet << m_myRoom->GetRoomNum();
	packet << m_isPlay;
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
}

void LobbyChar::PackageMyInfoForGame( SPacket& packet )
{
	SSynchronize Sync( this );

	packet << m_sessionId;
	int size = _tcslen( m_tstrId ) * sizeof( TCHAR );
	packet << size;
	packet.PutData( m_tstrId, size );
	packet << m_myTeam;
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
	m_IndexQ.Create( m_document->SessionCount, 0);

	m_vecCharSpace.reserve( m_document->SessionCount );

	for( int i=0; i<m_document->SessionCount; ++i )
	{
		LobbyChar* tmpChar = new LobbyChar;
		tmpChar->Init(i);

		m_vecCharSpace.push_back( tmpChar );
	}
}

void CharMgr::Release()
{
	for( int i=0; i<m_document->SessionCount; ++i )
	{
		delete m_vecCharSpace[i];
	}
	m_vecCharSpace.clear();
}

LobbyChar* CharMgr::GetCharSPace()
{
	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	//���� list�� �߰� �� �ش�
	m_listPlayer.AddItem( index );

	return m_vecCharSpace[index];
}

void CharMgr::ReturnCharSpace( LobbyChar* charspace )
{
	//�޾ƿ� ������ ������ ������ return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	//list���� ������ �ش�
	m_listPlayer.DelItem( index );

	m_IndexQ.PutIndex( index );
}

void CharMgr::ReturnCharSpace( int sessionID )
{
	//�޾ƿ� session��ȣ�� ������ ������ return
	if( sessionID < 0 || sessionID >= 100 )
		return;

	LobbyChar* tmpChar = FindCharAsSessionId( sessionID );

	ReturnCharSpace( tmpChar );
}

LobbyChar* CharMgr::FindCharAsSessionId( int session )
{
	std::vector<LobbyChar*>::iterator iter;
	iter = m_vecCharSpace.begin();
	for( ; iter != m_vecCharSpace.end(); ++iter )
	{
		//sessionid�� ���� �ָ� ã���� �ٷ� return
		if( (*iter)->GetSessionID() == session )
			return (*iter);
	}

	//������ NULL�� return
	return NULL;
}

void CharMgr::PackageAllCharacter( SPacket& packet )
{
	SSynchronize Sync( this );

	//�켱 ��� �ο��� �ִ´�.
	packet << m_listPlayer.GetItemCount();

	std::list<int>::iterator iter = m_listPlayer.GetHeader();
	for( ; !m_listPlayer.IsEnd( iter ); ++iter )
	{
		//�ش� ĳ������ ������ �ִ´�.
		m_vecCharSpace[(*iter)]->PackageMyInfoForLobby( packet );
	}
}


