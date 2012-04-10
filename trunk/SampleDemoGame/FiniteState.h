#pragma once

#include "SServerStd.h"

class CFiniteState
{
private:
	//������ ���� ID
	DWORD						m_stateID;
	//�� ���¿� �´� Event�� OutputState���� ������ map
	std::map< DWORD, DWORD >	m_mapTransition;

public:
	CFiniteState( DWORD state );
	~CFiniteState(void);

	//�� FiniteState�� ����
	DWORD GetStateID();
	//���¿� �´� Event�� OutputState�� ���
	void AddTransition( DWORD inputEvent, DWORD outputState );
	//�ش� transition����
	void DelTransition( DWORD inputEvent );
	//���¿� Event�� ���� OutputState�� return
	DWORD OutputState( DWORD inputEvent );

	//�ش� ������ transition����
	DWORD GetCount();
};
