#include "Character.h"

#include "StandState.h"
#include "Walkstate.h"

TCHAR* Character::message[] = { _T("stand"), _T("move") };

Character::Character( FiniteStateMachine* fsm, int speed ) : m_stateMachine(fsm)
														   , m_speed( speed )
{
	//배열에 상태 클래스를 하나씩 넣어 준다
	m_arrayAction[STATE_STAND]		= (CharacterState*)new StandState(this);
	m_arrayAction[STATE_MOVE]		= (CharacterState*)new Walkstate(this);

	//기본 상태 셋팅
	m_action = m_arrayAction[STATE_STAND];
}

Character::~Character(void)
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

void Character::Process()
{
	//현재 설정되어 있는 상태의 process를 동작해 준다.
	m_action->Process();
}

void Character::SetID( int x )
{
	m_charID = x;
}

int Character::GetID()
{
	return m_charID;
}

void Character::SetPosition( int x, int y )
{
	m_position.x = x;
	m_position.y = y;
}

void Character::SetPosition( POINT pos )
{
	m_position = pos;
}

int Character::GetPosX()
{
	return m_position.x;
}

int Character::GetPosY()
{
	return m_position.y;
}

POINT Character::GetPosition()
{
	return m_position;
}
void Character::SetSpeed( int s )
{
	m_speed = s;
}

int Character::GetSpeed()
{
	return m_speed;
}

void Character::SetGoal( int x, int y )
{
	m_goalPos.x = x;
	m_goalPos.y = y;
}

void Character::SetGoal( POINT pos )
{
	m_goalPos = pos;
}

int Character::GetGoalX()
{
	return m_goalPos.x;
}

int Character::GetGoalY()
{
	return m_goalPos.y;
}

POINT Character::GetGoal()
{
	return m_goalPos;
}

void Character::SetState( int state )
{
	m_action = m_arrayAction[(State)state];
}

int Character::GetState()
{
	if( m_stateMachine == 0 )
		return 0;

	return m_stateMachine->GetCurrentStateID();
}

void Character::SetTransition( DWORD _event )
{
	m_stateMachine->StateTransition( _event );
	m_action = m_arrayAction[m_stateMachine->GetCurrentStateID()];
}

void Character::MoveTo()
{
	if( m_position.x < m_goalPos.x ) m_position.x += m_speed;
	if( m_position.x > m_goalPos.x ) m_position.x -= m_speed;
	if( m_position.y < m_goalPos.y ) m_position.y += m_speed;
	if( m_position.y > m_goalPos.y ) m_position.y -= m_speed;
}

void Character::Render( HDC dc )
{
	//사각형
	Rectangle( dc,
			   m_position.x-5, m_position.y-5,
			   m_position.x+5, m_position.y+5 );

	TCHAR tmp[256];
	wsprintf( tmp, _T("%d: %s"), m_charID, message[m_stateMachine->GetCurrentStateID()] );
	//상태
	TextOut( dc,
			 m_position.x, m_position.y,
			 tmp,
			 wcslen(tmp) );
}

BOOL Character::IsGoal()
{
	if( m_goalPos.x-5 > m_position.x )
		return FALSE;
	else if( m_goalPos.y-5 > m_position.y )
		return FALSE;
	else if( m_goalPos.x+5 < m_position.x )
		return FALSE;
	else if( m_goalPos.y+5 < m_position.y )
		return FALSE;
	else
		return TRUE;
}

