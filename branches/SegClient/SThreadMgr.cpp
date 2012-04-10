#include "SThreadMgr.h"

SThreadMgr::SThreadMgr(void)
{
}

SThreadMgr::~SThreadMgr(void)
{
	//list�� �ִ� ��� �����带 ���� ����
	while( m_listThread.size() > 0 )
	{
		CloseHandle( *m_listThread.begin() );
		m_listThread.erase( m_listThread.begin() );
	}
}

void SThreadMgr::IsEndAllThread()
{
	std::list<HANDLE>::iterator iter = m_listThread.begin();
	for( ; iter != m_listThread.end(); )
	{
		if( WaitForSingleObject( *iter, INFINITE ) == WAIT_OBJECT_0 )
		{
			//�����尡 �� �̻� ������� ����
			CloseHandle( *iter );
			m_listThread.erase( iter++ );
			break;
		}
	}
	//��� �����尡 ���� ��
	//return �ȴ�
}

BOOL SThreadMgr::EndAllThread()
{
// 	std::list<HANDLE>::iterator iter = m_listThread.begin();
// 	for( ; iter != m_listThread.end(); )
// 	{
// 		PostThreadMessage( 
// 	}
	//��� �����尡 ���� �� return �ȴ�

	return TRUE;
}

HANDLE SThreadMgr::Create( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* threadID )
{
	HANDLE threadHandle;
	threadHandle = CreateThread( 0, 0, startAddress, parameter, 0, threadID );

	m_listThread.push_back( threadHandle );

	return threadHandle;
}
