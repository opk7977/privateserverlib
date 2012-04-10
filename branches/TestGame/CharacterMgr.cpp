#include "CharacterMgr.h"


CCharacterMgr::CCharacterMgr(void) : m_iHeroID(0)
								   , m_iCount(0)
								   , m_iPlayerCountIN(0)
{
	InitializeCriticalSection( &m_crtChar );
}

CCharacterMgr::~CCharacterMgr(void)
{
	Release();

	DeleteCriticalSection( &m_crtChar );
}

void CCharacterMgr::StartCritical()
{
	EnterCriticalSection( &m_crtChar );
}

void CCharacterMgr::EndCritical()
{
	LeaveCriticalSection( &m_crtChar );
}

void CCharacterMgr::SetPlayerCount( int count )
{
	m_iPlayerCountIN = count;
}

int CCharacterMgr::GetPlayerCount()
{
	return m_iPlayerCountIN;
}

void CCharacterMgr::CreateCharacter( int id, int x, int y, int z, int dir, int state )
{
	//id�� Ȯ������
	CCharacter* tmpChar = FindChar( id );
	if( tmpChar != NULL )
		return;

	CFiniteStateMachine* fsm;

	fsm = new CFiniteStateMachine();

	//======================================
	// �� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_STAND, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// ���� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_U, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// �������� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_UL, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// �������� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_L, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// ���ʾƷ��� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_LB, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// �Ʒ��� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_B, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// �����ʾƷ��� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_BR, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// ���������� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_R, CCharacter::EVENT_RU_DOWN, CCharacter::STATE_MOVE_RU );

	//======================================
	// ���������� ���� �ִ� ���¿���
	//======================================
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_KYE_UP, CCharacter::STATE_STAND );

	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_U_DOWN, CCharacter::STATE_MOVE_U );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_UL_DOWN, CCharacter::STATE_MOVE_UL );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_L_DOWN, CCharacter::STATE_MOVE_L );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_LB_DOWN, CCharacter::STATE_MOVE_LB );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_B_DOWN, CCharacter::STATE_MOVE_B );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_BR_DOWN, CCharacter::STATE_MOVE_BR );
	fsm->AddStateTransition( CCharacter::STATE_MOVE_RU, CCharacter::EVENT_R_DOWN, CCharacter::STATE_MOVE_R );

	fsm->SetCurrentState( CCharacter::STATE_STAND );
	
	//--------------------------------------
	// ĳ���� ����
	//--------------------------------------
	tmpChar = new CCharacter( fsm );

	tmpChar->SetPos( x, y, z );
	tmpChar->SetIndexID( id );
	tmpChar->SetDir( dir );
	tmpChar->SetState( state );

	//StartCritical();
	
	//���
	m_playerList[id] = tmpChar;

	//player�� ���� �÷� �ش�.
	++m_iCount;

	//EndCritical();
}

int CCharacterMgr::GetNowPlayerCount()
{
	return m_iCount;
}

void CCharacterMgr::DeleteCharacter( int id )
{
	CCharacter* tmp;
	tmp = FindChar( id );

	if( tmp == NULL )
		return;

	StartCritical();

	std::map< DWORD, CCharacter* >::iterator iter;

	iter = m_playerList.find( tmp->GetIndexID() );

	delete iter->second;
	iter->second = 0;

	m_playerList.erase( iter );

	--m_iCount;

	EndCritical();
}

void CCharacterMgr::MoveCharacter()
{
	//player�� ������ �׳� ���� ����.
	if( m_iCount <= 0 )
		return;

	StartCritical();

	std::map< DWORD, CCharacter* >::iterator iter;

	iter = m_playerList.begin();
	for( ; iter != m_playerList.end(); iter++ )
	{
		(iter->second)->Process();
	}

	EndCritical();
}

void CCharacterMgr::RenderAll( HDC* dc )
{
	//player�� ������ �׳� ���� ����.
	if( m_iCount <= 0 )
		return;

	StartCritical();

	std::map< DWORD, CCharacter* >::iterator iter;

	iter = m_playerList.begin();
	for( ; iter != m_playerList.end(); iter++ )
	{
		(iter->second)->Render( dc );
	}

	EndCritical();
}

CCharacter* CCharacterMgr::FindChar( int _index )
{
	//player�� ������ �׳� ���� ����.
	if( m_iCount <= 0 )
		return NULL;

	//�켱 ã�� ���ô�
	std::map< DWORD, CCharacter* >::iterator iter;
	CCharacter* tmp;

	iter = m_playerList.find( _index );
	if( iter == m_playerList.end() )
		return 0;

	tmp = m_playerList[_index];

	return tmp;
}

CCharacter* CCharacterMgr::GetHero()
{
	//player�� ������ �׳� ���� ����.
	if( m_iCount <= 0 )
		return NULL;

	if( m_iHeroID <= 0 )
		return NULL;

	return m_playerList[m_iHeroID];
}

void CCharacterMgr::SetHeroId( int id )
{
	m_iHeroID = id;
}

int CCharacterMgr::GetHeroId()
{
	return m_iHeroID;
}

void CCharacterMgr::Release()
{
	StartCritical();

	std::map< DWORD, CCharacter* >::iterator iter, iterPrev;

	iter = m_playerList.begin();
	while( iter != m_playerList.end() )
	{
		iterPrev = iter++;
		delete iterPrev->second;
		m_playerList.erase( iterPrev );
	}

	EndCritical();

	m_playerList.clear();
}
