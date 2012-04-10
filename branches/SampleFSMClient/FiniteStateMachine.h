#ifndef __FINITE_STATE_MACHINE__
#define __FINITE_STATE_MACHINE__


#include "FiniteState.h"

//상태를 관리하는 class
class FiniteStateMachine
{
private:
	//<현재상태, FiniteState주소값>
	std::map< DWORD, FiniteState* >		m_mapMachine;
	//현재의 상태
	FiniteState*						m_currentState;

public:
	FiniteStateMachine(void);
	~FiniteStateMachine(void);

	//상태와 이벤트에 따라 결과 상태를 추가한다.
	//현재상태Id, 발생이벤트Id, 결과상태ID
	void AddStateTransition( DWORD stateID, DWORD inputEvent, DWORD outputStateID );
	//저장되어 있는 상태를 삭제
	void DelStateTransition( DWORD stateID, DWORD inputEvent );
	//지금 상태에서 입력받은 event에 맞는 상태를 return한다.
	DWORD GetOutPutState( DWORD inputEvnet );

	//입력받은 state에 맞는 finiteState를 받아 둔다.
	void SetCurrentState( DWORD stateID );
	//현재 stateID를 return
	DWORD GetCurrentStateID();
	//현재 상태에서 입력받은 event에 맞는 finiteState를 받음
	void StateTransition( int _event );
	void SetTransition( DWORD stateID );
};


#endif


