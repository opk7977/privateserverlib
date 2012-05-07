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

	//받아온 패킷의 데이터를 지금의 패킷으로 복사한다.
	CopyMemory( m_chDataBuffer, clSPacket.GetDataBufferPtr(), DEFAULT_BUFFER_SIZE );

	m_iReceivedSize = clSPacket.GetRecvSize();

	DWORD offset;

	//읽을 위치의 주소와 데이터 위치의 주소값을 뺴면 데이터 주소의 크기가 나온다.
	offset = (DWORD)clSPacket.GetReadPos() - (DWORD)clSPacket.GetDataBufferPtr();
	//지금의 최초 위치에 offset을 더하면 같은 크기의 주소만큼 이동할 수 있다.
	m_chpReadPos += offset;

	//쓸 위치도 마찬가지의 방법으로 같은 위치로 이동한다.
	offset = (DWORD)clSPacket.GetWritePos() - (DWORD)clSPacket.GetDataBufferPtr();
	m_chpWritePos += offset;
}

SPacket::~SPacket(void)
{
}

BOOL SPacket::IsValidHeader()
{
	//패킷의 크기가 헤더의 크기보다 크러나 같으면 헤더는 있는거니까
	//TRUE를 return
	if( GetPacketSize() >= PACKET_HEADER_SIZE )
		return TRUE;

	return FALSE;
}

BOOL SPacket::IsValidPacket()
{
	//헤더도 없고
	//받은데이터의 크기가 패킷의 헤더크기보다 작고
	//받은데이터의 크기가 패킷의 데이터보다 작으면
	//사용할 수 있는 패킷 혹은 더 받아야 하는 패킷이다
	if( IsValidHeader() == FALSE || m_iReceivedSize < PACKET_HEADER_SIZE || m_iReceivedSize < GetDateFiledSize() )
		return FALSE;

	return TRUE;
}

void SPacket::SetID( unsigned short protocolID )
{
	//프로토콜 id할당(?)...지정(?)
	*(m_packetHeader.m_sProtocolID) = protocolID;
}

unsigned short SPacket::GetID()
{
	//프로토콜 id를 return
	return *(m_packetHeader.m_sProtocolID);
}

void SPacket::PacketClear()
{
	//패킷의 모든 내용을 초기화 시켜 준다
	ZeroMemory( m_chDataBuffer, DEFAULT_BUFFER_SIZE );

	//데이터 크기와 프로토콜 값도 초기화!!
	m_packetHeader.m_sProtocolID	= ( unsigned short* )m_chDataBuffer + 0;
	m_packetHeader.m_sDataSize		= ( unsigned short* )( ( char* )m_chDataBuffer + 2 );

	//각각 주소값 위치를 찾아 들어감
	m_chpBuffer			= &m_chDataBuffer[PACKET_HEADER_SIZE]; //데이터의 시작 위치
	m_chpReadPos		= m_chpWritePos			= m_chpBuffer;
	m_chpEndofDataField = &m_chDataBuffer[DEFAULT_BUFFER_SIZE];

	SetID(0);

	m_iReceivedSize = 0;
}

int SPacket::MoveReadPos( int size )
{
	//이동할 크기가 0과 같거나 작으면 return
	if( size <= 0 )
		return 0;

	//이동할 위치에 크기를 더해서 마지막 공간을 넘어가게 된다면 return
	if( m_chpReadPos + size > m_chpEndofDataField )
		return 0;

	//읽는 포인트 이동
	m_chpReadPos += size;

	//이동한크기 return
	return size;
}

int SPacket::MoveWritePos( int size )
{
	//이동할 크기가 0과 같거나 작으면 return
	if( size <= 0 )
		return 0;

	//이동할 위치에 크기를 더해서 마지막 공간을 넘어가게 된다면 return
	if( m_chpWritePos + size > m_chpEndofDataField )
		return 0;

	//쓰는 포이트 이동
	m_chpWritePos += size;

	//이동한크기 return
	return size;
}

void SPacket::CopyToPacket( char* buf, int size )
{
	//우선 정리좀 해 주고
	PacketClear();
	CopyMemory( m_chDataBuffer, buf, size );
	m_iReceivedSize += size;
}

int SPacket::PutData( char* data, int size )
{
	//쓸 데이터가 마지막 공간을 넘어가면 return
	if( m_chpWritePos + size >= m_chpEndofDataField )
		return 0;

	CopyMemory( m_chpWritePos, data, size );
	//정보를 넣은 만큼 쓸 자리를 이동해 주고
	m_chpWritePos += size;

	//받은 데이터양을 더해주고
	m_iReceivedSize += size;
	
	*(m_packetHeader).m_sDataSize += size;

	return size;
}

int SPacket::GetData( char* data, int size )
{
	//읽을 데이터가 현재 쓰인 데이터의 크기를 넘어가면 return
	if( m_chpReadPos + size > m_chpBuffer + GetDateFiledSize() )
		return 0;

	//읽을 데이터가 마지막 공간을 넘어 가면 return
	if( m_chpReadPos + size >= m_chpEndofDataField )
		return 0;

	//ZeroMemory를 하지 못하는 이유는 다른 자료형들을 넣을때 이 함수를 호출하기 때문에
	// size이상의 memory를 초기화 할 수 없다
	// char형 문자열을 받아가기 위해서 처음에 zeromemory를 하고 넘기자
	CopyMemory( data, m_chpReadPos, size );
	m_chpReadPos += size;

	return size;
}

int SPacket::PutData( TCHAR* data, int size )
{
	//유니코드형 문자열을 넣음
	//변환 없이 그냥 그대로 넣어서 나중에 뺄때도 그래도 뺀다.

	//쓸 데이터가 마지막 공간을 넘어가면 return
	if( m_chpWritePos + size >= m_chpEndofDataField )
		return 0;

	//메모리를 copy하는거니까그냥 넣으면 됨
	CopyMemory( m_chpWritePos, data, size );
	//쓴만큼 이동하고
	m_chpWritePos += size;

	m_iReceivedSize += size;

	*(m_packetHeader).m_sDataSize += size;

	return size;
}

int SPacket::GetData( TCHAR* data, int size )
{
	//읽을 데이터가 현재 쓰인 데이터의 크기를 넘어가면 return
	if( m_chpReadPos + size > m_chpBuffer + GetDateFiledSize() )
		return 0;

	//읽을 데이터가 마지막 공간을 넘거 가면 return
	if( m_chpReadPos + size >= m_chpEndofDataField )
		return 0;

	//문자열이 들어가고 뒤에 null값이 있기 위해 복사할 문자열+2만큼의 공간을 0으로 초기화 해준다
	//그러고 복하사면 문사된 문자열 끝의 글자가 null값이 들어가게 되어
	//문자열이 완성된다.
	ZeroMemory( data, size+sizeof(TCHAR) );
	CopyMemory( data, m_chpReadPos, size );
	m_chpReadPos += size;

	return size;
}

////////////////////////////////////////////////////////////
//	연산자 오퍼레이터
////////////////////////////////////////////////////////////
//대입 연산자
SPacket	&SPacket::operator = ( SPacket &clSPacket )
{
	//우선 패킷 청소하고
	PacketClear();

	//항목을 복사
	*(m_packetHeader).m_sProtocolID = clSPacket.GetID();
	*(m_packetHeader).m_sDataSize = clSPacket.GetDateFiledSize();

	CopyMemory( m_chpBuffer, clSPacket.GetDataBufferPtr(), clSPacket.GetPacketSize() );

	m_iReceivedSize = clSPacket.GetRecvSize();

	DWORD offset;

	//읽을 위치의 주소와 데이터 위치의 주소값을 빼면 데이터주소의 크기가 나온다.
	offset = (DWORD)clSPacket.GetReadPos() - (DWORD)clSPacket.GetDataBufferPtr();
	//지금의 최초 위치에 offset을 더하면 같은 크기의 주소만큼 이동할 수 있다.
	m_chpReadPos += offset;

	//write의 위치도 마찬가지의 방법으로 같은 위치로 이동한다.
	offset = (DWORD)clSPacket.GetWritePos() - (DWORD)clSPacket.GetDataBufferPtr();
	m_chpWritePos += offset;

	return *this;
}

//데이터를 넣는 연산자
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

//데이터를 읽어오는 연산자
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