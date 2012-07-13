#include "SUDPNet.h"
#include "UPacketQueue.h"

#ifdef SERVER_LOG_PRINT
	#include "SLogger.h"
#endif

SUDPNet::SUDPNet(void)
: m_gameStart(FALSE)
, m_isConnect(FALSE)
{
	m_packetQ = &GetUPacketQ;
}

SUDPNet::~SUDPNet(void)
{
}

BOOL SUDPNet::Init( int port )
{
	//이미 연결되어 있으면 안됨
	if( m_isConnect )
		return FALSE;

	//소켓 초기화
	if( !m_conSock.Init() )
		return FALSE;

	if( !m_conSock.CreateUDPSock() )
		return FALSE;

	//바인드
	if( !m_conSock.BindSocket( port ) )
		return FALSE;

	//소켓 넌블럭
	if( !m_conSock.SetNonBlkSock() )
		return FALSE;

	//인덱스 큐 준비
	//0번부터 8개의 index를 준비
	m_clientIndexQ.Create( 8, 0 );

	//client공간을 만들어 놓자
	for( int i=0; i<8; ++i )
	{
		SockAddr* tmpAddr = new SockAddr(i);
		m_clientSpace.SetAt( i, tmpAddr );
	}

	//연결합니다//
	
	if( !m_isConnect )
		BeginThread();

	m_gameStart = TRUE;

	return TRUE;
}

void SUDPNet::Release()
{
	SSynchronize Sync( this );

	m_gameStart = FALSE;
	m_conSock.Release();
	ClientClear();
}

BOOL SUDPNet::AddSockAddr( int sessionId, char* _ip, int port )
{

	SSynchronize Sync( this );

	int index = m_clientIndexQ.GetIndex();
	if( index < 0 )
		return FALSE;

	//해당 공간을 받아 오고
	SockAddr* tmpAddr = m_clientSpace[index];
	tmpAddr->m_sockAddr.sin_family		= AF_INET;
	tmpAddr->m_sockAddr.sin_addr.s_addr = inet_addr( _ip );
	tmpAddr->m_sockAddr.sin_port		= htons( port );

	//세션 id추가
	tmpAddr->SetSessionID( sessionId );

	m_clientList.AddItem( tmpAddr );

	return TRUE;
}

BOOL SUDPNet::DelSockAddr( int sessionId )
{
	if( m_clientList.IsEmpty() )
		return FALSE;
	
	std::list<SockAddr*>::iterator iter = m_clientList.GetHeader();
	int index;
	for( ; !m_clientList.IsEnd( iter ) ; ++iter )
	{
		if( (*iter)->GetSessionID() == sessionId )
		{
			index = (*iter)->GetIndex();
			break;
		}
	}

	m_clientList.DelItem( (*iter) );
	m_clientSpace[index]->Clear();
	m_clientIndexQ.PutIndex( index );

	return TRUE;
}

void SUDPNet::ClientClear()
{
	SSynchronize Sync( this );

	//index모두 반납
	if( m_clientList.IsEmpty() )
		return;

	std::list<SockAddr*>::iterator iter = m_clientList.GetHeader();
	for( ; !m_clientList.IsEnd( iter ); ++iter )
	{
		m_clientIndexQ.PutIndex( (*iter)->GetIndex() );
	}

	m_clientList.Clear();

	for( int i=0; i<8; ++i )
		m_clientSpace[i]->Clear();

	//u패킷 큐 초기화
	m_packetQ->Release();
}

BOOL SUDPNet::Run()
{
	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	WSABUF		tmpWSABuf;
	DWORD		tmpFlag = 0;
	DWORD		readBytes = 0;

	sockaddr_in recvSockAddr;
	int			sockAddrSize = sizeof( recvSockAddr );

	m_isConnect = TRUE;

	while(1)
	{
		//연결 끊었으면 받지 말자
		if( !m_gameStart )
			continue;

		Sleep(0);

		tmpWSABuf.len = tmpbufSize;
		tmpWSABuf.buf = tmpbuf;

// 		retval = WSARecvFrom( m_conSock.GetSocket()
// 							, &tmpWSABuf
// 							, 1
// 							, (LPDWORD)&readBytes
// 							, &tmpFlag
// 							, (SOCKADDR*)&recvSockAddr
// 							, &sockAddrSize
// 							, NULL, NULL );
		retval = recvfrom( m_conSock.GetSocket()
							, tmpbuf
							, tmpbufSize
							, 0
							, (SOCKADDR*)&recvSockAddr
							, &sockAddrSize );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
				if( !m_isConnect )
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
					//GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("SUDPNet::Run()패킷 받음\n\n") );
					m_packetQ->PutPacket( m_recvPack );

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

void SUDPNet::SendPacketAllClient( SPacket& packet )
{
	SSynchronize Sync( this );

	//임시 구조체
	WSABUF		tmpBuf;
	tmpBuf.len = packet.GetPacketSize();
	tmpBuf.buf = packet.GetDataBufferPtr();
	DWORD writtenBytes = 0;
	DWORD tmpFlag = 0;

	int retval;

	std::list<SockAddr*>::iterator iter = m_clientList.GetHeader();
	for( ; !m_clientList.IsEnd( iter ); ++iter )
	{
// 		retval = WSASendTo( m_conSock.GetSocket()
// 							, &tmpBuf
// 							, 1
// 							, &writtenBytes
// 							, tmpFlag
// 							, (sockaddr*)&((*iter)->m_sockAddr)
// 							, sizeof(sockaddr)
// 							, NULL
// 							, NULL );

		retval = sendto( m_conSock.GetSocket(), packet.GetDataBufferPtr(), packet.GetPacketSize(),
				0, (sockaddr*)&((*iter)->m_sockAddr), sizeof(sockaddr) );

		//잘 보내지면 0을 반환
		if( retval != 0 )
		{
			DWORD lastError = GetLastError();

			//"아직 다 보내지 못한 상태이고 보낸건 성공했다" 의 경우가 아니면 전송 오류
			if( lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS )
			{
#ifdef SERVER_LOG_PRINT
				GetLogger.ErrorLog( lastError, _T("[Session::SendPacket()] " ) );
#endif
			}
		}
	}
}
