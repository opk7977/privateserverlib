#include "FiniteState.h"

FiniteState::FiniteState( DWORD state ) : m_stateID( state )
{
}

FiniteState::~FiniteState(void)
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

DWORD FiniteState::GetStateID()
{
	return m_stateID;
}

void FiniteState::AddTransition( DWORD inputEvnet, DWORD outputEvent )
{
	//해당 event에 결과 상태를 저장하자
	m_mapTransition[inputEvnet] = outputEvent;
}

void FiniteState::DelTransition( DWORD inputEvnet )
{
	//해당 위치를 지워 준다
	m_mapTransition.erase( inputEvnet );
}

DWORD FiniteState::OutputState( DWORD inputEvent )
{
	//조건에 맞는 상태를 return해 준다.
	std::map< DWORD, DWORD >::iterator iter;

	//맞는 상태를 찾아 보고
	iter = m_mapTransition.find( inputEvent );
	//없으면 -1 return해 주고
	if( iter == m_mapTransition.end() )
		return -1;

	//있으면 해당 위치의 output Event를 return 해 준다.
	return m_mapTransition[inputEvent];
}

DWORD FiniteState::GetCount()
{
	return (int)m_mapTransition.size();
}
