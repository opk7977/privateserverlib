#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(void) : m_currentState(0)
{
}

FiniteStateMachine::~FiniteStateMachine(void)
{
	//map�� �ִ� �͵��� �� ����
	std::map< DWORD, FiniteState* >::iterator iter, iterPrev;

	iter = m_mapMachine.begin();
	while( iter != m_mapMachine.end() )
	{
		iterPrev = iter++;
		//FiniteState�� �Ҵ�� ������ ���� �ְ�
		delete iterPrev->second;
		m_mapMachine.erase( iterPrev );
	}
	m_mapMachine.clear();
}

void FiniteStateMachine::AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID )
{
	std::map< DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//�̹� �ش� ���°� �ִ��� �ߺ� �˻�
	iter = m_mapMachine.begin();
	while( iter != m_mapMachine.end() )
	{
		state = iter->second;
		//������ ���� ���� �ʿ�� ����.
		if( state->GetStateID() == stateID )
			break;

		iter++;
	}

	//���� ������ ID�� ���� ���� �ʴ´ٸ� ���� �����ؾ� �Ѵ�
	if( iter == m_mapMachine.end() )
	{
		state = new FiniteState( stateID );
		m_mapMachine[state->GetStateID()] = state;
	}

	//���� �ش� ���¿� �̺�Ʈ�� ������¸� ��������
	//inputEvent�� ���� �ߺ� Ȯ���� ���� �ʾ���
	state->AddTransition( inputEvent, outputStateID );
}

void FiniteStateMachine::DelStateTransition( DWORD stateID, DWORD inputEvent )
{
	std::map< DWORD, FiniteState* >::iterator iter, iterPrev;
	FiniteState* state;

	//�Է¹��� state�� �´� ���¸� ã�´�.
	iter = m_mapMachine.begin();
	while( iter != m_mapMachine.end() )
	{
		iterPrev = iter++;
		state = iterPrev->second;

		//ã������ �׸�����
		if( state->GetStateID() == stateID )
			break;
	}

	//ã�� �������� ���� ����.
	if( iter == m_mapMachine.end() )
		return;

	//ã������ �ش� ������ �����.
	state->DelTransition( inputEvent );
	//����� �ش� ���¿� �ƹ��� �ൿ�� ���� ���� �ʴٸ� transition�� ���� �ش�.
	if( state->GetCount() == 0 )
	{
		delete state;
		m_mapMachine.erase( iterPrev );
	}
}

DWORD FiniteStateMachine::GetOutPutState( DWORD inputEvnet )
{
	std::map< DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//�Է¹��� Event�� �������� �ʴٸ� ������ return
	iter = m_mapMachine.find( GetCurrentStateID() );
	//������ -1 return
	if( iter == m_mapMachine.end() )
		return -1;

	//�ִٸ� �ش��ϴ� ������ �޾� �ͼ�
	state = m_mapMachine[GetCurrentStateID()];

	//event�� return �� �ش�.
	return state->OutputState( inputEvnet );
}

void FiniteStateMachine::SetCurrentState( DWORD stateID )
{
	std::map< DWORD, FiniteState* >::iterator iter;

	//������ ���� ����
	iter = m_mapMachine.find( stateID );
	if( iter == m_mapMachine.end() )
		return;

	//������ ����
	m_currentState = iter->second;
}

DWORD FiniteStateMachine::GetCurrentStateID()
{
	//����Ǿ� �ִ� ���� ���°� ������ -1 return
	if( m_currentState == 0 )
		return -1;

	return m_currentState->GetStateID();
}

void FiniteStateMachine::StateTransition( int _event )
{
	//����Ǿ� �ִ� ���� ���°� ������ -1 return
	if( m_currentState == 0 )
		return;

	//event�� �´� ��� state���� ����
	DWORD outputState = m_currentState->OutputState( _event );
	//�ش� ����� üũ�Ѵ�
	if( outputState == -1 )
		return;

	//�������� ������ ���state�� �ش��ϴ� FiniteState�� �����Ѵ�.
	m_currentState = m_mapMachine[outputState];
}

void FiniteStateMachine::SetTransition( DWORD stateID )
{
	m_currentState = m_mapMachine[stateID];
}
