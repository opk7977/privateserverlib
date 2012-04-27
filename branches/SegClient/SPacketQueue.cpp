#include "SPacketQueue.h"
#include "SPacket.h"

SPacketQueue::SPacketQueue(void)
: m_iReadPos(0)
, m_iWritePos(0)
, m_iDataCount(0)
{
	//공간 할당 해 준다
	m_vecPacketPool.resize( VECBUFFER_SIZE );
	m_vecPacketSize.resize( VECBUFFER_SIZE );

	//각 공간에 버퍼 할당
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
	//꽉 찼으면 이제 그만..
	if( m_iDataCount >= VECBUFFER_SIZE )
		return FALSE;

	//글자수가 너무 많아도 안됨
	if( dataSize >= PACKETDATA_SIZE )
		return FALSE;

	//데이터를 담는다.
	CopyMemory( m_vecPacketPool[m_iWritePos], buf, dataSize );
	//데이터 크기를 받아 놓는다.
	m_vecPacketSize[m_iWritePos] = dataSize;

	//넣어진 data의 개수를 증가시킨다.
	++m_iDataCount;

	//써야하는위치를 옮겨 준다.
	if( ++m_iWritePos >= VECBUFFER_SIZE )
		m_iWritePos = 0;			//끝을 넘기게 되면 0으로..

	return TRUE;
}

BOOL SPacketQueue::GetPacket( SPacket& packet )
{
	//읽어갈 data가 없으면 하지 않음
	if( m_iDataCount <= 0 )
		return FALSE;

	//패킷으로 복사
	packet.CopyToPacket( m_vecPacketPool[m_iReadPos], m_vecPacketSize[m_iReadPos] );

	return TRUE;
}

void SPacketQueue::MoveReadPos()
{
	//처리가 끝난 데이터를 줄여 주고
	--m_iDataCount;

	// 읽을 위치를 옮겨 준다.
	if( ++m_iReadPos >= VECBUFFER_SIZE )
		m_iReadPos = 0;
}

int SPacketQueue::GetDataCount()
{
	return m_iDataCount;
}
