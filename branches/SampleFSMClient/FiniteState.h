#ifndef __FINITE_STATE__
#define __FINITE_STATE__

#include "SServerStd.h"

class FiniteState
{
private:
	//������ ���� ID
	DWORD						m_stateID;
	//�� ���¿� �´� Event�� OutputState���� ������ map
	std::map< DWORD, DWORD >	m_mapTransition;

public:
	FiniteState( DWORD state );
	~FiniteState(void);

	//�� FiniteState�� ����
	DWORD GetStateID();
	//���¿� �´� Event�� OutputState�� ���
	void AddTransition( DWORD inputEvnet, DWORD outputEvent );
	//�ش� transition����
	void DelTransition( DWORD inputEvnet );
	//���¿� Evnet�� ���� outputEvent�� return
	DWORD OutputState( DWORD inputEvent );
	
	//�ش� ������ transition����
	DWORD GetCount();
};


#endif
