#include "UDPSender.h"
#include "SSynchronize.h"
#include "SPacket.h"

#include "DataLeader.h"

UDPSender::UDPSender(void)
: m_startThread( FALSE )
, m_srvNameSize(0)
{
	ZeroMemory( m_serverName, SERVER_NICNAME_LENTH );

	m_document = &GetDocument;
}

UDPSender::~UDPSender(void)
{
}

BOOL UDPSender::Init( TCHAR* serverName, char* serverIp, int PortNum )
{
	//======================================
	// 서버 이름을 저장
	//======================================
	m_srvNameSize = _tcslen( serverName )*sizeof(TCHAR);
	if( m_srvNameSize > SERVER_NICNAME_LENTH )
		m_srvNameSize = SERVER_NICNAME_LENTH;
	CopyMemory( m_serverName, serverName, m_srvNameSize );

	//======================================
	// ip와 portNum를 저장
	//======================================
	CopyMemory( m_serverIp, serverIp, strlen(serverIp) );
	m_portNum = PortNum;

	//======================================
	// 소켓 설정
	//======================================
	if( !m_sock.Init() )
		return FALSE;

	if( !m_sock.CreateUDPSock() )
		return FALSE;

	if( !m_sock.SetNonBlkSock() )
		return FALSE;

	//======================================
	// 브로드캐스트 옵션 설정
	//======================================
	BOOL bEnable = TRUE;
	int retval = setsockopt( m_sock.GetSocket(), SOL_SOCKET, SO_BROADCAST, (char *)&bEnable, sizeof(bEnable) );
	if( retval < 0 )
	{
		retval = WSAGetLastError();
		return FALSE;
	}

	//======================================
	// 시간 초기화
	//======================================
	m_timer.Init();

	//======================================
	// 시작
	//======================================
	BeginThread();
	m_startThread = TRUE;

	return TRUE;
}

void UDPSender::Release()
{
	SSynchronize sync( this );

	EndThread();
	m_sock.Release();
}

BOOL UDPSender::Run()
{
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(m_portNum);
	remoteaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	SPacket sendPacket( 0 );
	sendPacket << m_srvNameSize;
	sendPacket.PutData( m_serverName, m_srvNameSize );
	sendPacket << (int)sizeof( m_serverIp );
	sendPacket.PutData( m_serverIp, sizeof( m_serverIp ) );

	float onetime = 0.0f;
	int retval;

	while( m_startThread )
	{
		m_timer.ProcessTime();

		onetime += m_timer.GetElapsedTime();
		if( onetime >= 1.0f )
		{
			onetime = 0.0f;

			retval = sendto( m_sock.GetSocket(), sendPacket.GetDataBufferPtr(), sendPacket.GetPacketSize(),
				0, (SOCKADDR *)&remoteaddr, sizeof(remoteaddr) );

			if(retval == SOCKET_ERROR)
			{
				int a = WSAGetLastError();
				return FALSE;
			}
		}
	}

	return TRUE;
}

void UDPSender::EndThread()
{
	m_startThread = FALSE;

	SThread::EndThread();
}
