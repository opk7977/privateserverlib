#include "SNetwork.h"
#ifdef SERVER_LOG_PRINT
	#include "SLogger.h"
#endif
#include "SPacketQueue.h"

SNetwork::SNetwork(void)
/*: m_LoginSrvPort(0)*/
: m_isConnect(FALSE)
, m_isReConnect(FALSE)
, m_sendSize(0)
, m_workingThread(FALSE)
{
	ZeroMemory( m_sendStream, DEFAULT_BUFFER_SIZE );
	/*ZeroMemory( m_LoginSrvIp, 15 );*/

	//��Ŷ ť�� ����!
	m_packetQ = &GetPacketQ;
	//m_packetQ->Init();
}

SNetwork::~SNetwork(void)
{
	Release();

	//�����尡 �ױ� ��ٸ���.
	WaitForSingleObject( GetThreadHandle(), 1000 );
}

BOOL SNetwork::Init()
{
	//��Ŷ ť �ʱ�ȭ
	m_packetQ->Init();

	//�ʱ�ȭ
	if( !m_conSock.Init() )
		return FALSE;

	if( !m_conSock.CreateSocket() )
		return FALSE;

	return TRUE;
}

void SNetwork::Release()
{
	ResetEvent( m_hStartEvent );
	m_conSock.Release();

	m_packetQ->Release();
}

// void SNetwork::ConToLoginSrv( char* ipaddr, int port )
// {
// 	//������ �޾� ���´�
// 	CopyMemory( m_LoginSrvIp, ipaddr, strlen(ipaddr) );
// 	m_LoginSrvPort = port;
// 
// 	BeginThread();
// }

BOOL SNetwork::ConnectToSrv( char* ipAddr, int port )
{
	m_hStartEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL);

	if( !m_workingThread )
		BeginThread();

	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

	if( !m_conSock.SetNonBlkSock() )
		return FALSE;

	SetEvent( m_hStartEvent );
	//m_isConnect = TRUE;

	return TRUE;
}

BOOL SNetwork::ReConnect( char* ipAddr, int port )
{
	m_isReConnect = TRUE;

	//�켱 ���� ����
	DisConnect();

	//�ٽ� ����
	Init();

	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

	//Sleep(1);

	if( !m_conSock.SetNonBlkSock() )
		return FALSE;

	SetEvent( m_hStartEvent );

	m_isReConnect = FALSE;

	return TRUE;
}

BOOL SNetwork::IsCounnect()
{
	return m_isConnect;
}

void SNetwork::DisConnect()
{
	m_isConnect = FALSE;
	ResetEvent( m_hStartEvent );
	m_conSock.Release();
}

BOOL SNetwork::Run()
{
	//ConnectToSrv( m_LoginSrvIp, m_LoginSrvPort );
	m_workingThread = TRUE;

	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	while(1)
	{
		Sleep( 0 );

		WaitForSingleObject( m_hStartEvent, INFINITE );

		retval = recv( m_conSock.GetSocket(), tmpbuf, tmpbufSize, 0 );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
				if( !m_isReConnect )
				{
					//�ٽ� ������ ���� ��� ���� ���°� �ƴϸ� ��¥ ����
#ifdef SERVER_LOG_PRINT
					GetLogger.ErrorLog( errorCode, _T("[SNetwork::Run()] ") );
#endif
					return FALSE;
				}
			}
		}
		else
		{
			recvSize += retval;

			while( recvSize > 0 )
			{
				m_recvPack.CopyToPacket( buffer, recvSize );

				if( m_recvPack.IsValidPacket() && m_recvPack.GetPacketSize() <= recvSize )
				{
					//��Ŷ�� ��Ŷť�� �־� �д�.
					GetPacketQ.PutPacket( m_recvPack.GetDataBufferPtr(), m_recvPack.GetPacketSize() );

					//�޾� ó���� ��ŭ�� ũ�⸦ ���ְ�
					recvSize -= m_recvPack.GetPacketSize();

					//Ȥ�� ���� ��Ŷ�� �Ѱ� �̻��̶�� �� Ȯ���ؾ� �Ѵ�
					if( recvSize > 0 )
					{
						//���� �޾ƾ� �Ұ� ���Ҵٸ� �켱 buffer�� �־� �д�
						CopyMemory( buffer, ( m_recvPack.GetDataBufferPtr()+m_recvPack.GetPacketSize() ), recvSize );
						tmpbuf += recvSize;
					}
					else
					{
						//�� �޾Ƽ� ���� �޾ƾ� �ϴϱ� ������ ó������ ���ư���
						tmpbuf = &buffer[0];
					}
				}
				else
				{
					//�׳� ���� �� ����...���ư��� �� ����.
					tmpbuf += recvSize;
					break;
				}
			}
		}
	}
	return TRUE;
}

BOOL SNetwork::PilePacket( SPacket* packet )
{
	int size = packet->GetPacketSize();

	//ũ�⸦ Ȯ���Ѵ�
	if( m_sendSize + size > DEFAULT_BUFFER_SIZE )
	{
		//�� Ŀ���� �켱 ������ ����
		if( !SendPacket() )
			return FALSE;
	}

	CopyMemory( m_sendStream + m_sendSize, packet->GetDataBufferPtr(), size );
	m_sendSize += size;

	return TRUE;
}

BOOL SNetwork::SendPacket( SPacket* packet )
{
	int retval = send( m_conSock.GetSocket(), packet->GetDataBufferPtr(), packet->GetPacketSize(), 0 );

	if( retval < 0 )
	{
		DWORD errorcode = WSAGetLastError();
		if( errorcode != WSAEWOULDBLOCK )
		{
#ifdef SERVER_LOG_PRINT
			GetLogger.ErrorLog( WSAGetLastError(), _T("[Network::SendPacket()] ") );
#endif
			Release();
			return FALSE;
		}
	}
	else if( retval == 0 )
	{
		//���� ����
		Release();
		return FALSE;
	}

	//�� ���ۉ�����
	return TRUE;
}

BOOL SNetwork::SendPacket()
{
	//����Ǿ� ���� ������ �������� �ʴ´�
	if( !m_isConnect )
		return TRUE;

	//������ �����Ͱ� ������ �������� �ʴ´�
	if( m_sendSize <= 0 )
		return TRUE;

	int retval = send( m_conSock.GetSocket(), m_sendStream, m_sendSize, 0 );

	if( retval < 0 )
	{
		DWORD errorcode = WSAGetLastError();
		if( errorcode != WSAEWOULDBLOCK )
		{
#ifdef SERVER_LOG_PRINT
			GetLogger.ErrorLog( WSAGetLastError(), _T("[Network::SendPacket()] ") );
#endif
			Release();
			return FALSE;
		}
	}
	else if( retval == 0 )
	{
		//���� ����
		Release();
		return FALSE;
	}
	if( retval >= 20 )
		int a = 1000;

	//�� ���ۉ�����
	m_sendSize = 0;
	return TRUE;
}
