#include "SWorkThread.h"
#include "SSessionMgr.h"
#include "SegIOCP.h"

BOOL SWorkThread::m_bThreadLoop = TRUE;

SWorkThread::SWorkThread(void)
{
}

SWorkThread::~SWorkThread(void)
{
	m_bThreadLoop = FALSE;
	//쓰레드가 죽길 기다린다.
	WaitForSingleObject( GetThreadHandle(), 1000 );
	Release();
}

BOOL SWorkThread::Init()
{
	return TRUE;
}

BOOL SWorkThread::Run()
{
	//==================================================
	//데이터를 받는데 필요한 변수들
	//--------------------------------------------------
	//받은 데이터 양을 담을 변수와, return된 key값을 받을 변수
	DWORD			bytesTransfer, keyValue;
	//return되는 overlapped구조체를 담을 구조체 변수
	LPOVERLAPPED	overlapped;
	DWORD			retval;
	//IOCP핸들을 받아 오자
	HANDLE			handleIOCP = GetIOCP.GetIOCPWorkHandle();

	//workThread 동작!
	while( m_bThreadLoop )
	{
		//완료포트의 결과를 받아 온다.
		retval = GetQueuedCompletionStatus( handleIOCP, &bytesTransfer, &keyValue, &overlapped, INFINITE );

		//Sleep(0);

		//완료포트가 TRUE를 return 하고 받아온 결과들에 데이터가 있는지 체크해서 처리한다
		if( retval == TRUE && keyValue != 0 && bytesTransfer != 0 && overlapped != 0 )
		{
			//받은 결과를 처리
			SSession* session = 0;
			session = (SSession*)GetSessionMgr.GetSession( keyValue );

			//session을 제대로 받아 왔으면 처리합시다!
			if( session != NULL )
				session->Dispatch( bytesTransfer, overlapped );
		}
		else
		{
			//반환된 key값이 있고, ERROR_OVERATION_ABORTED이 반환되면
			//WSARecv대기상태에서 WSASend를 하게 되어 반환도니 취소값이므로
			//에러가 아니니 넘어 가야 한다.
			//그렇지 않다면 session의 remove처리를 해 준다.
			if( keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED )
				GetSessionMgr.RemoveSession( (int)keyValue );
		}
	}

	OutputDebugString( _T("workThread종료") );

	return TRUE;
}

void SWorkThread::Release()
{
	m_bThreadLoop = FALSE;
}
