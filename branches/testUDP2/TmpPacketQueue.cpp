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

	//���� �Ҵ�
	m_vecPacPool.resize( m_iQSize );
	m_vecPacketSize.resize( m_iQSize );

	//�� ������ ���� �Ҵ�
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
	//�� á���� ����
	if( m_iDataCount >= PACKETPOOL_SIZE )
		return FALSE;

	//������ ũ�Ⱑ �ʹ� Ŀ�� �ȵ�
	if( dataSize >= PACKETDATA_SIZE )
		return FALSE;

	//������ �ְ�
	CopyMemory( m_vecPacPool[m_iWritePos], buf, dataSize );
	//������ ũ�⸦ �޾� ���´�
	m_vecPacketSize[m_iWritePos] = dataSize;

	//�־��� data�� ������ ������Ų��
	++m_iDataCount;

	//��� �ϴ� ��ġ�� ���� �Ű� �ش�
	if( ++m_iWritePos >= PACKETPOOL_SIZE )
		m_iWritePos = 0;			//���� �ѱ�� �Ǹ� �ٽ� ó������

	return TRUE;
}

BOOL TmpPacketQueue::GetPacket( SPacket& packet )
{
	//�о data�� ������ ����
	if( m_iDataCount <= 0 )
		return FALSE;

	//��Ŷ���� ����
	packet.CopyToPacket( m_vecPacPool[m_iReadPos], m_vecPacketSize[m_iReadPos] );

	//��ġ �Ű� �ֱ�
	MoveReadPos();

	return TRUE;
}

void TmpPacketQueue::MoveReadPos()
{
	//ó���� ���� �����͸� �ٿ� �ְ�
	--m_iDataCount;

	//���� ��ġ�� �Ű� �ش�
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

		//ó��
		PacketParsing();
	}
}

void TmpPacketParser::PacketParsing()
{
	if( m_packet.GetID() != 0 )
		return;			//ID�� �ٸ�..

	//��Ŷ ó���սô�..
	int		size = 0;
	TCHAR	srvName[32]={0,};
	char	srvIP[16]={0,};

	m_packet >> size;
	m_packet.GetData( srvName, size );
	m_packet >> size;
	m_packet.GetData( srvIP, size );

	// TODO: ���� �̸��� ip�� �޾� ����
	//OutputDebugString( srvName );
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("name: %s\n"), srvName );
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, "ip: %s\n", srvIP );
}









