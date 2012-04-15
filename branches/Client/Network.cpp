#include "StdAfx.h"
#include "Network.h"
#include "SLogger.h"
#include "PacketQueue.h"

//������ ���� �Լ�
unsigned int __stdcall _RecvRunc( void* pArg )
{
	return ((Network*)pArg)->RecvEvent();
}

Network::Network(void) : m_iThread(0)
{
}

Network::~Network(void)
{
	Release();
}

BOOL Network::Init( BOOL isNon /*= TRUE */ )
{
	if( !m_conSock.Init() )
		return FALSE;

	if( !m_conSock.CreateSocket() )
		return FALSE;

	if( isNon )
	{
		if( !m_conSock.SetNonBlkSock() )
			return FALSE;
	}

	return TRUE;
}

void Network::Release()
{
	ResetEvent( m_startWork );
	m_conSock.Release();
}

BOOL Network::ConnectToSrv( char* ipAddr, int port )
{
	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL);

	m_recvThread = (HANDLE)_beginthreadex( NULL, 0, &::_RecvRunc, (void*)this, 0, &m_iThread );
	if( !m_recvThread )
		return FALSE;

	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

	SetEvent( m_startWork );

	return TRUE;
}

BOOL Network::Reconnect( char* ipAddr, int port )
{
	//�켱 ���� ����
	DisConnect();

	//�ٽ� ����
	Init();

	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

	Sleep(2);

	SetEvent( m_startWork );

	return TRUE;	
}

void Network::DisConnect()
{
	ResetEvent( m_startWork );
	m_conSock.Release();
}

unsigned int Network::RecvEvent()
{
	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	while(1)
	{
		Sleep( 10 );

		WaitForSingleObject( m_startWork, INFINITE );
		retval = recv( m_conSock.GetSocket(), tmpbuf, tmpbufSize, 0 );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
				GetLogger.ErrorLog( errorCode, _T("[Network::RecvEvent()] ") );
				return FALSE;
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
	return 0;
}

int Network::SendPacket( SPacket* packet )
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
