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

	//�̼��ǿ� ���� WSARecv�� ���ʷ� ȣ���� �ش�.
	WaitForPacketReceive();

	//log�� ���� ���///
	SOCKADDR_IN	sockAddr;
	int size = sizeof( sockAddr );
	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
		//ip����
		CopyMemory( m_IPAddr, inet_ntoa(sockAddr.sin_addr), strlen( inet_ntoa(sockAddr.sin_addr) ) );
		MultiByteToWideChar( CP_ACP, 0, m_IPAddr, strlen(m_IPAddr), m_tIPAddr, 20 );
		
		//log��///////////////////////////////////////////////////////////
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
		//log��///////////////////////////////////////////////////////////
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::OnDestroy()] Disconnect Client_IP: %s_Port: %d\n",
			inet_ntoa( sockAddr.sin_addr ), ntohs( sockAddr.sin_port ) );
#endif
	}

	//�ش� ������ ������ �ݾ� �ش�
	closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	m_iRecvSize = 0;
}

void SSession::Dispatch( DWORD bytesTransferred, OVERLAPPED* ov )
{
	CUSTOMOVERLAPPED* overlaped = (CUSTOMOVERLAPPED*)ov;

	if( bytesTransferred == 0 )
		return;

	//����� ��Ŷ�� �����Ŷ�� ó���ؾ� ��
	if( overlaped->flag == ASYNCFLAG_RECEIVE )
		DispatchReceive( bytesTransferred );
}

void SSession::DispatchReceive( DWORD bytesTransfer )
{
	SOCKADDR_IN sockAddr;
	int size = sizeof( sockAddr );
	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
		//log��///////////////////////////////////////////////////////////
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::DispatchReceive()] Reciev Packet From IP: %s / bytes: %d\n",
			inet_ntoa( sockAddr.sin_addr ), bytesTransfer );
#endif
	}

	SPacket recvPacket;

	//���� ũ�⸦ �� ���� ũ�⿡ ���Ѵ�
	m_iRecvSize += bytesTransfer;

	while( m_iRecvSize > 0 )
	{
		//���� �����͸� ��Ŷ�� �־� �ش�.
		recvPacket.CopyToPacket( m_chRecvBuf, m_iRecvSize );

		//��Ŷ�� ������ �����Ͱ� �׿� �ְ�, �ϳ��� ��Ŷ�� �ش��ϴ� ũ���� �����͸� �޾Ҵٸ� ó������
		if( recvPacket.IsValidPacket() && m_iRecvSize >= recvPacket.GetPacketSize() )
		{
			//���� �͵� �°� ���� receiveũ�⵵ ������
			PacketParsing( recvPacket );

			//���� ��ŭ�� ũ�⸦ ���µ� �ְ� 0�� �ƴϸ� ���� �����͸� ���縦 ���� ���´�
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
			//���� ��Ŷ�� �ϼ����� ���ߴٸ� ������ �� ����
			break;
		}
	}
	//�ٽ� ���� �غ� �Ѵ�.
	WaitForPacketReceive();
}

int SSession::SendPacket( SPacket& packet, BOOL tudp /*= FALSE*/ )
{
	DWORD writtenBytes = 0;

	if( m_socket == INVALID_SOCKET )
		return 0;

	ZeroMemory( &m_OverlappedSend, sizeof( CUSTOMOVERLAPPED ) );
	m_OverlappedSend.flag = ASYNCFLAG_SEND;

	//�ӽ� ����ü�� ����� �����͸� ��´�
	WSABUF		tmpBuf;
	tmpBuf.len = packet.GetPacketSize();
	tmpBuf.buf = packet.GetDataBufferPtr();
	DWORD tmpFlag = 0;

	int retval;

	//UDP SEND���� Ȯ���ϰ�
	if( tudp == TRUE )
	{
		//UDP ����
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
		//TCP ����
		retval = WSASend( m_socket
						, &tmpBuf
						, 1
						, &writtenBytes
						, tmpFlag
						, (LPWSAOVERLAPPED)&m_OverlappedSend
						, NULL );
	}

	//�� �������� 0�� ��ȯ
	if( retval != 0 )
	{
		DWORD lastError = GetLastError();

		//"���� �� ������ ���� �����̰� ������ �����ߴ�" �� ��찡 �ƴϸ� ���� ����
		if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
		{
#ifdef _DEBUG
			GetLogger.ErrorLog( lastError, _T("[Session::SendPacket()] " ) );
#endif
			return 0;
		}
	}

	//���´ٴ� log�� ǥ��

	SOCKADDR_IN sockAddr;
	int size = sizeof( sockAddr );

	if( getpeername( m_socket, (SOCKADDR*)&sockAddr, &size ) != SOCKET_ERROR )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Session::SendPacket()] Completed Send Pakcet To IP: %s / socket: %d / bytes: %d\n\n",
			inet_ntoa( sockAddr.sin_addr ), m_socket, writtenBytes );
#endif
	}

	//�� ���������� ���۵� ũ�Ⱑ return�ȴ�
	return writtenBytes;
}

int SSession::SendPacket( unsigned short protocolID )
{
	//���� ��Ŷ�� �ϳ� ����
	SPacket sendPacket( protocolID );

	//tcp�� ��������
	return SendPacket( sendPacket );
}

SOCKET SSession::GetSocket()
{
	return m_socket;
}

BOOL SSession::IsDisconnected()
{
	//���� ������ ���´�
	WSANETWORKEVENTS netEvent;
	BOOL ret;

	//�̺�Ʈ �ڵ��� �̹� ���� ������ ���ص� ��
	if( m_hRecvEvent == INVALID_HANDLE_VALUE )
		return TRUE;

	ZeroMemory( &netEvent, sizeof( netEvent ) );
	WSAEventSelect( GetSocket(), m_hRecvEvent, FD_READ | FD_CLOSE );
	WSAEnumNetworkEvents( GetSocket(), m_hRecvEvent, &netEvent );

	//������ �Ѵٸ� TRUE�� return�ϰ���..
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

	//�����͸� ��ƿ� �ӽ� ����ü ����
	WSABUF		tmpBuf;
	tmpBuf.len = bufSize;
	tmpBuf.buf = &m_chRecvBuf[m_iRecvSize];
	//WSARecv�� �� flag��...LPDWORD�̱� ������ ���� �־�� �Ѵ�
	DWORD		tmpFlag = 0;

	//���� �����ϸ� 0�� return�Ѵ�
	int retval = WSARecv( m_socket, &tmpBuf, 1, (LPDWORD)&readBytes, &tmpFlag, (LPWSAOVERLAPPED)&m_OverlappedRecv, NULL );

	//Ȯ��!!
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

