#include "LobbyChar.h"
#include "LobbySession.h"

#include "DataLeader.h"

LobbyChar::LobbyChar(void)
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

void LobbyChar::Init()
{
	SSynchronize Sync( this );

	m_session = NULL;
	m_isPlay = FALSE;
	m_sessionId = 0;
	ZeroMemory( m_tstrId, 30 );
	m_myTeam = -1;
	m_ready = FALSE;
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
	if( m_session->GetMyRoom() == NULL )
		packet << 0;
	else
		packet << m_session->GetMyRoom()->GetRoomNum();
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
	m_connectPlayer.clear();
}

LobbyChar* CharMgr::GetCharSPace()
{
	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	//접속 list에 추가 해 준다
	m_connectPlayer.push_back( index );

	return m_vecCharSpace[index];
}

void CharMgr::ReturnCharSpace( LobbyChar* charspace )
{
	//받아온 공간에 문제가 있으면 return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	//list에서 제거해 준다
	{
		SSynchronize Sync( this );

		std::list<int>::iterator iter = m_connectPlayer.begin();
		for( ; iter != m_connectPlayer.end(); ++iter )
		{
			if( (*iter) == index )
			{
				m_connectPlayer.erase( iter );
				break;
			}
		}
	}

	m_IndexQ.PutIndex( index );
}

void CharMgr::ReturnCharSpace( int sessionID )
{
	//받아온 session번호에 문제가 있으면 return
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
		//sessionid와 같은 애를 찾으면 바로 return
		if( (*iter)->GetSessionID() == session )
			return (*iter);
	}

	//없으면 NULL을 return
	return NULL;
}


