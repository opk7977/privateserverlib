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
	//WORKER_THREAD_COUNT = 3
	SWorkThread*	m_pWorkThread[WORKER_THREAD_COUNT];

private:
	SegIOCP(void);
	virtual ~SegIOCP(void);

public:
	BOOL	Init();
	BOOL	Run();
	void	Release();
	HANDLE	GetIOCPWorkHandle();

	//�ܺο��� IOCP�� �ڵ��� ����ϴ� �Լ�
	BOOL	AddHandleToIOCP( HANDLE handle, DWORD key );
};

#define GetIOCP SegIOCP::GetInstance()

#endif

