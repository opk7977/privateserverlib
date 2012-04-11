#include "StdAfx.h"
#include "PacketQueue.h"

PacketQueue::PacketQueue(void) : m_iReadPos(0)
							   , m_iWritePos(0)
							   , m_iDataCount(0)
{
	//공간 할당해 주고
	m_vecPacketPool.resize( VECBUFFER_SIZE );

	//각각의 공간에 버퍼를 할당해 준다
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
	//꽉 찼으면 이제 그만..
	if( m_iDataCount >= VECBUFFER_SIZE )
		return FALSE;

	//글자수가 너무 많아도 안됨
	if( dataSize >= PACKETDATA_SIZE )
		return FALSE;

	//데이터를 담는다.
	CopyMemory( m_vecPacketPool[m_iWritePos], buf, dataSize );

	//넣어진 data의 개수를 증가시킨다.
	++m_iDataCount;

	//써야하는위치를 옮겨 준다.
	if( ++m_iWritePos >= VECBUFFER_SIZE )
		m_iWritePos = 0;			//끝을 넘기게 되면 0으로..

	return TRUE;
}

char* PacketQueue::GetPacket()
{
	//읽어갈 data가 없으면 하지 않음
	if( m_iDataCount <= 0 )
		return NULL;

	return m_vecPacketPool[m_iReadPos];
}

void PacketQueue::MoveReadPos()
{
	//처리가 끝난 데이터를 줄여 주고
	--m_iDataCount;

	// 읽을 위치를 옮겨 준다.
	if( ++m_iReadPos >= VECBUFFER_SIZE )
		m_iReadPos = 0;
}

int PacketQueue::GetDataCount()
{
	return m_iDataCount;
}
