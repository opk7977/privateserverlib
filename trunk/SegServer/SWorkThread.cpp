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
	//�����尡 �ױ� ��ٸ���.
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
	//�����͸� �޴µ� �ʿ��� ������
	//--------------------------------------------------
	//���� ������ ���� ���� ������, return�� key���� ���� ����
	DWORD			bytesTransfer, keyValue;
	//return�Ǵ� overlapped����ü�� ���� ����ü ����
	LPOVERLAPPED	overlapped;
	DWORD			retval;
	//IOCP�ڵ��� �޾� ����
	HANDLE			handleIOCP = GetIOCP.GetIOCPWorkHandle();

	//workThread ����!
	while( m_bThreadLoop )
	{
		//�Ϸ���Ʈ�� ����� �޾� �´�.
		retval = GetQueuedCompletionStatus( handleIOCP, &bytesTransfer, &keyValue, &overlapped, INFINITE );

		//Sleep(0);

		//�Ϸ���Ʈ�� TRUE�� return �ϰ� �޾ƿ� ����鿡 �����Ͱ� �ִ��� üũ�ؼ� ó���Ѵ�
		if( retval == TRUE && keyValue != 0 && bytesTransfer != 0 && overlapped != 0 )
		{
			//���� ����� ó��
			SSession* session = 0;
			session = (SSession*)GetSessionMgr.GetSession( keyValue );

			//session�� ����� �޾� ������ ó���սô�!
			if( session != NULL )
				session->Dispatch( bytesTransfer, overlapped );
		}
		else
		{
			//��ȯ�� key���� �ְ�, ERROR_OVERATION_ABORTED�� ��ȯ�Ǹ�
			//WSARecv�����¿��� WSASend�� �ϰ� �Ǿ� ��ȯ���� ��Ұ��̹Ƿ�
			//������ �ƴϴ� �Ѿ� ���� �Ѵ�.
			//�׷��� �ʴٸ� session�� removeó���� �� �ش�.
			if( keyValue != 0 && GetLastError() != ERROR_OPERATION_ABORTED )
				GetSessionMgr.RemoveSession( (int)keyValue );
		}
	}

	OutputDebugString( _T("workThread����") );

	return TRUE;
}

void SWorkThread::Release()
{
	m_bThreadLoop = FALSE;
}
