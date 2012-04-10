#include "FiniteState.h"

FiniteState::FiniteState( DWORD state ) : m_stateID( state )
{
}

FiniteState::~FiniteState(void)
{
	//�ʿ� �ִ� ��� �׸��� �� ���� �ش�.
	std::map< DWORD, DWORD >::iterator iter, iterPrev;

	iter = m_mapTransition.begin();
	while( iter != m_mapTransition.end() )
	{
		//���縦 �����ϰ� ������ �Ѿ
		iterPrev = iter++;
		//�����س� ���� ��ġ�� ����
		m_mapTransition.erase( iterPrev );
	}
	//��ü ������ ����!
	m_mapTransition.clear();
}

DWORD FiniteState::GetStateID()
{
	return m_stateID;
}

void FiniteState::AddTransition( DWORD inputEvnet, DWORD outputEvent )
{
	//�ش� event�� ��� ���¸� ��������
	m_mapTransition[inputEvnet] = outputEvent;
}

void FiniteState::DelTransition( DWORD inputEvnet )
{
	//�ش� ��ġ�� ���� �ش�
	m_mapTransition.erase( inputEvnet );
}

DWORD FiniteState::OutputState( DWORD inputEvent )
{
	//���ǿ� �´� ���¸� return�� �ش�.
	std::map< DWORD, DWORD >::iterator iter;

	//�´� ���¸� ã�� ����
	iter = m_mapTransition.find( inputEvent );
	//������ -1 return�� �ְ�
	if( iter == m_mapTransition.end() )
		return -1;

	//������ �ش� ��ġ�� output Event�� return �� �ش�.
	return m_mapTransition[inputEvent];
}

DWORD FiniteState::GetCount()
{
	return (int)m_mapTransition.size();
}
