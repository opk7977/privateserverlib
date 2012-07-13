#include "TmpPacketQueue.h"
#include "SLogger.h"

TmpPacketQueue::TmpPacketQueue(void)
{
}

TmpPacketQueue::~TmpPacketQueue(void)
{
}

void TmpPacketQueue::Init( int poolsize )
{
	m_iReadPos = 0;
	m_iWritePos = 0;
	m_iDataCount = 0;

	m_iQSize = poolsize;

	//공간 할당
	m_vecPacPool.resize( m_iQSize );
	m_vecPacketSize.resize( m_iQSize );

	//각 공간에 버퍼 할당
	for( int i=0; i<m_iQSize; ++i )
		m_vecPacPool[i] = new char[PACKETDATA_SIZE];
}

void TmpPacketQueue::Release()
{
	for( int i=0; i<m_iQSize; ++i )
	{
		delete m_vecPacPool[i];
		m_vecPacPool[i] = 0;
	}

	m_vecPacPool.clear();
	m_vecPacketSize.clear();
}

BOOL TmpPacketQueue::PutPacket( char* buf, int dataSize )
{
	//꽉 찼으면 빠이
	if( m_iDataCount >= PACKETPOOL_SIZE )
		return FALSE;

	//데이터 크기가 너무 커도 안됨
	if( dataSize >= PACKETDATA_SIZE )
		return FALSE;

	//데이터 넣고
	CopyMemory( m_vecPacPool[m_iWritePos], buf, dataSize );
	//데이터 크기를 받아 놓는다
	m_vecPacketSize[m_iWritePos] = dataSize;

	//넣어진 data의 개수를 증가시킨다
	++m_iDataCount;

	//써야 하는 위치를 이제 옮겨 준다
	if( ++m_iWritePos >= PACKETPOOL_SIZE )
		m_iWritePos = 0;			//끝을 넘기게 되면 다시 처음으로

	return TRUE;
}

BOOL TmpPacketQueue::GetPacket( SPacket& packet )
{
	//읽어갈 data가 없으면 빠이
	if( m_iDataCount <= 0 )
		return FALSE;

	//패킷으로 복사
	packet.CopyToPacket( m_vecPacPool[m_iReadPos], m_vecPacketSize[m_iReadPos] );

	//위치 옮겨 주기
	MoveReadPos();

	return TRUE;
}

void TmpPacketQueue::MoveReadPos()
{
	//처리가 끝난 데이터를 줄여 주고
	--m_iDataCount;

	//읽을 위치를 옮겨 준다
	if( ++m_iReadPos >= PACKETPOOL_SIZE )
		m_iReadPos = 0;
}

int TmpPacketQueue::GetDataCount()
{
	return m_iDataCount;
}

//==============================================================

TmpPacketParser::TmpPacketParser()
{
	m_packetQ	= &GetPrePacketQ;
	m_logger	= &GetLogger;
}

TmpPacketParser::~TmpPacketParser()
{
}

void TmpPacketParser::Init()
{
}

void TmpPacketParser::Release()
{
}

void TmpPacketParser::PacketProcess()
{
	int count = m_packetQ->GetDataCount();

	if( count <= 0 )
		return;

	for( int i=0; i<count; ++i )
	{
		m_packetQ->GetPacket( m_packet );

		//처리
		PacketParsing();
	}
}

void TmpPacketParser::PacketParsing()
{
	if( m_packet.GetID() != 0 )
		return;			//ID가 다름..

	//패킷 처리합시다..
	int		size = 0;
	TCHAR	srvName[32]={0,};
	char	srvIP[16]={0,};

	m_packet >> size;
	m_packet.GetData( srvName, size );
	m_packet >> size;
	m_packet.GetData( srvIP, size );

	// TODO: 서버 이름과 ip를 받아 왔음
	//OutputDebugString( srvName );
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("name: %s\n"), srvName );
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, "ip: %s\n", srvIP );
}









