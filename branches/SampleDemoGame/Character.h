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
		EVENT_KYE_UP,		//���� Ű�� ������ �ʴ´�.

		EVENT_U_DOWN,		//��			Ű�� ����
		EVENT_UL_DOWN,		//��,����		Ű�� ����
		EVENT_L_DOWN,		//����			Ű�� ����
		EVENT_LB_DOWN,		//����,�Ʒ�		Ű�� ����
		EVENT_B_DOWN,		//�Ʒ�			Ű�� ����
		EVENT_BR_DOWN,		//�Ʒ�,������	Ű�� ����
		EVENT_R_DOWN,		//������		Ű�� ����
		EVENT_RU_DOWN,		//������,��		Ű�� ����
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
	//�ڽ��� charID
	TCHAR	m_strID[50];
	//�ڽ��� ��ġ
	POINT	m_position;


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

	//charID
	void SetStrID( TCHAR* id );
	TCHAR* GetStrID();

	//Position
	void SetX( int x );
	void SetY( int y );
	void SetPos( int x, int y );
	void SetPos( POINT pos );

	int GetX();
	int GetY();
	POINT GetPos();

	//���°�
	void SetState( int state );
	int GetState();

	//���°� ��ȭ
	void SetTransition( DWORD _event );

	//ĳ���Ϳ�����
	void MoveTo( int x, int y );

	//�׸���
	void Render( CDC* dc );
};
