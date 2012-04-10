#include "StdAfx.h"
#include "Network.h"
#include "SLogger.h"

//쓰레드 전역 함수
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

void Network::Init( BOOL isNon /*= TRUE */ )
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
	m_srvSock.Release();
}

void Network::ConnectToSrv( char* ipAddr, int port )
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

void Network::Reconnect( char* ipAddr, int port )
{
	//우선 연결 끊고
	DisConnect();

	//다시 연결
	if( !m_conSock.ConnectSock( ipAddr, port ) )
		return FALSE;

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
		retval = recv( m_srvSock.GetSocket(), tmpbuf, tmpbufSize, 0 );

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
					//패킷을 처리 할 수 있는 함수 호출
					PacketParsing( &m_recvPack );

					//혹시 받은 패킷이 한개 이상이라면 또 확인해야 한다
					recvSize -= m_recvPack.GetPacketSize();
					if( recvSize > 0 )
					{
						CopyMemory( buffer, ( m_recvPack.GetDataBufferPtr()+recvSize-4 ), recvSize );
						tmpbuf += recvSize;
					}
				}
				else
				{
					//아니면 돌아가서 더 받자.
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
	int retval = send( m_srvSock.GetSocket(), packet->GetDataBufferPtr(), packet->GetPacketSize(), 0 );

	if( retval < 0 )
	{
		GetLogger.ErrorLog( WSAGetLastError(), _T("[Network::SendPacket()] ") );
	}
	else if( retval == 0 )
	{
		//연결 끊기
		Release();
	}

	//잘 전송됬으면
	return retval;
}
