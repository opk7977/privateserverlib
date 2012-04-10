#include "SThreadMgr.h"

SThreadMgr::SThreadMgr(void)
{
}

SThreadMgr::~SThreadMgr(void)
{
	//list에 있는 모든 쓰레드를 종료 삭제
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
			//쓰레드가 더 이상 실행되지 않음
			CloseHandle( *iter );
			m_listThread.erase( iter++ );
			break;
		}
	}
	//모든 쓰레드가 종료 됨
	//return 된다
}

BOOL SThreadMgr::EndAllThread()
{
// 	std::list<HANDLE>::iterator iter = m_listThread.begin();
// 	for( ; iter != m_listThread.end(); )
// 	{
// 		PostThreadMessage( 
// 	}
	//모든 쓰레드가 종료 됨 return 된다

	return TRUE;
}

HANDLE SThreadMgr::Create( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* threadID )
{
	HANDLE threadHandle;
	threadHandle = CreateThread( 0, 0, startAddress, parameter, 0, threadID );

	m_listThread.push_back( threadHandle );

	return threadHandle;
}
