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
	//�Ҵ�� ũ�⸸ŭ vector�� ũ�⸦ �Ҵ��� �ش�.
	m_vecIndex.resize( m_iMaxCount );
	//�Ҵ�� vector ������ index���� �־� �ش�
	for( int i=0; i<m_iMaxCount; ++i )
	{
		//����ڰ� ������ ù �� ���� index���� ����� �ش�.
		if( !PutIndex( i+startindex ) )
			return FALSE;
	}
	return TRUE;
}

void SIndexQueue::Release()
{
	//vector����
	if( m_vecIndex.empty() )
		return;

	m_vecIndex.clear();
}

BOOL SIndexQueue::IsPull()
{
	//��� �ϴ� ��ġ�� �̹� ���� ��� �ֵ���
	//vector�� �� ä���� �ִٴ� ���� �ȴ�.
	if( m_vecIndex[m_iWritePos] > 0 )
		return TRUE;

	return FALSE;
}

int SIndexQueue::IsEmpty()
{
	//�о�� �ϴ� ��ġ�� ���� ���ٸ�
	//vector�� ��� �ִٴ� ���� �ȴ�.
	if( m_vecIndex[m_iReadPos] < 0 )
		return TRUE;

	return FALSE;
}

int SIndexQueue::GetIndex()
{
	//�޾� �� �� �ִ� ���� ���ٸ�
	if( IsEmpty() )
		return -1;

	//���� �ϳ� �ް�
	int tmp = m_vecIndex[m_iReadPos];
	//�޾ư� �������� ����ٴ� �ǹ̷� -1�� �־� �ش�
	m_vecIndex[m_iReadPos] = -1;

	//�д� ��ġ�� �ϳ� �÷� �ְ�
	//��ġ�� �ְ��� �Ѿ� ���� �ٽ� 0���� �ǵ��� �ش�
	if( ++m_iReadPos >= m_iMaxCount )
		m_iReadPos = 0;

	return tmp;
}

BOOL SIndexQueue::PutIndex( int _index )
{
	//vector�� ���� ������ ���� �̻��ѰŴ�..
	if( IsPull() )
		return FALSE;

	//���� �־� �ְ� write��ġ�� �ϳ� �÷� �ش�
	m_vecIndex[m_iWritePos] = _index;
	//���� ��ġ�� �ϳ� �÷� �ְ�
	//��ġ�� �ְ��� �Ѿ� ���� �ٽ� 0���� �ǵ��� �ش�.
	if( ++m_iWritePos >= m_iMaxCount )
		m_iWritePos = 0;

	return TRUE;
}


