#include "SSession.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

SSession::SSession(void)
: m_iRecvSize(0)
, m_hRecvEvent(INVALID_HANDLE_VALUE)
, m_socket(INVALID_SOCKET)
, m_mySessionID(0)
{
	ZeroMemory( m_IPAddr, 20 );
	ZeroMemory( m_tIPAddr, 40 );
}

SSession::~SSession(void)
{
	WSACloseEvent( m_hRecvEvent );

	if( m_socket != INVALID_SOCKET )
	{
		closesocket( m_socket );
		m_socket = INVALID_SOCKET;
	}
}

void SSession::OnCreate()
{
	if( m_socket == INVALID_SOCKET )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Session::OnCreate()] socket is INVALID_SOCKET\n") );
#endif
		return;
	}

	//이세션에 대한 WSARecv를 최초로 호출해 준다.
	WaitForPacketReceive();

	//log용 정보 출력///
	SOCKADDR_IN	sockAddr;
	int size = sizeof( sockAddr );
	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
		//ip저장
		CopyMemory( m_IPAddr, inet_ntoa(sockAddr.sin_addr), strlen( inet_ntoa(sockAddr.sin_addr) ) );
		MultiByteToWideChar( CP_ACP, 0, m_IPAddr, strlen(m_IPAddr), m_tIPAddr, 20 );
		
		//log용///////////////////////////////////////////////////////////
#ifdef _DEBUG
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::OnCreate()] Connect Client_IP: %s_Port: %d\n",
// 			inet_ntoa( sockAddr.sin_addr ), ntohs( sockAddr.sin_port ) );
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::OnCreate()] Connect Client_IP: %s_Port: %d\n",
			m_IPAddr, ntohs( sockAddr.sin_port ) );
#endif
	}

	m_bActive = true;
}

void SSession::OnDestroy()
{
	if( m_socket == INVALID_SOCKET )
		return;

	SOCKADDR_IN sockAddr;
	int size = sizeof( sockAddr );
	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
		//log용///////////////////////////////////////////////////////////
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::OnDestroy()] Disconnect Client_IP: %s_Port: %d\n",
			inet_ntoa( sockAddr.sin_addr ), ntohs( sockAddr.sin_port ) );
#endif
	}

	//해당 세션의 정보를 닫아 준다
	closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	m_iRecvSize = 0;
}

void SSession::Dispatch( DWORD bytesTransferred, OVERLAPPED* ov )
{
	CUSTOMOVERLAPPED* overlaped = (CUSTOMOVERLAPPED*)ov;

	if( bytesTransferred == 0 )
		return;

	//결과가 패킷을 받은거라면 처리해야 함
	if( overlaped->flag == ASYNCFLAG_RECEIVE )
		DispatchReceive( bytesTransferred );
}

void SSession::DispatchReceive( DWORD bytesTransfer )
{
	SOCKADDR_IN sockAddr;
	int size = sizeof( sockAddr );
	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
		//log용///////////////////////////////////////////////////////////
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::DispatchReceive()] Reciev Packet From IP: %s / bytes: %d\n",
			inet_ntoa( sockAddr.sin_addr ), bytesTransfer );
#endif
	}

	SPacket recvPacket;

	//받은 크기를 총 받은 크기에 더한다
	m_iRecvSize += bytesTransfer;

	while( m_iRecvSize > 0 )
	{
		//받은 데이터를 패킷에 넣어 준다.
		recvPacket.CopyToPacket( m_chRecvBuf, m_iRecvSize );

		//패킷에 적당히 데이터가 쌓여 있고, 하나의 패킷에 해당하는 크기의 데이터를 받았다면 처리하자
		if( recvPacket.IsValidPacket() && m_iRecvSize >= recvPacket.GetPacketSize() )
		{
			//받은 것도 맞고 받은 receive크기도 맞으면
			PacketParsing( recvPacket );

			//받은 만큼의 크기를 뺐는데 애가 0이 아니면 남은 데이터만 복사를 남겨 놓는다
			m_iRecvSize -= recvPacket.GetPacketSize();
			if( m_iRecvSize > 0 )
			{
				unsigned char buffer[DEFAULT_BUFFER_SIZE];
				CopyMemory( buffer, (m_chRecvBuf + recvPacket.GetPacketSize()), m_iRecvSize );
				CopyMemory( m_chRecvBuf, buffer, m_iRecvSize );
			}
		}
		else
		{
			//아직 패킷이 완성되지 못했다면 나가서 더 받자
			break;
		}
	}
	//다시 받을 준비를 한다.
	WaitForPacketReceive();
}

int SSession::SendPacket( SPacket& packet, BOOL tudp /*= FALSE*/ )
{
	DWORD writtenBytes = 0;

	if( m_socket == INVALID_SOCKET )
		return 0;

	ZeroMemory( &m_OverlappedSend, sizeof( CUSTOMOVERLAPPED ) );
	m_OverlappedSend.flag = ASYNCFLAG_SEND;

	//임시 구조체를 만들어 데이터를 담는다
	WSABUF		tmpBuf;
	tmpBuf.len = packet.GetPacketSize();
	tmpBuf.buf = packet.GetDataBufferPtr();
	DWORD tmpFlag = 0;

	int retval;

	//UDP SEND인지 확인하고
	if( tudp == TRUE )
	{
		//UDP 전송
		retval = WSASendTo( m_socket
						   , &tmpBuf
						   , 1
						   , &writtenBytes
						   , tmpFlag
						   , &m_sockAddr
						   , sizeof( SOCKADDR )
						   , (LPWSAOVERLAPPED)&m_OverlappedSend
						   , NULL );
	}
	else
	{
		//TCP 전송
		retval = WSASend( m_socket
						, &tmpBuf
						, 1
						, &writtenBytes
						, tmpFlag
						, (LPWSAOVERLAPPED)&m_OverlappedSend
						, NULL );
	}

	//잘 보내지면 0을 반환
	if( retval != 0 )
	{
		DWORD lastError = GetLastError();

		//"아직 다 보내지 못한 상태이고 보낸건 성공했다" 의 경우가 아니면 전송 오류
		if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
		{
#ifdef _DEBUG
			GetLogger.ErrorLog( lastError, _T("[Session::SendPacket()] " ) );
#endif
			return 0;
		}
	}

	//보냈다는 log를 표시

	SOCKADDR_IN sockAddr;
	int size = sizeof( sockAddr );

	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::SendPacket()] Completed Send Pakcet To IP: %s / socket: %d / bytes: %d\n\n",
			inet_ntoa( sockAddr.sin_addr ), m_socket, writtenBytes );
#endif
	}

	//다 성공했으면 전송된 크기가 return된다
	return writtenBytes;
}

int SSession::SendPacket( unsigned short protocolID )
{
	//보낼 패킷을 하나 만들어서
	SPacket sendPacket( protocolID );

	//tcp로 전송하자
	return SendPacket( sendPacket );
}

SOCKET SSession::GetSocket()
{
	return m_socket;
}

BOOL SSession::IsDisconnected()
{
	//서버 연결을 끊는다
	WSANETWORKEVENTS netEvent;
	BOOL ret;

	//이벤트 핸들이 이미 닫혀 있으면 안해도 됨
	if( m_hRecvEvent == INVALID_HANDLE_VALUE )
		return TRUE;

	ZeroMemory( &netEvent, sizeof( netEvent ) );
	WSAEventSelect( GetSocket(), m_hRecvEvent, FD_READ | FD_CLOSE );
	WSAEnumNetworkEvents( GetSocket(), m_hRecvEvent, &netEvent );

	//닫혀야 한다면 TRUE를 return하겠지..
	ret = ((netEvent.lNetworkEvents & FD_CLOSE ) == FD_CLOSE );

	return ret;
}

void SSession::SetSocket( SOCKET socket )
{
	m_socket = socket;
}

void SSession::SetSockAddr( SOCKADDR addr )
{
	m_sockAddr = addr;
}

BOOL SSession::WaitForPacketReceive()
{
	if( m_socket == INVALID_SOCKET )
		return FALSE;

	DWORD readBytes = 0;
	DWORD bufSize = DEFAULT_BUFFER_SIZE - m_iRecvSize;

	ZeroMemory( &m_OverlappedRecv, sizeof( CUSTOMOVERLAPPED ) );
	m_OverlappedRecv.flag = ASYNCFLAG_RECEIVE;

	//데이터를 담아올 임시 구조체 선언
	WSABUF		tmpBuf;
	tmpBuf.len = bufSize;
	tmpBuf.buf = &m_chRecvBuf[m_iRecvSize];
	//WSARecv에 들어갈 flag값...LPDWORD이기 때문에 만들어서 넣어야 한다
	DWORD		tmpFlag = 0;

	//전송 성공하면 0을 return한다
	int retval = WSARecv( m_socket, &tmpBuf, 1, (LPDWORD)&readBytes, &tmpFlag, (LPWSAOVERLAPPED)&m_OverlappedRecv, NULL );

	//확인!!
	if( retval != 0 )
	{
		DWORD lastError = GetLastError();

		if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
		{
#ifdef _DEBUG
			GetLogger.ErrorLog( lastError, _T("[Session::WaitForPacketReceive()] " ) );
#endif
			return FALSE;
		}
	}

	return TRUE;
}

