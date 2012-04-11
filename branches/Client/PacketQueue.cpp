#include "StdAfx.h"
#include "PacketQueue.h"

PacketQueue::PacketQueue(void) : m_iReadPos(0)
							   , m_iWritePos(0)
							   , m_iDataCount(0)
{
	//���� �Ҵ��� �ְ�
	m_vecPacketPool.resize( VECBUFFER_SIZE );

	//������ ������ ���۸� �Ҵ��� �ش�
	for( int i=0; i<VECBUFFER_SIZE; ++i )
	{
		m_vecPacketPool[i] = new char[PACKETDATA_SIZE];
	}
}

PacketQueue::~PacketQueue(void)
{
	for( int i=0; i<VECBUFFER_SIZE; ++i )
	{
		delete m_vecPacketPool[i];
		m_vecPacketPool[i] = 0;
	}

	m_vecPacketPool.clear();
}

BOOL PacketQueue::PutPacket( char* buf, int dataSize )
{
	//�� á���� ���� �׸�..
	if( m_iDataCount >= VECBUFFER_SIZE )
		return FALSE;

	//���ڼ��� �ʹ� ���Ƶ� �ȵ�
	if( dataSize >= PACKETDATA_SIZE )
		return FALSE;

	//�����͸� ��´�.
	CopyMemory( m_vecPacketPool[m_iWritePos], buf, dataSize );

	//�־��� data�� ������ ������Ų��.
	++m_iDataCount;

	//����ϴ���ġ�� �Ű� �ش�.
	if( ++m_iWritePos >= VECBUFFER_SIZE )
		m_iWritePos = 0;			//���� �ѱ�� �Ǹ� 0����..

	return TRUE;
}

char* PacketQueue::GetPacket()
{
	//�о data�� ������ ���� ����
	if( m_iDataCount <= 0 )
		return NULL;

	return m_vecPacketPool[m_iReadPos];
}

void PacketQueue::MoveReadPos()
{
	//ó���� ���� �����͸� �ٿ� �ְ�
	--m_iDataCount;

	// ���� ��ġ�� �Ű� �ش�.
	if( ++m_iReadPos >= VECBUFFER_SIZE )
		m_iReadPos = 0;
}

int PacketQueue::GetDataCount()
{
	return m_iDataCount;
}
