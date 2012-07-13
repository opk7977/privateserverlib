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

	//�� �����
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("UPacketQueue::PutPacket()%d�� session�� ������ ����\n\n"), session );
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("UPacketQueue::PutPacket()%d�� session�� ������ ����\n\n"), sessionId );
#endif
		return FALSE;
	}

	DWORD dataTime;
	packet >> dataTime;

	//�̹��� �������� �����Ͱ� �� ũ�ų� �������� ���� �����͸� �ٲ۴�
	if( dataTime >= tmp->GetDataTime() )
	{
		tmp->SetDataTime( dataTime );
		tmp->SetData( packet.GetDataBufferPtr(), packet.GetPacketSize() );
	}
	//�����Ͱ� �� �������� ���� �ʾƵ� �ȴ�

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
// 	//�о data�� ������ ���� ����
// 	if( m_iDataCount <= 0 )
// 		return FALSE;
// 
// 	//��Ŷ���� ����
// 	packet.CopyToPacket( m_vecPacketPool[m_iReadPos], m_vecPacketSize[m_iReadPos] );
// 
// 	MoveReadPos();
// 
// 	return TRUE;
// }
// 
// void UPacketQueue::MoveReadPos()
// {
// 	//ó���� ���� �����͸� �ٿ� �ְ�
// 	--m_iDataCount;
// 
// 	// ���� ��ġ�� �Ű� �ش�.
// 	if( ++m_iReadPos >= VECBUFFER_SIZE )
// 		m_iReadPos = 0;
// }
// 
// int UPacketQueue::GetDataCount()
// {
// 	return m_iDataCount;
// }

