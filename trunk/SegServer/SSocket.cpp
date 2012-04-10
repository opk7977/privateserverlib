#include "SSocket.h"

SSocket::SSocket(void) : m_socket(INVALID_SOCKET)
{
}

SSocket::~SSocket(void)
{
	//���� �ɶ��� release�� �� ����
	Release();
}

BOOL SSocket::Init()
{
	return TRUE;
}

BOOL SSocket::Run()
{
	return TRUE;
}

void SSocket::Release()
{
	//���Ͽ� ���� ���� �۾��� �� �ִ� �Լ� ȣ��
	CloseSocket();
}

BOOL SSocket::CreateSocket()
{
	if( m_socket != INVALID_SOCKET )
		return TRUE;

	WSAData wsaData;

	//�����ϸ� 0�� return�ϴ� �Ϳ� ����!!
	//�����ϸ� false return
	if( WSAStartup( MAKEWORD(2, 2), &wsaData ) != 0 )
		return FALSE;

	//���� ������ �ѹ� Ȯ���� �ش�.
	if( wsaData.wVersion != MAKEWORD(2, 2) )
	{
		//������ �ٸ��� false return
		WSACleanup();
		return FALSE;
	}

	//���� ����(TCP)
	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	//���� ������ ������ �־��ٸ� false return
	if( m_socket == INVALID_SOCKET )
	{
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

BOOL SSocket::SetScokReuseAddr()
{
	bool reuse = true;
	int retval;
	//ip������ ���� ������ �ش�!
	//������ �ʼ����� �����̴�.
	//�����ϸ� 0 return
	retval = setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );

	if( retval != 0 )
		return FALSE;

	return TRUE;
}

BOOL SSocket::SetNonBlkSock()
{
	//������ �ͺ� ������ ����
	unsigned long argp = 1;

	//������ �ͺ� ������ ��ȯ
	ioctlsocket( m_socket, FIONBIO, &argp );

	return TRUE;
}

BOOL SSocket::ConnectSock( char* ipAddr, int port )
{
	SOCKADDR_IN sockAddr;

	//���Ͽ� ���� ������ �����ϰ�
	ZeroMemory( &sockAddr, sizeof( sockAddr ) );
	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= inet_addr( ipAddr );
	sockAddr.sin_port			= htons( port );

	//�����Ѵ�.
	//if( connect( m_socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR) ) == SOCKET_ERROR )
	if( WSAConnect( m_socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR), NULL, NULL, NULL, NULL ) == SOCKET_ERROR )
	{
		//�񵿱� ������ ���ӽõ��� �ٷ� �Ϸ� ���� �ʱ⶧���� SOCKET_ERROR��
		//return �� ���� �ֱ� ������ WSAEWOULDBLOCK�� Ȯ���� �־�� �Ѵ�.
		if( WSAGetLastError() != WSAEWOULDBLOCK )
			return FALSE;
	}

	return TRUE;
}

BOOL SSocket::BindSocket( int port )
{
	SOCKADDR_IN sockAddr;

	ZeroMemory( &sockAddr, sizeof( sockAddr ) );
	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= htonl( INADDR_ANY );
	sockAddr.sin_port			= htons( port );

	if( bind( m_socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR) ) == SOCKET_ERROR )
	{
		//bind�� �� �� �Ǿ��ٸ� �����ϰ� ���ư���
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL SSocket::SocketListen( int backlogCount )
{
	if( listen( m_socket, backlogCount ) == SOCKET_ERROR )
	{
		//listen�� �� �� �Ǿ��ٸ� �����ϰ� ���ư���
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL SSocket::CloseSocket()
{
	//������ �Ҵ�Ǿ� ���� �ʴٸ� �ʱ�ȭ�� �Ǿ� ���� �������̴�
	//�׳� ���ư��� ��
	if( m_socket == INVALID_SOCKET )
		return TRUE;

	closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	WSACleanup();

	return TRUE;
}
