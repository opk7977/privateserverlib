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
	//�ʱ�ȭ
	if( !m_sock.Init() )
		return FALSE;

	//���� ����
	if( !m_sock.CreateUDPSock() )
		return FALSE;

	//���ε�
	if( !m_sock.BindSocket( PRE_UDP_PORTNUM ) )
		return FALSE;

	//���� �ͺ�
	if( !m_sock.SetNonBlkSock() )
		return FALSE;

	//��Ŷ ���� ���� ����� ����
	m_packetQ->Init( 30 );

	//======================================
	// ����
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
					//��Ŷ�� ��Ŷť�� �־� �д�.
					m_packetQ->PutPacket( m_recvPack.GetDataBufferPtr(), m_recvPack.GetPacketSize() );

					//�޾� ó���� ��ŭ�� ũ�⸦ ���ְ�
					recvSize -= m_recvPack.GetPacketSize();

					//Ȥ�� ���� ��Ŷ�� �Ѱ� �̻��̶�� �� Ȯ���ؾ� �Ѵ�
					if( recvSize > 0 )
					{
						//���� �޾ƾ� �Ұ� ���Ҵٸ� �켱 buffer�� �־� �д�
						CopyMemory( buffer, ( m_recvPack.GetDataBufferPtr()+m_recvPack.GetPacketSize() ), recvSize );
						tmpbuf += recvSize;
					}
					else
					{
						//�� �޾Ƽ� ���� �޾ƾ� �ϴϱ� ������ ó������ ���ư���
						tmpbuf = &buffer[0];
					}
				}
				else
				{
					//�׳� ���� �� ����...���ư��� �� ����.
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
