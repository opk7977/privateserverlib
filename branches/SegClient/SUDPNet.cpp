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
	//���� �ʱ�ȭ
	if( !m_conSock.Init() )
		return FALSE;

	if( !m_conSock.CreateUDPSock( INADDR_ANY, port ) )
		return FALSE;

	//���� �ͺ�
	if( !m_conSock.SetNonBlkSock() )
		return FALSE;

	//�ε��� ť �غ�
	//0������ 8���� index�� �غ�
	m_clientIndexQ.Create( 8, 0 );

	//client������ ����� ����
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

	//�ش� ������ �޾� ����
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

	//index��� �ݳ�
	std::list<SockAddr*>::iterator iter = m_clientList.GetHeader();
	for( ; m_clientList.IsEnd( iter ); ++iter )
	{
		m_clientIndexQ.PutIndex( (*iter)->GetIndex() );
	}

	m_clientList.Clear();

	for( int i=0; i<8; ++i )
		m_clientSpace[i]->Clear();

	//���� �ʱ�ȭ
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
		//���� �������� ���� ����
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
					//�ٽ� ������ ���� ��� ���� ���°� �ƴϸ� ��¥ ����
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
					//��Ŷ�� ��Ŷť�� �־� �д�.
					GetPacketQ.PutPacket( m_recvPack.GetDataBufferPtr(), m_recvPack.GetPacketSize() );

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

void SUDPNet::SendPacketAllClient( SPacket& packet )
{
	
}
