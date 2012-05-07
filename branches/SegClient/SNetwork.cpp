#include "SNetwork.h"
#include "SLogger.h"
#include "SPacketQueue.h"

SNetwork::SNetwork(void)
{
}

SNetwork::~SNetwork(void)
{
	Release();

	//�����尡 �ױ� ��ٸ���.
	WaitForSingleObject( GetThreadHandle(), 1000 );
}

BOOL SNetwork::Init( BOOL isNon /*= TRUE */ )
{
	if( !m_conSock.Init() )
		return FALSE;

	if( !m_conSock.CreateSocket() )
		return FALSE;

// 	if( isNon )
// 	{
// 		if( !m_conSock.SetNonBlkSock() )
// 			return FALSE;
// 	}

	return TRUE;
}

void SNetwork::Release()
{
	ResetEvent( m_hStartEvent );
	m_conSock.Release();
}

BOOL SNetwork::IsConnect()
{
	//����Ǿ� ������ TRUE
	return ( m_conSock.GetSocket() != INVALID_SOCKET );
}

BOOL SNetwork::ConnectToSrv( char* ipAddr, int port )
{
	m_hStartEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL);

	BeginThread();

	//SetEvent( m_hStartEvent );

	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

	if( !m_conSock.SetNonBlkSock() )
		return FALSE;

	SetEvent( m_hStartEvent );

	return TRUE;
}

BOOL SNetwork::ReConnect( char* ipAddr, int port )
{
	//�켱 ���� ����
	DisConnect();

	//���� �����
	Init();

	//SetEvent( m_hStartEvent );

	//�ٽ� ����
	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

	if( !m_conSock.SetNonBlkSock() )
		return FALSE;

	//Sleep(1);

	SetEvent( m_hStartEvent );

	return TRUE;
}

void SNetwork::DisConnect()
{
	ResetEvent( m_hStartEvent );
	m_conSock.Release();
}

BOOL SNetwork::Run()
{
	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	while(1)
	{
		//Sleep( 0 );

		WaitForSingleObject( m_hStartEvent, INFINITE );

		retval = recv( m_conSock.GetSocket(), tmpbuf, tmpbufSize, 0 );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
				GetLogger.ErrorLog( errorCode, _T("[SNetwork::Run()] ") );
				//return FALSE;
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

int SNetwork::SendPacket( SPacket* packet )
{
	int retval = send( m_conSock.GetSocket(), packet->GetDataBufferPtr(), packet->GetPacketSize(), 0 );

	if( retval < 0 )
	{
		GetLogger.ErrorLog( WSAGetLastError(), _T("[Network::SendPacket()] ") );
	}
	else if( retval == 0 )
	{
		//���� ����
		Release();
	}

	//�� ���ۉ�����
	return retval;
}
