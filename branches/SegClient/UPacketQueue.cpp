#include "UPacketQueue.h"
#include "SPacket.h"

#ifdef SERVER_LOG_PRINT
	#include "SLogger.h"
#endif

CharData::CharData( int sessionId )
: m_sessionId( sessionId )
,m_dataTime(0)
{
	ZeroMemory( m_data, PACKETDATA_SIZE );
}

CharData::~CharData()
{
}

void CharData::SetData( char* data, int size )
{
	CopyMemory( m_data, data, size );
	m_isData = TRUE;
}

char* CharData::GetData()
{
	return m_data;
}

int CharData::GetSessionID()
{
	return m_sessionId;
}

void CharData::SetDataTime( int dwTime )
{
	m_dataTime = dwTime;
}

DWORD CharData::GetDataTime()
{
	return m_dataTime;
}

void CharData::SetNonData()
{
	m_isData = FALSE;
}

BOOL CharData::IsData()
{
	return m_isData;
}

CharData & CharData::operator = ( CharData &clSPacket )
{
	this->m_isData		= clSPacket.IsData();
	this->m_sessionId	= clSPacket.GetSessionID();
	CopyMemory( this->m_data, clSPacket.GetData(), PACKETDATA_SIZE );
	this->m_dataTime	= clSPacket.GetDataTime();

	return *this;
}

//==============================================================

UPacketQueue::UPacketQueue(void)
{
}

UPacketQueue::~UPacketQueue(void)
{
}

void UPacketQueue::Init()
{
}

void UPacketQueue::Release()
{
	SSynchronize sync( this );

	if( m_uPacketPool.IsEmpty() )
		return;

	//싹 지운다
	CharData* tmp;
	POSITION pos = m_uPacketPool.GetStartPosition();
	while( pos )
	{
		tmp = m_uPacketPool.GetNextValue( pos );
		delete tmp;
		tmp = 0;
	}
	m_uPacketPool.RemoveAll();
}

void UPacketQueue::AddItem( int session )
{
	SSynchronize sync( this );

	CharData* tmp = new CharData(session);
	tmp->SetNonData();

	m_uPacketPool[session] = tmp;
}

void UPacketQueue::DelItem( int session )
{
	SSynchronize sync( this );

	if( m_uPacketPool.IsEmpty() )
		return;

	CharData* tmp = NULL;
	m_uPacketPool.Lookup( session, tmp );
	if( tmp == NULL )
	{
#ifdef SERVER_LOG_PRINT
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("UPacketQueue::PutPacket()%d번 session의 공간이 없음\n\n"), session );
#endif
		return;
	}

	delete tmp;
	tmp = 0;
	m_uPacketPool.RemoveKey( session );

}

BOOL UPacketQueue::PutPacket( SPacket& packet )
{
	SSynchronize sync( this );

	int sessionId = packet.GetID();
	CharData* tmp = NULL;
	m_uPacketPool.Lookup( sessionId, tmp );
	if( tmp == NULL )
	{
#ifdef SERVER_LOG_PRINT
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("UPacketQueue::PutPacket()%d번 session의 공간이 없음\n\n"), sessionId );
#endif
		return FALSE;
	}

	DWORD dataTime;
	packet >> dataTime;

	//이번에 넣으려는 데이터가 더 크거나 같은경우면 새로 데이터를 바꾼다
	if( dataTime >= tmp->GetDataTime() )
	{
		tmp->SetDataTime( dataTime );
		tmp->SetData( packet.GetDataBufferPtr(), packet.GetPacketSize() );
	}
	//데이터가 더 옛날꺼면 넣지 않아도 된다

	return TRUE;
}

POSITION UPacketQueue::GetPackPoolStartPos()
{
	return m_uPacketPool.GetStartPosition();
}

void UPacketQueue::NextPos( POSITION &pos, CharData** data )
{
	(*data) = m_uPacketPool.GetNextValue( pos );
}

// BOOL UPacketQueue::GetPacket( SPacket& packet )
// {
// 	//읽어갈 data가 없으면 하지 않음
// 	if( m_iDataCount <= 0 )
// 		return FALSE;
// 
// 	//패킷으로 복사
// 	packet.CopyToPacket( m_vecPacketPool[m_iReadPos], m_vecPacketSize[m_iReadPos] );
// 
// 	MoveReadPos();
// 
// 	return TRUE;
// }
// 
// void UPacketQueue::MoveReadPos()
// {
// 	//처리가 끝난 데이터를 줄여 주고
// 	--m_iDataCount;
// 
// 	// 읽을 위치를 옮겨 준다.
// 	if( ++m_iReadPos >= VECBUFFER_SIZE )
// 		m_iReadPos = 0;
// }
// 
// int UPacketQueue::GetDataCount()
// {
// 	return m_iDataCount;
// }

