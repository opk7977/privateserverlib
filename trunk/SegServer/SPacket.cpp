#include "SPacket.h"

SPacket::SPacket(void) : m_chpBuffer(NULL)
					   , m_iReceivedSize(0)
					   , m_chpReadPos(NULL)
					   , m_chpWritePos(NULL)
{
	PacketClear();
}

SPacket::SPacket( unsigned short protocolid ) : m_chpBuffer(NULL)
											  , m_iReceivedSize(0)
											  , m_chpReadPos(NULL)
											  , m_chpWritePos(NULL)
{
	PacketClear();
	SetID( protocolid );
}

SPacket::SPacket( const SPacket &clSPacket ) : m_chpBuffer(NULL)
											 , m_iReceivedSize(0)
											 , m_chpReadPos(NULL)
											 , m_chpWritePos(NULL)
{
	PacketClear();

	//�޾ƿ� ��Ŷ�� �����͸� ������ ��Ŷ���� �����Ѵ�.
	CopyMemory( m_chDataBuffer, clSPacket.GetDataBufferPtr(), DEFAULT_BUFFER_SIZE );

	m_iReceivedSize = clSPacket.GetRecvSize();

	DWORD offset;

	//���� ��ġ�� �ּҿ� ������ ��ġ�� �ּҰ��� ���� ������ �ּ��� ũ�Ⱑ ���´�.
	offset = (DWORD)clSPacket.GetReadPos() - (DWORD)clSPacket.GetDataBufferPtr();
	//������ ���� ��ġ�� offset�� ���ϸ� ���� ũ���� �ּҸ�ŭ �̵��� �� �ִ�.
	m_chpReadPos += offset;

	//�� ��ġ�� ���������� ������� ���� ��ġ�� �̵��Ѵ�.
	offset = (DWORD)clSPacket.GetWritePos() - (DWORD)clSPacket.GetDataBufferPtr();
	m_chpWritePos += offset;
}

SPacket::~SPacket(void)
{
}

BOOL SPacket::IsValidHeader()
{
	//��Ŷ�� ũ�Ⱑ ����� ũ�⺸�� ũ���� ������ ����� �ִ°Ŵϱ�
	//TRUE�� return
	if( GetPacketSize() >= PACKET_HEADER_SIZE )
		return TRUE;

	return FALSE;
}

BOOL SPacket::IsValidPacket()
{
	//����� ����
	//������������ ũ�Ⱑ ��Ŷ�� ���ũ�⺸�� �۰�
	//������������ ũ�Ⱑ ��Ŷ�� �����ͺ��� ������
	//����� �� �ִ� ��Ŷ Ȥ�� �� �޾ƾ� �ϴ� ��Ŷ�̴�
	if( IsValidHeader() == FALSE || m_iReceivedSize < PACKET_HEADER_SIZE || m_iReceivedSize < GetDateFiledSize() )
		return FALSE;

	return TRUE;
}

void SPacket::SetID( unsigned short protocolID )
{
	//�������� id�Ҵ�(?)...����(?)
	*(m_packetHeader.m_sProtocolID) = protocolID;
}

unsigned short SPacket::GetID()
{
	//�������� id�� return
	return *(m_packetHeader.m_sProtocolID);
}

void SPacket::PacketClear()
{
	//��Ŷ�� ��� ������ �ʱ�ȭ ���� �ش�
	ZeroMemory( m_chDataBuffer, DEFAULT_BUFFER_SIZE );

	//������ ũ��� �������� ���� �ʱ�ȭ!!
	m_packetHeader.m_sProtocolID	= ( unsigned short* )m_chDataBuffer + 0;
	m_packetHeader.m_sDataSize		= ( unsigned short* )( ( char* )m_chDataBuffer + 2 );

	//���� �ּҰ� ��ġ�� ã�� ��
	m_chpBuffer			= &m_chDataBuffer[PACKET_HEADER_SIZE]; //�������� ���� ��ġ
	m_chpReadPos		= m_chpWritePos			= m_chpBuffer;
	m_chpEndofDataField = &m_chDataBuffer[DEFAULT_BUFFER_SIZE];

	SetID(0);

	m_iReceivedSize = 0;
}

int SPacket::MoveReadPos( int size )
{
	//�̵��� ũ�Ⱑ 0�� ���ų� ������ return
	if( size <= 0 )
		return 0;

	//�̵��� ��ġ�� ũ�⸦ ���ؼ� ������ ������ �Ѿ�� �ȴٸ� return
	if( m_chpReadPos + size > m_chpEndofDataField )
		return 0;

	//�д� ����Ʈ �̵�
	m_chpReadPos += size;

	//�̵���ũ�� return
	return size;
}

int SPacket::MoveWritePos( int size )
{
	//�̵��� ũ�Ⱑ 0�� ���ų� ������ return
	if( size <= 0 )
		return 0;

	//�̵��� ��ġ�� ũ�⸦ ���ؼ� ������ ������ �Ѿ�� �ȴٸ� return
	if( m_chpWritePos + size > m_chpEndofDataField )
		return 0;

	//���� ����Ʈ �̵�
	m_chpWritePos += size;

	//�̵���ũ�� return
	return size;
}

void SPacket::CopyToPacket( char* buf, int size )
{
	//�켱 ������ �� �ְ�
	PacketClear();
	CopyMemory( m_chDataBuffer, buf, size );
	m_iReceivedSize += size;
}

int SPacket::PutData( char* data, int size )
{
	//�� �����Ͱ� ������ ������ �Ѿ�� return
	if( m_chpWritePos + size >= m_chpEndofDataField )
		return 0;

	CopyMemory( m_chpWritePos, data, size );
	//������ ���� ��ŭ �� �ڸ��� �̵��� �ְ�
	m_chpWritePos += size;

	//���� �����;��� �����ְ�
	m_iReceivedSize += size;
	
	*(m_packetHeader).m_sDataSize += size;

	return size;
}

int SPacket::GetData( char* data, int size )
{
	//���� �����Ͱ� ���� ���� �������� ũ�⸦ �Ѿ�� return
	if( m_chpReadPos + size > m_chpBuffer + GetDateFiledSize() )
		return 0;

	//���� �����Ͱ� ������ ������ �Ѿ� ���� return
	if( m_chpReadPos + size >= m_chpEndofDataField )
		return 0;

	//ZeroMemory�� ���� ���ϴ� ������ �ٸ� �ڷ������� ������ �� �Լ��� ȣ���ϱ� ������
	// size�̻��� memory�� �ʱ�ȭ �� �� ����
	// char�� ���ڿ��� �޾ư��� ���ؼ� ó���� zeromemory�� �ϰ� �ѱ���
	CopyMemory( data, m_chpReadPos, size );
	m_chpReadPos += size;

	return size;
}

int SPacket::PutData( TCHAR* data, int size )
{
	//�����ڵ��� ���ڿ��� ����
	//��ȯ ���� �׳� �״�� �־ ���߿� ������ �׷��� ����.

	//�� �����Ͱ� ������ ������ �Ѿ�� return
	if( m_chpWritePos + size >= m_chpEndofDataField )
		return 0;

	//�޸𸮸� copy�ϴ°Ŵϱ�׳� ������ ��
	CopyMemory( m_chpWritePos, data, size );
	//����ŭ �̵��ϰ�
	m_chpWritePos += size;

	m_iReceivedSize += size;

	*(m_packetHeader).m_sDataSize += size;

	return size;
}

int SPacket::GetData( TCHAR* data, int size )
{
	//���� �����Ͱ� ���� ���� �������� ũ�⸦ �Ѿ�� return
	if( m_chpReadPos + size > m_chpBuffer + GetDateFiledSize() )
		return 0;

	//���� �����Ͱ� ������ ������ �Ѱ� ���� return
	if( m_chpReadPos + size >= m_chpEndofDataField )
		return 0;

	//���ڿ��� ���� �ڿ� null���� �ֱ� ���� ������ ���ڿ�+2��ŭ�� ������ 0���� �ʱ�ȭ ���ش�
	//�׷��� ���ϻ�� ����� ���ڿ� ���� ���ڰ� null���� ���� �Ǿ�
	//���ڿ��� �ϼ��ȴ�.
	ZeroMemory( data, size+sizeof(TCHAR) );
	CopyMemory( data, m_chpReadPos, size );
	m_chpReadPos += size;

	return size;
}

////////////////////////////////////////////////////////////
//	������ ���۷�����
////////////////////////////////////////////////////////////
//���� ������
SPacket	&SPacket::operator = ( SPacket &clSPacket )
{
	//�켱 ��Ŷ û���ϰ�
	PacketClear();

	//�׸��� ����
	*(m_packetHeader).m_sProtocolID = clSPacket.GetID();
	*(m_packetHeader).m_sDataSize = clSPacket.GetDateFiledSize();

	CopyMemory( m_chpBuffer, clSPacket.GetDataBufferPtr(), clSPacket.GetPacketSize() );

	m_iReceivedSize = clSPacket.GetRecvSize();

	DWORD offset;

	//���� ��ġ�� �ּҿ� ������ ��ġ�� �ּҰ��� ���� �������ּ��� ũ�Ⱑ ���´�.
	offset = (DWORD)clSPacket.GetReadPos() - (DWORD)clSPacket.GetDataBufferPtr();
	//������ ���� ��ġ�� offset�� ���ϸ� ���� ũ���� �ּҸ�ŭ �̵��� �� �ִ�.
	m_chpReadPos += offset;

	//write�� ��ġ�� ���������� ������� ���� ��ġ�� �̵��Ѵ�.
	offset = (DWORD)clSPacket.GetWritePos() - (DWORD)clSPacket.GetDataBufferPtr();
	m_chpWritePos += offset;

	return *this;
}

//�����͸� �ִ� ������
SPacket	&SPacket::operator << ( BYTE _value )
{
	PutData( reinterpret_cast<char *>(&_value), sizeof(BYTE) );
	return *this;
}
SPacket	&SPacket::operator << ( short _value )
{
	PutData( reinterpret_cast<char *>(&_value), sizeof(short) );
	return *this;
}
SPacket	&SPacket::operator << ( int _value )
{
	PutData( reinterpret_cast<char *>(&_value), sizeof(int) );
	return *this;
}
SPacket	&SPacket::operator << ( DWORD _value )
{
	PutData( reinterpret_cast<char *>(&_value), sizeof(DWORD) );
	return *this;
}
SPacket & SPacket::operator<<( float _value )
{
	PutData( reinterpret_cast<char *>(&_value), sizeof(float) );
	return *this;
}
SPacket &SPacket::operator << ( __int64 _value )
{
	PutData( reinterpret_cast<char *>(&_value), sizeof(__int64) );
	return *this;
}
SPacket &SPacket::operator << ( SPacket &_value )
{
	unsigned short _id = _value.GetID();
	unsigned short _size = _value.GetDateFiledSize();

	PutData( reinterpret_cast<char *>(&_id), sizeof( unsigned short ) );
	PutData( reinterpret_cast<char *>(&_size), sizeof( unsigned short ) );
	PutData( _value.GetDataBufferPtr(), _size );

	return *this;
}

//�����͸� �о���� ������
SPacket	&SPacket::operator >> ( BYTE &_value )
{
	GetData( reinterpret_cast<char *>(&_value), sizeof(BYTE) );
	return *this;
}
SPacket	&SPacket::operator >> ( short &_value )
{
	GetData( reinterpret_cast<char *>(&_value), sizeof(short) );
	return *this;
}
SPacket	&SPacket::operator >> ( int &_value )
{
	GetData( reinterpret_cast<char *>(&_value), sizeof(int) );
	return *this;
}
SPacket	&SPacket::operator >> ( DWORD &_value )
{
	GetData( reinterpret_cast<char *>(&_value), sizeof(DWORD) );
	return *this;
}
SPacket & SPacket::operator>>( float &_value )
{
	GetData( reinterpret_cast<char *>(&_value), sizeof(float) );
	return *this;
}
SPacket &SPacket::operator >> ( __int64 _value )
{
	GetData( reinterpret_cast<char *>(&_value), sizeof(__int64) );
	return *this;
}
SPacket &SPacket::operator >> ( SPacket &_value )
{
	unsigned short id=0, size=0;
	char buffer[DEFAULT_BUFFER_SIZE];

	*this >> id;
	*this >> size;

	GetData( buffer, size );

	_value.SetID( id );
	_value.PutData( buffer, size );

	return *this;
}