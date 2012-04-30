#include "SThreadMgr.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

SThreadMgr::SThreadMgr(void)
{
}

SThreadMgr::~SThreadMgr(void)
{
	//list에 있는 모든 쓰레드를 종료 삭제
	//while( m_listThread.size() > 0 )
	while( !m_listThread.empty() )
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
		else
		{
#ifdef _DEBUG
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[ThreadMgr::IsEndAllThread()] WaitForSingleObj Error return\n") );
#endif
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

//HANDLE SThreadMgr::Create( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* threadID )
HANDLE SThreadMgr::Create( LPVOID startAddress, LPVOID parameter, unsigned int *threadID )
{
	HANDLE threadHandle;
	threadHandle = (HANDLE)_beginthreadex( NULL, 0, (unsigned int(__stdcall*)(void*))startAddress, parameter, 0, threadID );
	//threadHandle = CreateThread( 0, 0, startAddress, parameter, 0, threadID );

	m_listThread.push_back( threadHandle );

	return threadHandle;
}
