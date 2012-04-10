#include "SMap.h"

SMap::SMap(void)
{
}

SMap::~SMap(void)
{
	Release();
}

void SMap::Release()
{
	//count�� 0���� �۰ų� ������ �׳� ���̹���
	if( m_Map.GetCount() <= 0 )
		return;

	//���鼭 �ϳ��ϳ� �޸� �Ҵ��� Ǯ�� �ְ�
	for( int i=0; i<m_iSize; ++i )
		delete m_Map[i+1];

	//���������� ��� �����
	m_Map.RemoveAll();
}

void SMap::PushIndex( int index, SServerObj* obj )
{
	m_Map[index] = obj;
	//map�� ũ��(session block�� ũ�⸦ �ϳ��� �÷���)
	++m_iSize;
}

void SMap::PopIndex( int index )
{
	delete m_Map[index];
	m_Map[index] = 0;
	m_Map.RemoveKey( index );
}

SServerObj* SMap::PeekObj( int index )
{
	return m_Map[index];
}

int SMap::GetSize()
{
	return m_iSize;
}
