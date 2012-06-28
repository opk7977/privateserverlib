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
	m_rankID			= 0;
	m_invincibleTime	= CHARACTER_INVINCIBLE_TIME;
	m_isInvincible		= FALSE;
	m_killCount			= m_deathCount			= 0;

	m_HP = 100;

	m_skillState		= SKILL_NONE;
	m_hideTime			= CHARACTER_HIDE_TIME;
	m_scanTime			= CHARACTER_SCAN_TIME;
}

void CharObj::Init( int index )
{
	m_vecIndex = index;

	Init();
}

void CharObj::CountDownInvincibleTime( float elaps )
{
	m_invincibleTime -= elaps;

	//시간이 0보다 작으면
	if( m_invincibleTime <= 0 )
	{
		//이제 무적 끗
		m_isInvincible		= FALSE;
		m_invincibleTime	= CHARACTER_INVINCIBLE_TIME;
	}
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

void CharObj::SetAlive()
{
	//HP
	m_HP = 100;
	
	//무적 설정
	m_isInvincible		= TRUE;
	m_invincibleTime	= CHARACTER_INVINCIBLE_TIME;

	//은신/ 스캔 수치 초기화
	m_hideTime			= CHARACTER_HIDE_TIME;
	m_scanTime			= CHARACTER_SCAN_TIME;
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

BOOL CharObj::HideUpDownOnePoint()
{
	SSynchronize sync( this );

	//죽은애는 그냥 return
	if( IsDie() )
		return TRUE;

	if( m_skillState != SKILL_HIDE )
	{
		//현재 은신 사용중이 아니면 증가
		//이미 10이면 그냥 return
		if( m_hideTime >= CHARACTER_HIDE_TIME )
			return TRUE;

		//올려 주고
		++m_hideTime;
	}
	else
	{
		//현재 은신 사용중
		//이미 0이면 줄일게 없음
		if( --m_hideTime <= 0 )
		{
			m_hideTime		= 0;
			m_skillState	= SKILL_NONE;
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CharObj::ScanUpDownOnePoint()
{
	SSynchronize sync( this );

	//죽은애는 그냥 return
	if( IsDie() )
		return TRUE;

	if( m_skillState != SKILL_SCAN )
	{
		//현재 은신 사용중이 아니면 증가
		//이미 10이면 그냥 return
		if( m_scanTime >= CHARACTER_SCAN_TIME )
			return TRUE;

		//올려 주고
		++m_scanTime;
	}
	else
	{
		//현재 은신 사용중
		//이미 0이면 줄일게 없음
		if( --m_scanTime <= 0 )
		{
			m_scanTime		= 0;
			m_skillState	= SKILL_NONE;
			return FALSE;
		}
	}

	return TRUE;
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

void CharObj::PackageMyInfoForDB( SPacket& packet )
{
	SSynchronize sync( this );

	packet << m_sessionID;
	packet << m_rankPoint;
	packet << m_killCount;
	packet << m_deathCount;
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

