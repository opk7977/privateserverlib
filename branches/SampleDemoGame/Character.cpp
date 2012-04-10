#include "StdAfx.h"

#include "SLogger.h"
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

void CCharacter::SetStrID( TCHAR* id )
{
	if( _tcslen(id) > 50 )
		return;

	_tcsncpy_s( m_strID, id, _tcslen(id) );
}

TCHAR* CCharacter::GetStrID()
{
	return m_strID;
}

void CCharacter::SetX( int x )
{
	m_position.x = x;
}

void CCharacter::SetY( int y )
{
	m_position.y = y;
}

void CCharacter::SetPos( int x, int y )
{
	m_position.x = x;
	m_position.y = y;
}

void CCharacter::SetPos( POINT pos )
{
	m_position = pos;
}

int CCharacter::GetX()
{
	return m_position.x;
}

int CCharacter::GetY()
{
	return m_position.y;
}

POINT CCharacter::GetPos()
{
	return m_position;
}

void CCharacter::SetState( int state )
{
	m_action = m_arrayAction[(State)state];
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Character] 캐릭터의 상태가 %d번 상태로 변함\n"), currentStateId );
		GetNetwork.SendChangeState( m_charID, m_oldState, m_position.x, m_position.y );
	}
}

void CCharacter::MoveTo( int x, int y )
{
	//======================================
	// x값
	//======================================
	m_position.x += x;
	if( m_position.x < 0 )
		m_position.x = 0;
	else if( m_position.x > 800 )
		m_position.x = 800;

	//======================================
	// y값
	//======================================
	m_position.y += y;
	if( m_position.y < 0 )
		m_position.y = 0;
	else if( m_position.y > 600 )
		m_position.y = 600;
}

void CCharacter::Render( CDC* dc )
{
	SetBkMode( *dc, TRANSPARENT );
	dc->TextOut( m_position.x-10, m_position.y-20, m_strID, _tcslen(m_strID) );
	dc->Rectangle( m_position.x-5, m_position.y-5, m_position.x+5, m_position.y+5 );
}
