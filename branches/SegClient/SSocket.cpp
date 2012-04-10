#include "SSocket.h"

SSocket::SSocket(void) : m_socket(INVALID_SOCKET)
{
}

SSocket::~SSocket(void)
{
	//종료 될때는 release를 해 주자
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
	//소켓에 대한 해제 작업을 해 주는 함수 호출
	CloseSocket();
}

BOOL SSocket::CreateSocket()
{
	if( m_socket != INVALID_SOCKET )
		return TRUE;

	WSAData wsaData;

	//성공하면 0을 return하는 것에 주의!!
	//실패하면 false return
	if( WSAStartup( MAKEWORD(2, 2), &wsaData ) != 0 )
		return FALSE;

	//버전 정보를 한번 확인해 준다.
	if( wsaData.wVersion != MAKEWORD(2, 2) )
	{
		//버전이 다르면 false return
		WSACleanup();
		return FALSE;
	}

	//소켓 생성(TCP)
	m_socket = socket( AF_INET, SOCK_STREAM, 0 );

	//소켓 생성에 문제가 있었다면 false return
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
	//ip재사용을 위해 설정해 준다!
	//서버에 필수적인 설정이다.
	//실패하면 0 return
	retval = setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );

	if( retval != 0 )
		return FALSE;

	return TRUE;
}

BOOL SSocket::SetNonBlkSock()
{
	//소켓을 넌블럭 형으로 설정
	unsigned long argp = 1;

	//소켓을 넌블럭 형으로 변환
	ioctlsocket( m_socket, FIONBIO, &argp );

	return TRUE;
}

BOOL SSocket::ConnectSock( char* ipAddr, int port )
{
	SOCKADDR_IN sockAddr;

	//소켓에 대한 정보를 설정하고
	ZeroMemory( &sockAddr, sizeof( sockAddr ) );
	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= inet_addr( ipAddr );
	sockAddr.sin_port			= htons( port );

	//연결한다.
	//if( connect( m_socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR) ) == SOCKET_ERROR )
	if( WSAConnect( m_socket, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR), NULL, NULL, NULL, NULL ) == SOCKET_ERROR )
	{
		//비동기 소켓은 접속시도가 바로 완료 되지 않기때문에 SOCKET_ERROR를
		//return 할 수도 있기 때문에 WSAEWOULDBLOCK를 확인해 주어야 한다.
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
		//bind가 잘 못 되었다면 종료하고 돌아가자
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL SSocket::SocketListen( int backlogCount )
{
	if( listen( m_socket, backlogCount ) == SOCKET_ERROR )
	{
		//listen이 잘 못 되었다면 종료하고 돌아가자
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

BOOL SSocket::CloseSocket()
{
	//소켓이 할당되어 있지 않다면 초기화도 되어 있지 않은것이니
	//그냥 돌아가면 됨
	if( m_socket == INVALID_SOCKET )
		return TRUE;

	closesocket( m_socket );
	m_socket = INVALID_SOCKET;
	WSACleanup();

	return TRUE;
}
