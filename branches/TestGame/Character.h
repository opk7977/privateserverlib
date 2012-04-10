#pragma once

#include "FiniteStateMachine.h"
#include "CharacterState.h"

class CCharacter
{
public:
	//캐릭터의 상태값
	enum State
	{
		STATE_STAND = 0,

		STATE_MOVE_U,		//위로			움직임
		STATE_MOVE_UL,		//왼쪽 위로		움직임
		STATE_MOVE_L,		//왼쪽으로		움직임
		STATE_MOVE_LB,		//왼쪽 아래로	움직임
		STATE_MOVE_B,		//아래로		움직임
		STATE_MOVE_BR,		//오른쪽 아래로 움직임
		STATE_MOVE_R,		//오른쪽으로	움직임
		STATE_MOVE_RU,		//오른쪽 위로	움직임

		STATE_COUNT,
	};

	//캐릭터의 행동값
	enum Event
	{
		EVENT_KYE_UP,		//모든 키를 누르지 않는다.

		EVENT_U_DOWN,		//위			키를 누름
		EVENT_UL_DOWN,		//위,왼쪽		키를 누름
		EVENT_L_DOWN,		//왼쪽			키를 누름
		EVENT_LB_DOWN,		//왼쪽,아래		키를 누름
		EVENT_B_DOWN,		//아래			키를 누름
		EVENT_BR_DOWN,		//아래,오른쪽	키를 누름
		EVENT_R_DOWN,		//오른쪽		키를 누름
		EVENT_RU_DOWN,		//오른쪽,위		키를 누름
	};

	//캐릭터의 방향값
	enum Direction
	{
		DIRECTION_DUMMY,

		DIRECTION_U,
		DIRECTION_UL,
		DIRECTION_L,
		DIRECTION_LB,
		DIRECTION_B,
		DIRECTION_BR,
		DIRECTION_R,
		DIRECTION_RU,
	};

private:
	//현재 상태
	CCharacterState*			m_action;
	//상태 배열
	CCharacterState*			m_arrayAction[STATE_COUNT];
	//상태를 받아 오는 상태 머신
	CFiniteStateMachine*		m_stateMachine;

	//자신의 indexID
	int		m_charID;
	//자신의 위치
	int		m_PosX;
	int		m_PosY;
	int		m_PosZ;
	//자신의 방향
	int		m_direction;


	//상태를 바꾸기 전의 상태
	int		m_oldState;

public:
	CCharacter( CFiniteStateMachine* fsm );
	~CCharacter(void);

	//class움직임
	void Process();

	//indexID
	void SetIndexID( int id );
	int	GetIndexID();

	//Position
	void SetPos( int x, int y, int z );

	int GetX();
	int GetY();
	int GetZ();

	//방향
	void SetDir( int dir );
	int GetDir();

	//상태값
	void SetState( int state );
	int GetState();

	//상태값 변화
	void SetTransition( DWORD _event );

	//캐릭터움직임
	void MoveTo( int x, int y, int z );

	//그리기
	void Render( HDC* dc );
};
