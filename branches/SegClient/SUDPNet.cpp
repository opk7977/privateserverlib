#include "SUDPNet.h"
#include "SPacketQueue.h"

SUDPNet::SUDPNet(void)
: m_gameStart(FALSE)
{
	m_packetQ = &GetPacketQ;
}

SUDPNet::~SUDPNet(void)
{
}

BOOL SUDPNet::Init( int port )
{
	//소켓 초기화
	if( !m_conSock.Init() )
		return FALSE;

	if( !m_conSock.CreateUDPSock( INADDR_ANY, port ) )
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

	m_gameStart = TRUE;
}

BOOL SUDPNet::AddSockAddr( char* _ip, int port )
{

	SSynchronize Sync( this );

	int index = m_clientIndexQ.GetIndex();
	if( index <= 0 )
		return FALSE;

	//해당 공간을 받아 오고
	SockAddr* tmpAddr = m_clientSpace[index];
	tmpAddr.sin_family		= AF_INET;
	tmpAddr.sin_addr.S_addr = inet_addr( _ip );
	tmpAddr.sin_port		= htons( port );

	m_clientList.AddItem( tmpAddr );
}

void SUDPNet::ClientClear()
{
	SSynchronize Sync( this );

	m_gameStart = FALSE;

	//index모두 반납
	std::list<SockAddr*>::iterator iter = m_clientList.GetHeader();
	for( ; m_clientList.IsEnd( iter ); ++iter )
	{
		m_clientIndexQ.PutIndex( (*iter)->GetIndex() );
	}

	m_clientList.Clear();

	for( int i=0; i<8; ++i )
		m_clientSpace[i]->Clear();

	//소켓 초기화
	m_conSock.Release();
}

BOOL SUDPNet::Run()
{
	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	sockaddr_in recvSockAddr;
	int			sockAddrSize = sizeof( recvSockAddr );

	while(1)
	{
		//연결 끊었으면 받지 말자
		if( !m_gameStart )
			continue;

		retval = recvfrom( m_conSock.GetSocket(), tmpbuf, tmpbufSize, 0, (SOCKADDR*)&recvSockAddr, sockAddrSize );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
				if( !m_gameStart )
				{
					//다시 연결을 위해 잠시 끊긴 상태가 아니면 진짜 오류
					GetLogger.ErrorLog( errorCode, _T("[SNetwork::Run()] ") );
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

void SUDPNet::SendPacketAllClient( SPacket& packet )
{
	
}
