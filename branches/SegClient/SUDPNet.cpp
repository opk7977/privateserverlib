#include "SUDPNet.h"
#include "SPacketQueue.h"
#include "SLogger.h"

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

	if( !m_conSock.CreateUDPSock() )
		return FALSE;

	//���ε�
	if( !m_conSock.BindSocket( port ) )
		return FALSE;

	//���� �ͺ�
// 	if( !m_conSock.SetNonBlkSock() )
// 		return FALSE;

	//�ε��� ť �غ�
	//0������ 8���� index�� �غ�
	m_clientIndexQ.Create( 8, 0 );

	//client������ ����� ����
	for( int i=0; i<8; ++i )
	{
		SockAddr* tmpAddr = new SockAddr(i);
		m_clientSpace.SetAt( i, tmpAddr );
	}

	BeginThread();
	m_gameStart = TRUE;

	return TRUE;
}

BOOL SUDPNet::AddSockAddr( int sessionId, char* _ip, int port )
{

	SSynchronize Sync( this );

	int index = m_clientIndexQ.GetIndex();
	if( index < 0 )
		return FALSE;

	//�ش� ������ �޾� ����
	SockAddr* tmpAddr = m_clientSpace[index];
	tmpAddr->m_sockAddr.sin_family		= AF_INET;
	tmpAddr->m_sockAddr.sin_addr.s_addr = inet_addr( _ip );
	tmpAddr->m_sockAddr.sin_port		= htons( port );

	//���� id�߰�
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

		retval = recvfrom( m_conSock.GetSocket(), tmpbuf, tmpbufSize, 0, (SOCKADDR*)&recvSockAddr, &sockAddrSize );

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
	SSynchronize Sync( this );

	int retval;

	std::list<SockAddr*>::iterator iter = m_clientList.GetHeader();
	for( ; !m_clientList.IsEnd( iter ); ++iter )
	{
		retval = sendto( m_conSock.GetSocket(), packet.GetDataBufferPtr(), packet.GetPacketSize(),
				0, (sockaddr*)&((*iter)->m_sockAddr), sizeof(sockaddr) );

		if( retval == SOCKET_ERROR )
		{
			GetLogger.ErrorLog( WSAGetLastError(), _T("SUDPNet error: ") );
		}
	}
}
