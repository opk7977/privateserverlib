#ifndef __INDEX_QEUEUE__
#define __INDEX_QEUEUE__


#include "SServerobj.h"


class SIndexQueue : public SServerObj
{
	//�⺻ ������� 100���� �����ȴ�
	//�����Ϸ��� SServerStd���� ������ �� ����
	std::vector<int>		m_vecIndex;

	int						m_iReadPos;
	int						m_iWritePos;

	int						m_iMaxCount;

public:
	SIndexQueue(void);
	virtual ~SIndexQueue(void);

	//�ʱ�ȭ �Լ�
	//startindex���� ����
	BOOL	Create( int count, int startindex );
	void	Release();

	//vector�� �� ���� ������ true�� return
	BOOL	IsPull();
	//vector�� ��� ������ true�� return
	BOOL	IsEmpty();
	//����� �� �ִ� index���� ���´�.
	//��� ������ index���� ���ٸ� -1�� return
	int		GetIndex();
	//������ index���� return
	int		PeekIndex() const { return m_iReadPos; }
	//�Ѱ� ���� index�� �ٽ� ����� �� �ֵ��� �ڿ� �־� �ش�.
	BOOL	PutIndex( int _index );
};


#endif

