#include "FiniteState.h"

CFiniteState::CFiniteState( DWORD state ) : m_stateID(state)
{
}

CFiniteState::~CFiniteState(void)
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

DWORD CFiniteState::GetStateID()
{
	return m_stateID;
}

void CFiniteState::AddTransition( DWORD inputEvent, DWORD outputState )
{
	//�ش� event�� ��� ���¸� �����Ѵ�
	m_mapTransition[inputEvent] = outputState;
}

void CFiniteState::DelTransition( DWORD inputEvent )
{
	//�ش� ��ġ�� ���� �ش�.
	m_mapTransition.erase( inputEvent );
}

DWORD CFiniteState::OutputState( DWORD inputEvent )
{
	//���ǿ� �´� ���¸� return�� �ش�,
	std::map< DWORD, DWORD >::iterator iter;
	
	//�켱 ã�� ����
	iter = m_mapTransition.find( inputEvent );

	//������ -1 return
	if( iter == m_mapTransition.end() )
		return -1;

	//������ state return
	return m_mapTransition[inputEvent];
}

DWORD CFiniteState::GetCount()
{
	return (int)m_mapTransition.size();
}
