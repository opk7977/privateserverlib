#pragma once

#include "FiniteState.h"

class CFiniteStateMachine
{
	//<���� ����, FiniteState�ּҰ�>
	std::map< DWORD, CFiniteState* >		m_mapMachine;
	//���� ����
	CFiniteState*						m_currentState;

public:
	CFiniteStateMachine(void);
	~CFiniteStateMachine(void);

	//���¿� �̺�Ʈ�� ���� ������¸� �߰��Ѵ�.
	//�������id, �߻��̹�Ʈid, �������id
	void AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateId );
	//����Ǿ� �ִ� ���¸� ����
	void DelStateTransition( DWORD stateID, DWORD inputEvent );
	//���� ���¿��� �Է¹��� event�� �´� ���¸� return�Ѵ�
	DWORD GetOutPutState( DWORD inputEvent );

	//�Է¹��� state�� �´� finiteState�� �޾� �д�.
	void SetCurrentState( DWORD stateID );
	//���� stateID�� return
	DWORD GetCurrentStateId();
	//���� ���¿��� �Է¹��� event�� �´� finiteState�� ����
	void StateTransition( int _event );
	void SetTransition( DWORD stateID );
};
