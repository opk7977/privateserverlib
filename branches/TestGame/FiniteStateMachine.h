#pragma once

#include "FiniteState.h"

class CFiniteStateMachine
{
	//<현재 상태, FiniteState주소값>
	std::map< DWORD, CFiniteState* >		m_mapMachine;
	//현재 상태
	CFiniteState*						m_currentState;

public:
	CFiniteStateMachine(void);
	~CFiniteStateMachine(void);

	//상태와 이벤트에 따라 결과상태를 추가한다.
	//현재상태id, 발생이밴트id, 결과상태id
	void AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateId );
	//저장되어 있는 상태를 삭제
	void DelStateTransition( DWORD stateID, DWORD inputEvent );
	//지금 상태에서 입력받은 event에 맞는 상태를 return한다
	DWORD GetOutPutState( DWORD inputEvent );

	//입력받은 state에 맞는 finiteState를 받아 둔다.
	void SetCurrentState( DWORD stateID );
	//현재 stateID를 return
	DWORD GetCurrentStateId();
	//현재 상태에서 입력받은 event에 맞는 finiteState를 받음
	void StateTransition( int _event );
	void SetTransition( DWORD stateID );
};
