#include "Network.h"

#include "Character.h"
#include "StandState.h"
#include "MoveUState.h"
#include "MoveULState.h"
#include "MoveLState.h"
#include "MoveLBState.h"
#include "MoveBState.h"
#include "MoveBRState.h"
#include "MoveRState.h"
#include "MoveRUState.h"

CCharacter::CCharacter( CFiniteStateMachine* fsm ) : m_stateMachine(fsm)
{
	m_arrayAction[STATE_STAND]		= (CCharacterState*)new CStandState(this);
	
	m_arrayAction[STATE_MOVE_U]		= (CCharacterState*)new CMoveUState(this);
	m_arrayAction[STATE_MOVE_UL]	= (CCharacterState*)new CMoveULState(this);
	m_arrayAction[STATE_MOVE_L]		= (CCharacterState*)new CMoveLState(this);
	m_arrayAction[STATE_MOVE_LB]	= (CCharacterState*)new CMoveLBState(this);
	m_arrayAction[STATE_MOVE_B]		= (CCharacterState*)new CMoveBState(this);
	m_arrayAction[STATE_MOVE_BR]	= (CCharacterState*)new CMoveBRState(this);
	m_arrayAction[STATE_MOVE_R]		= (CCharacterState*)new CMoveRState(this);
	m_arrayAction[STATE_MOVE_RU]	= (CCharacterState*)new CMoveRUState(this);

	//기본 셋팅은 서있는 상태
	m_action = m_arrayAction[STATE_STAND];
	m_direction = DIRECTION_U;

	m_oldState = STATE_STAND;
}

CCharacter::~CCharacter(void)
{
	if( m_stateMachine != 0 )
		delete m_stateMachine;

	//할당한 상태 값들을 모두 해제 해 주고 지워 준다.
	for( int i=0; i<STATE_COUNT; ++i )
	{
		delete m_arrayAction[i];
		m_arrayAction[0] = 0;
	}
}

void CCharacter::Process()
{
	m_action->Process();
}

void CCharacter::SetIndexID( int id )
{
	m_charID = id;
}

int CCharacter::GetIndexID()
{
	return m_charID;
}

void CCharacter::SetPos( int x, int y, int z )
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

int CCharacter::GetX()
{
	return m_PosX;
}

int CCharacter::GetY()
{
	return m_PosY;
}

int CCharacter::GetZ()
{
	return m_PosZ;
}

void CCharacter::SetState( int state )
{
	m_action = m_arrayAction[(State)state];
}

void CCharacter::SetDir( int dir )
{
	m_direction = dir;
}

int CCharacter::GetDir()
{
	return m_direction;
}

int CCharacter::GetState()
{
	if( m_stateMachine == 0 )
		return 0;

	return m_stateMachine->GetCurrentStateId();
}

void CCharacter::SetTransition( DWORD _event )
{
	m_stateMachine->StateTransition( _event );
	int currentStateId = m_stateMachine->GetCurrentStateId();
 	m_action = m_arrayAction[currentStateId];

	if( m_oldState != currentStateId )
	{
		m_oldState = currentStateId;
		//GetLogger.PutLog( 0, _T("[Character] 캐릭터의 상태가 %d번 상태로 변함\n"), currentStateId );

		//가만이 슨경우는 방향을 바꾸지 않아도 됨
		if( m_oldState != STATE_STAND )
		{
			//아니면 방향이 변했음
			m_direction = m_oldState;			
		}

								//( sessionId, state,     dirX,                 dirY,     dirZ,     posX,         posY,           posZ)
		GetNetwork.SendChangeState( m_charID, m_oldState, (float)m_direction, (float)0, (float)0, (float)m_PosX, (float)m_PosY, (float)m_PosZ );
	}
}

void CCharacter::MoveTo( int x, int y, int z )
{
	//======================================
	// x값
	//======================================
	m_PosX += x;
	if( m_PosX < 0 )
		m_PosX = 0;
	else if( m_PosX > 800 )
		m_PosX = 800;

	//======================================
	// y값
	//======================================
	m_PosY += y;
	if( m_PosY < 0 )
		m_PosY = 0;
	else if( m_PosY > 600 )
		m_PosY = 600;
}

void CCharacter::Render( HDC* dc )
{
	SetBkMode( *dc, TRANSPARENT );
	switch( m_direction )
	{
	case DIRECTION_U:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("U"), 1 );
		break;
	case DIRECTION_UL:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("UL"), 2 );
		break;
	case DIRECTION_L:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("L"), 1 );
		break;
	case DIRECTION_LB:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("LB"), 2 );
		break;
	case DIRECTION_B:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("B"), 1 );
		break;
	case DIRECTION_BR:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("BR"), 2 );
		break;
	case DIRECTION_R:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("R"), 1 );
		break;
	case DIRECTION_RU:
		//TextOut( *dc, m_PosX-10, m_PosY-20, _T("RU"), 2 );
		break;
	}
	
	Rectangle( *dc, m_PosX-5, m_PosY-5, m_PosX+5, m_PosY+5 );
}
