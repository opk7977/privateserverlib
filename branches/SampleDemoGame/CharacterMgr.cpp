#include "StdAfx.h"
#include "CharacterMgr.h"


CCharacterMgr::CCharacterMgr(void) : m_iHeroID(0)
								   , m_iCount(0)
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

void CCharacterMgr::CreateCharacter( int id, int x, int y, TCHAR* str )
{
	CFiniteStateMachine* fsm;

	fsm = new CFiniteStateMachine();

	//======================================
	// 서 있는 상태에서
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
	// 위로 가고 있는 상태에서
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
	// 왼쪽위로 가고 있는 상태에서
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
	// 왼쪽으로 가고 있는 상태에서
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
	// 왼쪽아래로 가고 있는 상태에서
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
	// 아래로 가고 있는 상태에서
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
	// 오른쪽아래로 가고 있는 상태에서
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
	// 오른쪽으로 가고 있는 상태에서
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
	// 오른쪽위로 가고 있는 상태에서
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
	// 캐릭터 생성
	//--------------------------------------
	CCharacter* tmpChar = new CCharacter( fsm );

	tmpChar->SetPos( x, y );
	tmpChar->SetIndexID( id );
	tmpChar->SetStrID( str );

	StartCritical();
	
	//등록
	m_playerList[id] = tmpChar;

	//player의 수를 올려 준다.
	++m_iCount;

	EndCritical();
}

void CCharacterMgr::DeleteCharacter( int id )
{
	CCharacter* tmp;
	tmp = FindChar( id );

	if( tmp == NULL )
		return;

	std::map< DWORD, CCharacter* >::iterator iter;

	StartCritical();

	iter = m_playerList.find( tmp->GetIndexID() );

	delete iter->second;
	iter->second = 0;

	m_playerList.erase( iter );

	--m_iCount;

	EndCritical();
}

void CCharacterMgr::MoveCharacter()
{
	//player가 없으면 그냥 돌아 간다.
	if( m_iCount <= 0 )
		return;

	std::map< DWORD, CCharacter* >::iterator iter;

	StartCritical();

	iter = m_playerList.begin();
	for( ; iter != m_playerList.end(); iter++ )
	{
		(iter->second)->Process();
	}

	EndCritical();
}

void CCharacterMgr::RenderAll( CDC* dc )
{
	//player가 없으면 그냥 돌아 간다.
	if( m_iCount <= 0 )
		return;

	std::map< DWORD, CCharacter* >::iterator iter;

	StartCritical();

	iter = m_playerList.begin();
	for( ; iter != m_playerList.end(); iter++ )
	{
		(iter->second)->Render( dc );
	}

	EndCritical();
}

CCharacter* CCharacterMgr::FindChar( int _index )
{
	//player가 없으면 그냥 돌아 간다.
	if( m_iCount <= 0 )
		return NULL;

	//우선 찾아 봅시다
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
	//player가 없으면 그냥 돌아 간다.
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
	std::map< DWORD, CCharacter* >::iterator iter, iterPrev;

	StartCritical();

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

