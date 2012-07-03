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
	m_killCount			= m_deathCount			= 0;

	//--------------------------------------

	m_HP = 100;

	m_invincibleTime	= CHARACTER_INVINCIBLE_TIME;
	m_isInvincible		= FALSE;

	//--------------------------------------
	m_skillState		= SKILL_NONE;

	m_hidePoint			= CHARACTER_HIDE_POINT;
	m_hideOnTime		= CHARACTER_HIDEON_TIME;
	m_hideOffTime		= CHARACTER_HIDEOFF_TIME;

	m_scanPoint			= CHARACTER_SCAN_POINT;
	m_scanOnTime		= CHARACTER_SCANON_TIME;
	m_scanOffTime		= CHARACTER_SCANOFF_TIME;
}

void CharObj::Init( int index )
{
	m_vecIndex = index;

	Init();
}

void CharObj::CountDownInvincibleTime( float elaps )
{
	m_invincibleTime -= elaps;

	//�ð��� 0���� ������
	if( m_invincibleTime <= 0 )
	{
		//���� ���� ��
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
		//�׾���
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
	SSynchronize sync( this );

	//HP
	m_HP = 100;
	
	//���� ����
	m_isInvincible		= TRUE;
	m_invincibleTime	= CHARACTER_INVINCIBLE_TIME;

	//����/ ��ĵ ��ġ �ʱ�ȭ
	m_skillState		= SKILL_NONE;

	m_hidePoint			= CHARACTER_HIDE_POINT;
	m_hideOnTime		= CHARACTER_HIDEON_TIME;
	m_hideOffTime		= CHARACTER_HIDEOFF_TIME;

	m_scanPoint			= CHARACTER_SCAN_POINT;
	m_scanOnTime		= CHARACTER_SCANON_TIME;
	m_scanOffTime		= CHARACTER_SCANOFF_TIME;
}

BOOL CharObj::HPUpOnePoint()
{
	SSynchronize sync( this );

	//�����ִ� �׳� return
	if( IsDie() )
		return FALSE;

	//�̹� 100�̸� �׳� return
	if( m_HP >= 100 )
		return FALSE;

	//�÷� �ְ�
	++m_HP;
	return TRUE;
}

BOOL CharObj::CountSkillPoint( float elaps )
{
	SSynchronize sync( this );

	if( IsDie() )
		return FALSE;

	BOOL isChange = FALSE;

	//���¿� ���� ó����
	if( m_skillState == SKILL_HIDE )
	{
		//--------------------------------------
		// ���� ��
		//--------------------------------------
		//======================================
		// ���� ��ġ�� ���̰�
		//======================================
		m_hideOnTime -= elaps;
		//�� �پ����� Ȯ���Ѵ�
		if( m_hideOnTime <= 0 )
		{
			m_hideOnTime = CHARACTER_SCANON_TIME;
			if( --m_hidePoint <= 0 )
			{
				m_hidePoint = 0;
				//���� ����Ʈ�� 0�̸� �۰ų� ������ ��ĵ Ǯ����� ��
				//SetSkillNone();
			}
			isChange = TRUE;
		}

		//======================================
		// ��ĵ ��ġ�� ���� ��
		//======================================
		//��ĵ point�� Ǯ�� �ƴϸ� ��
		if( m_scanPoint < CHARACTER_SCAN_POINT )
		{
			//��ĵ ��� �ð��� ���̰�
			m_scanOffTime -= elaps;
			if( m_scanOffTime <= 0 )
			{
				m_scanOffTime = CHARACTER_SCANOFF_TIME;
				if( ++m_scanPoint > CHARACTER_SCAN_POINT )
					m_scanPoint = CHARACTER_SCAN_POINT;

				isChange = TRUE;
			}
		}
	}
	else if( m_skillState == SKILL_SCAN )
	{
		//--------------------------------------
		// ��ĵ ��
		//--------------------------------------
		//======================================
		// ��ĵ �ð��� ���̰�
		//======================================
		m_scanOnTime -= elaps;
		//�� �پ����� Ȯ��
		if( m_scanOnTime <= 0 )
		{
			m_scanOnTime = CHARACTER_SCANON_TIME;
			if( --m_scanPoint <= 0)
			{
				m_scanPoint = 0;
				//��ĵ ����Ʈ�� 0���� �۰ų� ������ ��ĵ Ǯ����� ��
				//SetSkillNone();
			}
			isChange = TRUE;
		}

		//======================================
		// ���� ��ġ�� ���� ��
		//======================================
		//����point�� Ǯ�� �ƴϸ� ��
		if( m_hidePoint < CHARACTER_HIDE_POINT )
		{
			//���� ��� �ð��� ���̰�
			m_hideOffTime -= elaps;
			if( m_hideOffTime <= 0 )
			{
				m_hideOffTime = CHARACTER_HIDEOFF_TIME;
				if( ++m_hidePoint > CHARACTER_HIDE_POINT )
					m_hidePoint = CHARACTER_HIDE_POINT;

				isChange = TRUE;
			}
		}
	}
	else
	{
		//--------------------------------------
		// ��ų ������� �ƴ�
		//--------------------------------------
		//����point�� Ǯ�� �ƴϸ� ��
		if( m_hidePoint < CHARACTER_HIDE_POINT )
		{
			//���� ��� �ð��� ���̰�
			m_hideOffTime -= elaps;
			if( m_hideOffTime <= 0 )
			{
				m_hideOffTime = CHARACTER_HIDEOFF_TIME;
				if( ++m_hidePoint > CHARACTER_HIDE_POINT )
					m_hidePoint = CHARACTER_HIDE_POINT;

				isChange = TRUE;
			}
		}

		//��ĵ point�� Ǯ�� �ƴϸ� ��
		if( m_scanPoint < CHARACTER_SCAN_POINT )
		{
			//��ĵ ��� �ð��� ���̰�
			m_scanOffTime -= elaps;
			if( m_scanOffTime <= 0 )
			{
				m_scanOffTime = CHARACTER_SCANOFF_TIME;
				if( ++m_scanPoint > CHARACTER_SCAN_POINT )
					m_scanPoint = CHARACTER_SCAN_POINT;

				isChange = TRUE;
			}
		}
	}

	return isChange;
}

void CharObj::SetSkillNone()
{
	SSynchronize sync( this );

	m_skillState		= SKILL_NONE;
	//�ð� �ʱ�ȭ
	m_hideOnTime		= CHARACTER_HIDEON_TIME;
	m_scanOnTime		= CHARACTER_SCANON_TIME;
}

void CharObj::SetSkillHide()
{
	SSynchronize sync( this );

	m_skillState		= SKILL_HIDE;
	//�ð� �ʱ�ȭ
	m_hideOffTime		= CHARACTER_HIDEOFF_TIME;
}

void CharObj::SetSkillScan()
{
	SSynchronize sync( this );

	m_skillState		= SKILL_SCAN;
	//�ð� �ʱ�ȭ
	m_scanOffTime		= CHARACTER_SCANOFF_TIME;
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

	//�޾ƿ� ������ ������ ������ return
	if( charspace == NULL )
		return;

	int index = charspace->GetVecIndex();
	charspace->Init();

	m_IndexQ.PutIndex( index );
}

CharObj* CharMgr::FindCharAsIndex( int index )
{
	SSynchronize sync( this );

	//0���� �۰� �ʴ밪���� ũ�ų� ������ index����
	if( index < 0 || index >= m_document->SessionCount )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("CharMgr::FindChar()\nIndex�� �̻��մϴ�.\n\n") );
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
		//sessionId�� ���� �ָ� ã������ return�� �ش�
		if( (*iter)->GetSessionID() == sessionID )
			return (*iter);
	}

	//������ NULL�� return
	return NULL;
}

