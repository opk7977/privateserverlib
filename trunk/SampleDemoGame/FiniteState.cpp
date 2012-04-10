#include "FiniteState.h"

CFiniteState::CFiniteState( DWORD state ) : m_stateID(state)
{
}

CFiniteState::~CFiniteState(void)
{
	//맵에 있는 모든 항목을 다 지워 준다.
	std::map< DWORD, DWORD >::iterator iter, iterPrev;

	iter = m_mapTransition.begin();
	while( iter != m_mapTransition.end() )
	{
		//현재를 저장하고 앞으로 넘어감
		iterPrev = iter++;
		//저장해논 현재 위치를 지움
		m_mapTransition.erase( iterPrev );
	}
	//전체 공간도 삭제!
	m_mapTransition.clear();
}

DWORD CFiniteState::GetStateID()
{
	return m_stateID;
}

void CFiniteState::AddTransition( DWORD inputEvent, DWORD outputState )
{
	//해당 event에 결과 상태를 저장한다
	m_mapTransition[inputEvent] = outputState;
}

void CFiniteState::DelTransition( DWORD inputEvent )
{
	//해당 위치를 지워 준다.
	m_mapTransition.erase( inputEvent );
}

DWORD CFiniteState::OutputState( DWORD inputEvent )
{
	//조건에 맞는 상태를 return해 준다,
	std::map< DWORD, DWORD >::iterator iter;
	
	//우선 찾아 보고
	iter = m_mapTransition.find( inputEvent );

	//없으면 -1 return
	if( iter == m_mapTransition.end() )
		return -1;

	//있으면 state return
	return m_mapTransition[inputEvent];
}

DWORD CFiniteState::GetCount()
{
	return (int)m_mapTransition.size();
}
