#ifndef __CHARACTER__
#define __CHARACTER__

#include "FiniteStateMachine.h"
#include "CharacterState.h"

//TCHAR *message[] = { _T("stand"), _T("move") };

class Character
{
public:
	//character의 상태값을 정의
	enum State
	{
		STATE_STAND = 0,	//서 있는 상태, 기본
		STATE_MOVE,			//움직임,
		STATE_COUNT,
	};

	//character의 행동값을 정의
	enum Event
	{
		EVENT_STOPWALK,		//움직임을 멈춤
		EVENT_STARTWALK,	//움직임
	};

private:
	CharacterState*			m_action;
	CharacterState*			m_arrayAction[STATE_COUNT];
	FiniteStateMachine*		m_stateMachine;

//protected:
	int		m_charID;
	POINT	m_position;
	POINT	m_goalPos;
	int		m_speed;

	static TCHAR* message[];

public:
	Character( FiniteStateMachine* fsm, int speed );
	virtual ~Character(void);

	virtual void	Process();

	//id
	void SetID( int x );
	int GetID();

	//위치
	void SetPosition( int x, int y );
	void SetPosition( POINT pos );

	int GetPosX();
	int GetPosY();
	POINT GetPosition();

	//이동량
	void SetSpeed( int s );
	int GetSpeed();

	//골
	void SetGoal( int x, int y );
	void SetGoal( POINT pos );

	int GetGoalX();
	int GetGoalY();
	POINT GetGoal();

	void SetState( int state );
	int GetState();

	void SetTransition( DWORD _event );
	BOOL IsGoal();			//현재 골 지점까지 도착했는지 않했는지를 확인하는 함수
	void MoveTo();
	void Render( HDC dc );
};


#endif