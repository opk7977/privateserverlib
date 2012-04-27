#include "SPacketQueue.h"
#include "SPacket.h"

SPacketQueue::SPacketQueue(void)
: m_iReadPos(0)
, m_iWritePos(0)
, m_iDataCount(0)
{
	//���� �Ҵ� �� �ش�
	m_vecPacketPool.resize( VECBUFFER_SIZE );
	m_vecPacketSize.resize( VECBUFFER_SIZE );

	//�� ������ ���� �Ҵ�
	for( int i=0; i<VECBUFFER_SIZE; ++i )
		m_vecPacketPool[i] = new char[PACKETDATA_SIZE];
}

SPacketQueue::~SPacketQueue(void)
{
	for( int i=0; i<VECBUFFER_SIZE; ++i )
	{
		delete m_vecPacketPool[i];
		m_vecPacketPool[i] = 0;
	}

	m_vecPacketPool.clear();
	m_vecPacketSize.clear();
}

BOOL SPacketQueue::PutPacket( char* buf, int dataSize )
{
	//�� á���� ���� �׸�..
	if( m_iDataCount >= VECBUFFER_SIZE )
		return FALSE;

	//���ڼ��� �ʹ� ���Ƶ� �ȵ�
	if( dataSize >= PACKETDATA_SIZE )
		return FALSE;

	//�����͸� ��´�.
	CopyMemory( m_vecPacketPool[m_iWritePos], buf, dataSize );
	//������ ũ�⸦ �޾� ���´�.
	m_vecPacketSize[m_iWritePos] = dataSize;

	//�־��� data�� ������ ������Ų��.
	++m_iDataCount;

	//����ϴ���ġ�� �Ű� �ش�.
	if( ++m_iWritePos >= VECBUFFER_SIZE )
		m_iWritePos = 0;			//���� �ѱ�� �Ǹ� 0����..

	return TRUE;
}

BOOL SPacketQueue::GetPacket( SPacket& packet )
{
	//�о data�� ������ ���� ����
	if( m_iDataCount <= 0 )
		return FALSE;

	//��Ŷ���� ����
	packet.CopyToPacket( m_vecPacketPool[m_iReadPos], m_vecPacketSize[m_iReadPos] );

	return TRUE;
}

void SPacketQueue::MoveReadPos()
{
	//ó���� ���� �����͸� �ٿ� �ְ�
	--m_iDataCount;

	// ���� ��ġ�� �Ű� �ش�.
	if( ++m_iReadPos >= VECBUFFER_SIZE )
		m_iReadPos = 0;
}

int SPacketQueue::GetDataCount()
{
	return m_iDataCount;
}
