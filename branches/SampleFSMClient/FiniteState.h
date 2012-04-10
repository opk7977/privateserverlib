#ifndef __FINITE_STATE__
#define __FINITE_STATE__

#include "SServerStd.h"

class FiniteState
{
private:
	//현재의 상태 ID
	DWORD						m_stateID;
	//각 상태에 맞는 Event와 OutputState값을 가지는 map
	std::map< DWORD, DWORD >	m_mapTransition;

public:
	FiniteState( DWORD state );
	~FiniteState(void);

	//이 FiniteState의 상태
	DWORD GetStateID();
	//상태에 맞는 Event와 OutputState를 등록
	void AddTransition( DWORD inputEvnet, DWORD outputEvent );
	//해당 transition삭제
	void DelTransition( DWORD inputEvnet );
	//상태와 Evnet에 따른 outputEvent를 return
	DWORD OutputState( DWORD inputEvent );
	
	//해당 상태의 transition개수
	DWORD GetCount();
};


#endif
