#include "SIndexQueue.h"

SIndexQueue::SIndexQueue(void) : m_iReadPos(0)
							   , m_iWritePos(0)
{
}

SIndexQueue::~SIndexQueue(void)
{
}

BOOL SIndexQueue::Create( int count, int startindex )
{
	m_iMaxCount = count;
	//할당된 크기만큼 vector의 크기를 할당해 준다.
	m_vecIndex.resize( m_iMaxCount );
	//할당된 vector 공간에 index값을 넣어 준다
	for( int i=0; i<m_iMaxCount; ++i )
	{
		//사용자가 지정한 첫 값 부터 index값을 만들어 준다.
		if( !PutIndex( i+startindex ) )
			return FALSE;
	}
	return TRUE;
}

void SIndexQueue::Release()
{
	//vector빠이
	if( m_vecIndex.empty() )
		return;

	m_vecIndex.clear();
}

BOOL SIndexQueue::IsPull()
{
	//써야 하는 위치에 이미 값이 들어 있따면
	//vector는 꽉 채워져 있다는 것이 된다.
	if( m_vecIndex[m_iWritePos] > 0 )
		return TRUE;

	return FALSE;
}

int SIndexQueue::IsEmpty()
{
	//읽어야 하는 위치에 값이 없다면
	//vector는 비어 있다는 것이 된다.
	if( m_vecIndex[m_iReadPos] < 0 )
		return TRUE;

	return FALSE;
}

int SIndexQueue::GetIndex()
{
	//받아 올 수 있는 값이 없다면
	if( IsEmpty() )
		return -1;

	//값을 하나 받고
	int tmp = m_vecIndex[m_iReadPos];
	//받아간 공간에는 비었다는 의미로 -1을 넣어 준다
	m_vecIndex[m_iReadPos] = -1;

	//읽는 위치를 하나 올려 주고
	//위치가 최고값을 넘어 가면 다시 0으로 되돌려 준다
	if( ++m_iReadPos >= m_iMaxCount )
		m_iReadPos = 0;

	return tmp;
}

BOOL SIndexQueue::PutIndex( int _index )
{
	//vector가 꽉차 있으면 뭔가 이상한거다..
	if( IsPull() )
		return FALSE;

	//값을 넣어 주고 write위치를 하나 올려 준다
	m_vecIndex[m_iWritePos] = _index;
	//쓰는 위치를 하나 올려 주고
	//위치가 최고값을 넘어 가면 다시 0으로 되돌려 준다.
	if( ++m_iWritePos >= m_iMaxCount )
		m_iWritePos = 0;

	return TRUE;
}


