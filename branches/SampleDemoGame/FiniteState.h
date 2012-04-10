#pragma once

#include "SServerStd.h"

class CFiniteState
{
private:
	//현재의 상태 ID
	DWORD						m_stateID;
	//각 상태에 맞는 Event와 OutputState값을 가지는 map
	std::map< DWORD, DWORD >	m_mapTransition;

public:
	CFiniteState( DWORD state );
	~CFiniteState(void);

	//이 FiniteState의 상태
	DWORD GetStateID();
	//상태에 맞는 Event와 OutputState를 등록
	void AddTransition( DWORD inputEvent, DWORD outputState );
	//해당 transition삭제
	void DelTransition( DWORD inputEvent );
	//상태와 Event에 따른 OutputState를 return
	DWORD OutputState( DWORD inputEvent );

	//해당 상태의 transition개수
	DWORD GetCount();
};
