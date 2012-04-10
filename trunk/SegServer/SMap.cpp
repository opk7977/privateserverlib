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
	//count가 0보다 작거나 같으면 그냥 바이바이
	if( m_Map.GetCount() <= 0 )
		return;

	//돌면서 하나하나 메모리 할당을 풀어 주고
	for( int i=0; i<m_iSize; ++i )
		delete m_Map[i+1];

	//마지막으로 모두 지운다
	m_Map.RemoveAll();
}

void SMap::PushIndex( int index, SServerObj* obj )
{
	m_Map[index] = obj;
	//map의 크기(session block의 크기를 하나씩 늘려줌)
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
