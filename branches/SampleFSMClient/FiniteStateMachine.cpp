#include "FiniteStateMachine.h"

FiniteStateMachine::FiniteStateMachine(void) : m_currentState(0)
{
}

FiniteStateMachine::~FiniteStateMachine(void)
{
	//map에 있는 것들을 다 없앰
	std::map< DWORD, FiniteState* >::iterator iter, iterPrev;

	iter = m_mapMachine.begin();
	while( iter != m_mapMachine.end() )
	{
		iterPrev = iter++;
		//FiniteState는 할당된 공간을 지워 주고
		delete iterPrev->second;
		m_mapMachine.erase( iterPrev );
	}
	m_mapMachine.clear();
}

void FiniteStateMachine::AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID )
{
	std::map< DWORD, FiniteState* >::iterator iter;
	FiniteState* state;

	//이미 해당 상태가 있는지 중복 검사
	iter = m_mapMachine.begin();
	while( iter != m_mapMachine.end() )
	{
		state = iter->second;
		//있으면 새로 만들 필요는 없다.
		if( state->GetStateID() == stateID )
			break;

		iter++;
	}

	//만약 동일한 ID가 존재 하지 않는다면 새로 생성해야 한다
	if( iter == m_mapMachine.end() )
	{
		state = new FiniteState( stateID );
		m_mapMachine[state->GetStateID()] = state;
	}

	//이제 해당 상태에 이벤트와 결과상태를 저장하자
	//inputEvent에 대한 중복 확인은 하지 않았음
	state->AddTransition( inputEvent, outputStateID );
}

void FiniteStateMachine::DelStateTransition( DWORD stateID, DWORD inputEvent )
{
	std::map< DWORD, FiniteState* >::iterator iter, iterPrev;
	FiniteState* state;

	//입력받은 state에 맞는 상태를 찾는다.
	iter = m_mapMachine.begin();
	while( iter != m_mapMachine.end() )
	{
		iterPrev = iter++;
		state = iterPrev->second;

		//찾았으면 그만돌자
		if( state->GetStateID() == stateID )
			break;
	}

	//찾지 못했으면 돌아 간다.
	if( iter == m_mapMachine.end() )
		return;

	//찾았으면 해당 정보를 지운다.
	state->DelTransition( inputEvent );
	//지우고 해당 상태에 아무런 행동이 남아 있지 않다면 transition을 지워 준다.
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

	//입력받은 Event가 존재하지 않다면 오류를 return
	iter = m_mapMachine.find( GetCurrentStateID() );
	//없으면 -1 return
	if( iter == m_mapMachine.end() )
		return -1;

	//있다면 해당하는 정보를 받아 와서
	state = m_mapMachine[GetCurrentStateID()];

	//event를 return 해 준다.
	return state->OutputState( inputEvnet );
}

void FiniteStateMachine::SetCurrentState( DWORD stateID )
{
	std::map< DWORD, FiniteState* >::iterator iter;

	//없으면 돌아 간다
	iter = m_mapMachine.find( stateID );
	if( iter == m_mapMachine.end() )
		return;

	//있으면 저장
	m_currentState = iter->second;
}

DWORD FiniteStateMachine::GetCurrentStateID()
{
	//저장되어 있는 현재 상태가 없으면 -1 return
	if( m_currentState == 0 )
		return -1;

	return m_currentState->GetStateID();
}

void FiniteStateMachine::StateTransition( int _event )
{
	//저장되어 있는 현재 상태가 없으면 -1 return
	if( m_currentState == 0 )
		return;

	//event에 맞는 결과 state값을 받음
	DWORD outputState = m_currentState->OutputState( _event );
	//해당 결과를 체크한다
	if( outputState == -1 )
		return;

	//정상적인 결과라면 결과state에 해당하는 FiniteState를 저장한다.
	m_currentState = m_mapMachine[outputState];
}

void FiniteStateMachine::SetTransition( DWORD stateID )
{
	m_currentState = m_mapMachine[stateID];
}
