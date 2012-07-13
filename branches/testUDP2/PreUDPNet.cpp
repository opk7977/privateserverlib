#include "PreUDPNet.h"
#include "SSynchronize.h"

#include "TmpPacketQueue.h"
#include "SLogger.h"

PreUDPNet::PreUDPNet(void)
: m_startThread(FALSE)
{
	m_packetQ = &GetPrePacketQ;
}

PreUDPNet::~PreUDPNet(void)
{
}

BOOL PreUDPNet::Init()
{
	//초기화
	if( !m_sock.Init() )
		return FALSE;

	//소켓 생성
	if( !m_sock.CreateUDPSock() )
		return FALSE;

	//바인드
	if( !m_sock.BindSocket( PRE_UDP_PORTNUM ) )
		return FALSE;

	//소켓 넌블럭
	if( !m_sock.SetNonBlkSock() )
		return FALSE;

	//패킷 넣을 공간 만들어 놓기
	m_packetQ->Init( 30 );

	//======================================
	// 시작
	//======================================
	BeginThread();
	m_startThread = TRUE;

	return TRUE;
}

void PreUDPNet::Release()
{
	SSynchronize sync( this );

	EndThread();
	m_sock.Release();

	m_packetQ->Release();
}

BOOL PreUDPNet::Run()
{
	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;

	int retval;
	INT32 errorCode;
	int recvSize = 0;

// 	WSABUF		tmpWSABuf;
// 	DWORD		tmpFlag = 0;
// 	DWORD		readBytes = 0;

	sockaddr_in recvSockAddr;
	int			sockAddrSize = sizeof( recvSockAddr );

	while( m_startThread )
	{
// 		tmpWSABuf.len = tmpbufSize;
// 		tmpWSABuf.buf = tmpbuf;

		// 		retval = WSARecvFrom( m_conSock.GetSocket()
		// 							, &tmpWSABuf
		// 							, 1
		// 							, (LPDWORD)&readBytes
		// 							, &tmpFlag
		// 							, (SOCKADDR*)&recvSockAddr
		// 							, &sockAddrSize
		// 							, NULL, NULL );
		retval = recvfrom( m_sock.GetSocket()
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
				GetLogger.ErrorLog( errorCode, _T("[SNetwork::Run()] ") );
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
					//패킷을 패킷큐에 넣어 둔다.
					m_packetQ->PutPacket( m_recvPack.GetDataBufferPtr(), m_recvPack.GetPacketSize() );

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

void PreUDPNet::EndThread()
{
	m_startThread = FALSE;
}
