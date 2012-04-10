#include "SegIOCP.h"

SegIOCP::SegIOCP(void) : m_hIOCP(NULL)
{
}

SegIOCP::~SegIOCP(void)
{
	Release();
}

BOOL SegIOCP::Init()
{
	//IOCP를 생성!
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, WORKER_THREAD_COUNT );
	//생성 실패면 돌아가자
	if( m_hIOCP == 0 )
		return FALSE;

	//work Thread를 지정된 개수만큼 생성한다
	for( int i=0; i<WORKER_THREAD_COUNT; ++i )
	{
		m_pWorkThread[i] = new SWorkThread;
		m_pWorkThread[i]->BeginThread();
	}

	return TRUE;
}

BOOL SegIOCP::Run()
{
	return TRUE;
}

void SegIOCP::Release()
{
	for( int i=0; i<WORKER_THREAD_COUNT; ++i )
		delete m_pWorkThread[i];
}

HANDLE SegIOCP::GetIOCPWorkHandle()
{
	return m_hIOCP;
}

BOOL SegIOCP::AddHandleToIOCP( HANDLE handle, DWORD key )
{
	//넘겨받은 key값과 handle값을 완료포트에 연결(?)한다
	//성공하면 넘겨받은 핸들은 handle값이다
	if( CreateIoCompletionPort( handle, m_hIOCP, key, 0 ) != handle )
		return FALSE;	

	return TRUE;
}
