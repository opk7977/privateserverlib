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
	//IOCP�� ����!
	m_hIOCP = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, WORKER_THREAD_COUNT );
	//���� ���и� ���ư���
	if( m_hIOCP == 0 )
		return FALSE;

	//work Thread�� ������ ������ŭ �����Ѵ�
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
	//�Ѱܹ��� key���� handle���� �Ϸ���Ʈ�� ����(?)�Ѵ�
	//�����ϸ� �Ѱܹ��� �ڵ��� handle���̴�
	if( CreateIoCompletionPort( handle, m_hIOCP, key, 0 ) != handle )
		return FALSE;	

	return TRUE;
}
