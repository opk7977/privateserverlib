#include "CharMgr.h"
#include "SLogger.h"

CharObj::CharObj()
{
	Init();
}

CharObj::~CharObj()
{
}

void CharObj::Init()
{
	GameObj::Init();

	ZeroMemory( m_tstrID, 50 );
	m_iTeam = -1;
	m_State = 0;
	m_Direction.Clear();
}

void CharObj::Init( int index )
{
	GameObj::Init();

	m_vecIndex = index;

	ZeroMemory( m_tstrID, 50 );
	m_iTeam = -1;
	m_State = 0;
	m_Direction.Clear();
}

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
		if( (*iter)->GetIndexId() == sessionID )
			return (*iter);
	}

	//없으면 NULL을 return
	return NULL;
}

