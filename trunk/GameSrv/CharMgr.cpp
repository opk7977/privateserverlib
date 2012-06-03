#include "CharMgr.h"
#include "SLogger.h"
#include "GameSession.h"

CharObj::CharObj()
{
	Init();
}

CharObj::~CharObj()
{
}

void CharObj::Init()
{
	m_sessionID = 0;
	ZeroMemory( m_tstrID, 50 );
	m_session			= NULL;
	m_iTeam				= -1;
	m_startPoint		= 0;
	m_killCount			= 0;
	m_deathCount		= 0;

	m_HP = 100;
}

void CharObj::Init( int index )
{
	m_vecIndex = index;

	Init();
}

// void CharObj::PackageMyInfo( SPacket& packet )
// {
// 	SSynchronize Sync( this );
// 
// 	packet << m_sessionID;				//세션ID
// 	//자신이 생성될 위치를 담는다
// 	packet << m_startPoint;
// 
// // 	packet << m_State;				//캐릭터 상태
// // 	packet << m_Position.m_X;		//위치값
// // 	packet << m_Position.m_Y;
// // 	packet << m_Position.m_Z;
// // 	packet << m_Direction.m_X;		//방향값
// // 	packet << m_Direction.m_Y;
// // 	packet << m_Direction.m_Z;
// // 	packet << m_DirInt;
// 
// 	//ip와 port번호를 넣는다
// 	m_session->PackageMyNetInfo( packet );
// }

void CharObj::DownHP( int damage )
{
	SSynchronize sync( this );

	m_HP -= damage;

	if( m_HP <= 0 )
		m_HP = 0;
}

BOOL CharObj::IsDie()
{
	SSynchronize sync( this );

	return ( m_HP <= 0 ) ? TRUE : FALSE;
}

BOOL CharObj::HPUpOnePoint()
{
	//이미 100이면 그냥 return
	if( m_HP >= 100 )
		return FALSE;

	//올려 주고
	++m_HP;
	return TRUE;
}

void CharObj::DeathCountUp()
{
	++m_deathCount;
}

int CharObj::GetDeathCount()
{
	return m_deathCount;
}

void CharObj::KillCountUp()
{
	++m_killCount;
}

int CharObj::GetKillCount()
{
	return m_killCount;
}

// void CharObj::PackageCharInfoForResult( SPacket& packet )
// {
// 	SSynchronize sync( this );
// 
// 	//packet << 
// }

//==============================================================

CharMgr::CharMgr(void)
{
}

CharMgr::~CharMgr(void)
{
	Release();
}

void CharMgr::Init()
{
	m_IndexQ.Create( Character_Space, 0 );

	m_vecCharSpace.reserve( Character_Space );

	for( int i=0; i<Character_Space; ++i )
	{
		CharObj* tmpChar = new CharObj;
		tmpChar->Init(i);

		m_vecCharSpace.push_back(tmpChar);
	}
}

void CharMgr::Release()
{
	if( m_vecCharSpace.empty() )
		return;

	for( int i=0; i<Character_Space; ++i )
	{
		delete m_vecCharSpace[i];
	}
	m_vecCharSpace.clear();
}

CharObj* CharMgr::GetCharSpace()
{
	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	return m_vecCharSpace[index];
}

void CharMgr::ReturnCharSpace( CharObj* charspace )
{
	//받아온 공간에 문제가 있으면 return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	m_IndexQ.PutIndex( index );
}

CharObj* CharMgr::FindCharAsIndex( int index )
{
	//0보다 작고 초대값보다 크거나 같으면 index에러
	if( index < 0 || index >= Character_Space )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("CharMgr::FindChar()\nIndex가 이상합니다.\n\n") );
		return NULL;
	}

	return m_vecCharSpace[index];
}

CharObj* CharMgr::FindCharAsSessionId( int sessionID )
{
	std::vector<CharObj*>::iterator iter;
	iter = m_vecCharSpace.begin();
	for( ; iter != m_vecCharSpace.end(); ++iter )
	{
		//sessionId와 같은 애를 찾았으면 return해 준다
		if( (*iter)->GetSessionID() == sessionID )
			return (*iter);
	}

	//없으면 NULL을 return
	return NULL;
}

