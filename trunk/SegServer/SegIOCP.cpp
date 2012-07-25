#include "SegIOCP.h"

SegIOCP::SegIOCP(void)
: m_hIOCP(NULL)
, m_workThreadCount(0)
{
}

SegIOCP::~SegIOCP(void)
{
	//Release();
}

BOOL SegIOCP::Init( int threadCount /*= WORKER_THREAD_COUNT */ )
{
	m_workThreadCount = threadCount;
	if( m_workThreadCount > WORKER_THREAD_COUNT )
		m_workThreadCount = WORKER_THREAD_COUNT;

	//IOCP�� ����!
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, m_workThreadCount );
	//���� ���и� ���ư���
	if( m_hIOCP == 0 )
		return FALSE;

	//work Thread�� ������ ������ŭ �����Ѵ�
	for( int i=0; i<m_workThreadCount; ++i )
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
	for( int i=0; i<m_workThreadCount; ++i )
		m_pWorkThread[i]->EndThread();

	if ( m_hIOCP )
	{
		::CloseHandle( m_hIOCP );
		m_hIOCP = NULL;
	}
}

HANDLE SegIOCP::GetIOCPWorkHandle()
{
	return m_hIOCP;
}

BOOL SegIOCP::AddHandleToIOCP( HANDLE handle, DWORD key )
{
	//�Ѱܹ��� key���� handle���� �Ϸ���Ʈ�� ����(?)�Ѵ�
	//�����ϸ� �Ѱܹ��� �ڵ��� handle���̴�
	if( CreateIoCompletionPort( handle, m_hIOCP, key, 0 ) != handle )
		return FALSE;

	return TRUE;
}
