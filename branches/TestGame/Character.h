#pragma once

#include "FiniteStateMachine.h"
#include "CharacterState.h"

class CCharacter
{
public:
	//ĳ������ ���°�
	enum State
	{
		STATE_STAND = 0,

		STATE_MOVE_U,		//����			������
		STATE_MOVE_UL,		//���� ����		������
		STATE_MOVE_L,		//��������		������
		STATE_MOVE_LB,		//���� �Ʒ���	������
		STATE_MOVE_B,		//�Ʒ���		������
		STATE_MOVE_BR,		//������ �Ʒ��� ������
		STATE_MOVE_R,		//����������	������
		STATE_MOVE_RU,		//������ ����	������

		STATE_COUNT,
	};

	//ĳ������ �ൿ��
	enum Event
	{
		EVENT_KYE_UP,		//��� Ű�� ������ �ʴ´�.

		EVENT_U_DOWN,		//��			Ű�� ����
		EVENT_UL_DOWN,		//��,����		Ű�� ����
		EVENT_L_DOWN,		//����			Ű�� ����
		EVENT_LB_DOWN,		//����,�Ʒ�		Ű�� ����
		EVENT_B_DOWN,		//�Ʒ�			Ű�� ����
		EVENT_BR_DOWN,		//�Ʒ�,������	Ű�� ����
		EVENT_R_DOWN,		//������		Ű�� ����
		EVENT_RU_DOWN,		//������,��		Ű�� ����
	};

	//ĳ������ ���Ⱚ
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
	//���� ����
	CCharacterState*			m_action;
	//���� �迭
	CCharacterState*			m_arrayAction[STATE_COUNT];
	//���¸� �޾� ���� ���� �ӽ�
	CFiniteStateMachine*		m_stateMachine;

	//�ڽ��� indexID
	int		m_charID;
	//�ڽ��� ��ġ
	int		m_PosX;
	int		m_PosY;
	int		m_PosZ;
	//�ڽ��� ����
	int		m_direction;


	//���¸� �ٲٱ� ���� ����
	int		m_oldState;

public:
	CCharacter( CFiniteStateMachine* fsm );
	~CCharacter(void);

	//class������
	void Process();

	//indexID
	void SetIndexID( int id );
	int	GetIndexID();

	//Position
	void SetPos( int x, int y, int z );

	int GetX();
	int GetY();
	int GetZ();

	//����
	void SetDir( int dir );
	int GetDir();

	//���°�
	void SetState( int state );
	int GetState();

	//���°� ��ȭ
	void SetTransition( DWORD _event );

	//ĳ���Ϳ�����
	void MoveTo( int x, int y, int z );

	//�׸���
	void Render( HDC* dc );
};
