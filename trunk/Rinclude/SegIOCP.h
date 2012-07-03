#ifndef __IOCP__
#define __IOCP__

#include "SWorkThread.h"

class SegIOCP : public SSingleton <SegIOCP>
{
private:
	friend class SSingleton<SegIOCP>;

private:
	//IOCP�ڵ�
	HANDLE			m_hIOCP;
	//WORKER_THREAD_COUNT = 8
	SWorkThread*	m_pWorkThread[WORKER_THREAD_COUNT];
	int				m_workThreadCount;

private:
	SegIOCP(void);
	virtual ~SegIOCP(void);

public:
	//�ִ� WORKER_THREAD_COUNT���� ���������մϴ�
	BOOL	Init( int threadCount = WORKER_THREAD_COUNT );
	BOOL	Run();
	void	Release();
	HANDLE	GetIOCPWorkHandle();

	//�ܺο��� IOCP�� �ڵ��� ����ϴ� �Լ�
	BOOL	AddHandleToIOCP( HANDLE handle, DWORD key );
};

#define GetIOCP SegIOCP::GetInstance()

#endif

