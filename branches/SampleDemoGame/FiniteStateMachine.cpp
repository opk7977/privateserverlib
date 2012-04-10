#include "StdAfx.h"
#include "FiniteStateMachine.h"

CFiniteStateMachine::CFiniteStateMachine(void) : m_currentState(0)
{
}

CFiniteStateMachine::~CFiniteStateMachine(void)
{
	//map�� �ִ� �͵��� �� ����
	std::map< DWORD, CFiniteState* >::iterator iter, iterPrev;

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

void CFiniteStateMachine::AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateId )
{
	std::map< DWORD, CFiniteState* >::iterator iter;
	CFiniteState*	state;

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
		state = new CFiniteState( stateID );
		m_mapMachine[state->GetStateID()] = state;
	}

	//�ش� ���¿� �̹�Ʈ�� ��� ���¸� �����Ѵ�.
	state->AddTransition( inputEvent, outputStateId );
}

void CFiniteStateMachine::DelStateTransition( DWORD stateID, DWORD inputEvent )
{
	std::map< DWORD, CFiniteState* >::iterator iter, iterPrev;
	CFiniteState* state;

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

	//ã������
	state->DelTransition( inputEvent );
	//����� �ش� ���¿� �ƹ��� �ൿ�� ���� ���� �ʴٸ� transition�� �����ش�.
	if( state->GetCount() == 0 )
	{
		delete state;
		m_mapMachine.erase( iter );
	}
}

DWORD CFiniteStateMachine::GetOutPutState( DWORD inputEvent )
{
	std::map< DWORD, CFiniteState* >::iterator iter;
	CFiniteState* state;

	//�Է¹��� Event�� �������� �ʴٸ� ������ return
	iter = m_mapMachine.find( GetCurrentStateId() );
	//������ -1 return
	if( iter == m_mapMachine.end() )
		return -1;

	//�ִٸ� �ش��ϴ� ������ �޾� �ͼ�
	state = m_mapMachine[GetCurrentStateId()];

	//event�� return �� �ش�.
	return state->OutputState( inputEvent );
}

void CFiniteStateMachine::SetCurrentState( DWORD stateID )
{
	std::map< DWORD, CFiniteState* >::iterator iter;

	//������ ���� ����
	iter = m_mapMachine.find( stateID );
	if( iter == m_mapMachine.end() )
		return;

	//������ ����
	m_currentState = iter->second;
}

DWORD CFiniteStateMachine::GetCurrentStateId()
{
	//����Ǿ� �ִ� ���� ���°� ������ -1 return
	if( m_currentState == 0 )
		return -1;

	return m_currentState->GetStateID();
}

void CFiniteStateMachine::StateTransition( int _event )
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

void CFiniteStateMachine::SetTransition( DWORD stateID )
{
	m_currentState = m_mapMachine[stateID];
}
