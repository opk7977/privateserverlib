#ifndef __CHARACTER__
#define __CHARACTER__

#include "FiniteStateMachine.h"
#include "CharacterState.h"

//TCHAR *message[] = { _T("stand"), _T("move") };

class Character
{
public:
	//character�� ���°��� ����
	enum State
	{
		STATE_STAND = 0,	//�� �ִ� ����, �⺻
		STATE_MOVE,			//������,
		STATE_COUNT,
	};

	//character�� �ൿ���� ����
	enum Event
	{
		EVENT_STOPWALK,		//�������� ����
		EVENT_STARTWALK,	//������
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

	//��ġ
	void SetPosition( int x, int y );
	void SetPosition( POINT pos );

	int GetPosX();
	int GetPosY();
	POINT GetPosition();

	//�̵���
	void SetSpeed( int s );
	int GetSpeed();

	//��
	void SetGoal( int x, int y );
	void SetGoal( POINT pos );

	int GetGoalX();
	int GetGoalY();
	POINT GetGoal();

	void SetState( int state );
	int GetState();

	void SetTransition( DWORD _event );
	BOOL IsGoal();			//���� �� �������� �����ߴ��� ���ߴ����� Ȯ���ϴ� �Լ�
	void MoveTo();
	void Render( HDC dc );
};


#endif