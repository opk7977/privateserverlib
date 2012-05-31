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
	GameObj::Init();

	ZeroMemory( m_tstrID, 50 );
	m_session = NULL;
	m_iTeam = -1;
	m_State = 0;
	m_DirInt = 0;
	SetDirection( 0.f, 0.f, 1.f );

	m_HP = 100;
}

void CharObj::Init( int index )
{
	GameObj::Init();

	m_vecIndex = index;

	Init();
}

void CharObj::PackageMyInfo( SPacket& packet )
{
	SSynchronize Sync( this );

	packet << m_iId;				//����ID
	packet << m_State;				//ĳ���� ����
	packet << m_Position.m_X;		//��ġ��
	packet << m_Position.m_Y;
	packet << m_Position.m_Z;
	packet << m_Direction.m_X;		//���Ⱚ
	packet << m_Direction.m_Y;
	packet << m_Direction.m_Z;
	packet << m_DirInt;

	//ip�� port��ȣ�� �ִ´�
	m_session->PackageMyNetInfo( packet );
}

void CharObj::DownHP( int damage )
{
	m_HP -= damage;

	if( m_HP <= 0 )
		m_HP = 0;
}

BOOL CharObj::IsDie()
{
	return ( m_HP <= 0 ) ? TRUE : FALSE;
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
	//�޾ƿ� ������ ������ ������ return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	m_IndexQ.PutIndex( index );
}

CharObj* CharMgr::FindCharAsIndex( int index )
{
	//0���� �۰� �ʴ밪���� ũ�ų� ������ index����
	if( index < 0 || index >= Character_Space )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("CharMgr::FindChar()\nIndex�� �̻��մϴ�.\n\n") );
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
		//sessionId�� ���� �ָ� ã������ return�� �ش�
		if( (*iter)->GetIndexId() == sessionID )
			return (*iter);
	}

	//������ NULL�� return
	return NULL;
}

