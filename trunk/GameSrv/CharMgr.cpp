#include "CharMgr.h"
#include "DataLeader.h"
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
	m_rankID			= m_untilRankPoint		= 0;
	m_accumulKillCount	= m_accumulDeathCount	= 0;
	m_killCount			= m_deathCount			= 0;

	m_HP = 100;
}

void CharObj::Init( int index )
{
	m_vecIndex = index;

	Init();
}

void CharObj::DownHP( int damage )
{
	SSynchronize sync( this );

	m_HP -= damage;

	if( m_HP <= 0 )
	{
		//죽었음
		++m_deathCount;
		m_HP = 0;
	}
}

BOOL CharObj::IsDie()
{
	SSynchronize sync( this );

	return ( m_HP <= 0 ) ? TRUE : FALSE;
}

BOOL CharObj::HPUpOnePoint()
{
	SSynchronize sync( this );

	//죽은애는 그냥 return
	if( IsDie() )
		return FALSE;

	//이미 100이면 그냥 return
	if( m_HP >= 100 )
		return FALSE;

	//올려 주고
	++m_HP;
	return TRUE;
}

// void CharObj::DeathCountUp()
// {
// 	++m_deathCount;
// }

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
	m_document	= &GetDocument;
	m_logger	= &GetLogger;
}

CharMgr::~CharMgr(void)
{
	Release();
}

void CharMgr::Init()
{
	SSynchronize sync( this );

	m_IndexQ.Create( m_document->SessionCount, 0 );

	m_vecCharSpace.reserve( m_document->SessionCount );

	for( int i=0; i<m_document->SessionCount; ++i )
	{
		CharObj* tmpChar = new CharObj;
		tmpChar->Init(i);

		m_vecCharSpace.push_back(tmpChar);
	}
}

void CharMgr::Release()
{
	SSynchronize sync( this );

	if( m_vecCharSpace.empty() )
		return;

	for( int i=0; i<m_document->SessionCount; ++i )
	{
		delete m_vecCharSpace[i];
	}
	m_vecCharSpace.clear();
}

CharObj* CharMgr::GetCharSpace()
{
	SSynchronize sync( this );

	int index = m_IndexQ.GetIndex();
	if( index < 0 )
		return NULL;

	return m_vecCharSpace[index];
}

void CharMgr::ReturnCharSpace( CharObj* charspace )
{
	SSynchronize sync( this );

	//받아온 공간에 문제가 있으면 return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	m_IndexQ.PutIndex( index );
}

CharObj* CharMgr::FindCharAsIndex( int index )
{
	SSynchronize sync( this );

	//0보다 작고 초대값보다 크거나 같으면 index에러
	if( index < 0 || index >= m_document->SessionCount )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("CharMgr::FindChar()\nIndex가 이상합니다.\n\n") );
		return NULL;
	}

	return m_vecCharSpace[index];
}

CharObj* CharMgr::FindCharAsSessionId( int sessionID )
{
	SSynchronize sync( this );

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

