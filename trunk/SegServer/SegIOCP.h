#ifndef __IOCP__
#define __IOCP__

#include "SWorkThread.h"

class SegIOCP : public SSingleton <SegIOCP>
{
private:
	friend class SSingleton<SegIOCP>;

private:
	//IOCP핸들
	HANDLE			m_hIOCP;
	//WORKER_THREAD_COUNT = 8
	SWorkThread*	m_pWorkThread[WORKER_THREAD_COUNT];
	int				m_workThreadCount;

private:
	SegIOCP(void);
	virtual ~SegIOCP(void);

public:
	//최대 WORKER_THREAD_COUNT까지 설정가능합니다
	BOOL	Init( int threadCount = WORKER_THREAD_COUNT );
	BOOL	Run();
	void	Release();
	HANDLE	GetIOCPWorkHandle();

	//외부에서 IOCP에 핸들을 등록하는 함수
	BOOL	AddHandleToIOCP( HANDLE handle, DWORD key );
};

#define GetIOCP SegIOCP::GetInstance()

#endif

