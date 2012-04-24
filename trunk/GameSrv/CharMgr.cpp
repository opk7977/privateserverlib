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
	m_iSessionId = 0;
	ZeroMemory( m_ID, 50 );
	m_iTeam = -1;
	m_State = 0;
	m_Position.Clear();
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

	//m_vecCharSpace.reserve( Character_Space );

	for( int i=0; i<Character_Space; ++i )
	{
		CharObj* tmpChar = new CharObj;
		tmpChar->Init();

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

int CharMgr::GetCharIndex()
{
	return m_IndexQ.GetIndex();
}

void CharMgr::ReturnIndex( int index )
{
	//0���� �۰� �ִ밪���� ũ�ų� ������ index����
	if( index < 0 || index >= Character_Space )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("CharMgr::ReturnIndex()\nIndex�� �̻��մϴ�.\n\n") );
		return;
	}

	m_IndexQ.PutIndex( index );
}

CharObj* CharMgr::FindChar( int index )
{
	//0���� �۰� �ʴ밪���� ũ�ų� ������ index����
	if( index < 0 || index >= Character_Space )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("CharMgr::FindChar()\nIndex�� �̻��մϴ�.\n\n") );
		return NULL;
	}

	return m_vecCharSpace[index];
}


