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

	//패킷 큐를 셋팅!
	m_packetQ = &GetPacketQ;
	//m_packetQ->Init();
}

SNetwork::~SNetwork(void)
{
	Release();

	//쓰레드가 죽길 기다린다.
	WaitForSingleObject( GetThreadHandle(), 1000 );
}

BOOL SNetwork::Init()
{
	//패킷 큐 초기화
	m_packetQ->Init();

	//초기화
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
// 	//정보를 받아 놓는다
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

	//우선 연결 끊고
	DisConnect();

	//다시 연결
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
					//다시 연결을 위해 잠시 끊긴 상태가 아니면 진짜 오류
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

BOOL SNetwork::PilePacket( SPacket* packet )
{
	int size = packet->GetPacketSize();

	//크기를 확인한다
	if( m_sendSize + size > DEFAULT_BUFFER_SIZE )
	{
		//더 커지면 우선 보내고 보자
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
		//연결 끊기
		Release();
		return FALSE;
	}

	//잘 전송됬으면
	return TRUE;
}

BOOL SNetwork::SendPacket()
{
	//연결되어 있지 않으면 전송하지 않는다
	if( !m_isConnect )
		return TRUE;

	//전송할 데이터가 없으면 전송하지 않는다
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
		//연결 끊기
		Release();
		return FALSE;
	}
	if( retval >= 20 )
		int a = 1000;

	//잘 전송됬으면
	m_sendSize = 0;
	return TRUE;
}
