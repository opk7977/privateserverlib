#ifndef __FINITE_STATE_MACHINE__
#define __FINITE_STATE_MACHINE__


#include "FiniteState.h"

//���¸� �����ϴ� class
class FiniteStateMachine
{
private:
	//<�������, FiniteState�ּҰ�>
	std::map< DWORD, FiniteState* >		m_mapMachine;
	//������ ����
	FiniteState*						m_currentState;

public:
	FiniteStateMachine(void);
	~FiniteStateMachine(void);

	//���¿� �̺�Ʈ�� ���� ��� ���¸� �߰��Ѵ�.
	//�������Id, �߻��̺�ƮId, �������ID
	void AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID );
	//����Ǿ� �ִ� ���¸� ����
	void DelStateTransition( DWORD stateID, DWORD inputEvent );
	//���� ���¿��� �Է¹��� event�� �´� ���¸� return�Ѵ�.
	DWORD GetOutPutState( DWORD inputEvnet );

	//�Է¹��� state�� �´� finiteState�� �޾� �д�.
	void SetCurrentState( DWORD stateID );
	//���� stateID�� return
	DWORD GetCurrentStateID();
	//���� ���¿��� �Է¹��� event�� �´� finiteState�� ����
	void StateTransition( int _event );
	void SetTransition( DWORD stateID );
};


#endif


