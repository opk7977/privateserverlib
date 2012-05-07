#include "SNetwork.h"
#include "SLogger.h"
#include "SPacketQueue.h"

SNetwork::SNetwork(void)
{
}

SNetwork::~SNetwork(void)
{
	Release();

	//쓰레드가 죽길 기다린다.
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
	//연결되어 있으면 TRUE
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
	//우선 연결 끊고
	DisConnect();

	//소켓 만들고
	Init();

	//SetEvent( m_hStartEvent );

	//다시 연결
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
					//패킷을 패킷큐에 넣어 둔다.
					GetPacketQ.PutPacket( m_recvPack.GetDataBufferPtr(), m_recvPack.GetPacketSize() );

					//받아 처리한 만큼의 크기를 빼주고
					recvSize -= m_recvPack.GetPacketSize();

					//혹시 받은 패킷이 한개 이상이라면 또 확인해야 한다
					if( recvSize > 0 )
					{
						//아직 받아야 할게 남았다면 우선 buffer에 넣어 둔다
						CopyMemory( buffer, ( m_recvPack.GetDataBufferPtr()+m_recvPack.GetPacketSize() ), recvSize );
						tmpbuf += recvSize;
					}
					else
					{
						//다 받아서 새로 받아야 하니까 버퍼의 처음으로 돌아가자
						tmpbuf = &buffer[0];
					}
				}
				else
				{
					//그냥 아직 덜 받음...돌아가서 더 받자.
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
		//연결 끊기
		Release();
	}

	//잘 전송됬으면
	return retval;
}
