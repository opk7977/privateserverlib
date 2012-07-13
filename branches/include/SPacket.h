#ifndef __PACKET__
#define __PACKET__

#define DEFAULT_BUFFER_SIZE			8192
#define PACKET_HEADER_SIZE			4

#define STRING_BLOCK				512

#include "SServerObj.h"

class SPacket : public SServerObj
{
public:
	typedef struct
	{
		unsigned short*		m_sProtocolID;
		unsigned short*		m_sDataSize;
	} HEADER;

private:
	//데이터가 실제로 들어가서 쌓일 공간
	char	m_chDataBuffer[DEFAULT_BUFFER_SIZE];

	//packet 해더
	HEADER	m_packetHeader;
	//실제 데이터 시작 위치(해더다음)
	char*	m_chpBuffer;

	//읽을 위치/ 쓸 위치
	char*	m_chpReadPos;
	char*	m_chpWritePos;

	//쌓인(?) 패킷의 크기
	int		m_iReceivedSize;

	//데이터의 끗위치!!
	char*	m_chpEndofDataField;


public:
	//버퍼를 기본 크기로 생성
	SPacket(void);
	//패킷에 다이렉트로 protocolID를 지정해서 생성
	SPacket( unsigned short protocolid );
	//패킷 복사
	SPacket( const SPacket &clSPacket );
	//소멸자
	virtual ~SPacket(void);

	//해더가 들어 있는지를 확인( 사용중인 패킷 )
	//해더가 들어 있으면 TRUE를 return 
	BOOL IsValidHeader();
	//패킷이 사용중인가를 확인하는 함수
	//FALSE가 return되면 사용 가능
	BOOL IsValidPacket();

	//헤더 지정하기
	void SetID( unsigned short protocolID );
	//헤더의 프로토콜 ID얻기
	unsigned short GetID();

	//패킷 싹 지우기/ 청소
	void PacketClear();

	//패킷의 데이터크기(해더미포함) 얻기
	inline unsigned short GetDateFiledSize() const { return *m_packetHeader.m_sDataSize; }
	//받은 size얻기
	inline int GetRecvSize() const { return m_iReceivedSize; }
	//패킷의 전체크기 얻기(데이터+해더)
	inline int GetPacketSize() const { return GetDateFiledSize() + PACKET_HEADER_SIZE; }

	//패킷의 데이터 시작 위치 얻기
	inline char* GetDataBufferPtr() const { return (char*)m_chDataBuffer; }

	//패킷의 끝 위치 얻기
	inline char* GetDataBufferEndPtr() const { return m_chpEndofDataField; }

	//=======================================================================================
	// readPos/ writePos
	//---------------------------------------------------------------------------------------
	//쓰는 위치와 읽을 위치 얻기
	inline char* GetReadPos() const { return m_chpReadPos; }
	inline char* GetWritePos() const { return m_chpWritePos; }
	//---------------------------------------------------------------------------------------
	//쓰는 위치와 읽을 위치 이동
	int MoveReadPos( int size );
	int MoveWritePos( int size );
	//=======================================================================================

	//=======================================================================================
	// 데이터 쓰기/ 읽기
	//---------------------------------------------------------------------------------------
	//문자열을 패킷으로
	void CopyToPacket( char* buf, int size );
	//---------------------------------------------------------------------------------------
	//멀티바이트 용( in dataBuf, in size )( return size )
	int PutData( char* data, int size );
	int GetData( char* data, int size );

	int PutDataA( char* data, int size );
	int GetDataA( char* data, int size );
	//---------------------------------------------------------------------------------------
	//유니코드 용( in dataBuf, in size )( return size )
	int PutData( TCHAR* data, int size );
	int GetData( TCHAR* data, int size );

	int PutDataW( TCHAR* data, int size );
	int GetDataW( TCHAR* data, int size );
	//=======================================================================================

	////////////////////////////////////////////////////////////
	//	연산자 오퍼레이터
	////////////////////////////////////////////////////////////
	//대입 연산자
	SPacket	&operator = ( SPacket &clSPacket );

	//데이터를 넣는 연산자
	SPacket	&operator << ( BYTE _value );
	SPacket	&operator << ( short _value );
	SPacket	&operator << ( int _value );
	SPacket	&operator << ( DWORD _value );
	SPacket &operator << ( float _value );
	SPacket &operator << ( __int64 _value );
	SPacket &operator << ( SPacket &_value );

	//데이터를 읽어오는 연산자
	SPacket	&operator >> ( BYTE &_value );
	SPacket	&operator >> ( short &_value );
	SPacket	&operator >> ( int &_value );
	SPacket	&operator >> ( DWORD &_value );
	SPacket &operator >> ( float &_value );
	SPacket &operator >> ( __int64 _value );
	SPacket &operator >> ( SPacket &_value );

};


#endif

