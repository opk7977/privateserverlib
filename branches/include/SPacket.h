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
	//�����Ͱ� ������ ���� ���� ����
	char	m_chDataBuffer[DEFAULT_BUFFER_SIZE];

	//packet �ش�
	HEADER	m_packetHeader;
	//���� ������ ���� ��ġ(�ش�����)
	char*	m_chpBuffer;

	//���� ��ġ/ �� ��ġ
	char*	m_chpReadPos;
	char*	m_chpWritePos;

	//����(?) ��Ŷ�� ũ��
	int		m_iReceivedSize;

	//�������� ����ġ!!
	char*	m_chpEndofDataField;


public:
	//���۸� �⺻ ũ��� ����
	SPacket(void);
	//��Ŷ�� ���̷�Ʈ�� protocolID�� �����ؼ� ����
	SPacket( unsigned short protocolid );
	//��Ŷ ����
	SPacket( const SPacket &clSPacket );
	//�Ҹ���
	virtual ~SPacket(void);

	//�ش��� ��� �ִ����� Ȯ��( ������� ��Ŷ )
	//�ش��� ��� ������ TRUE�� return 
	BOOL IsValidHeader();
	//��Ŷ�� ������ΰ��� Ȯ���ϴ� �Լ�
	//FALSE�� return�Ǹ� ��� ����
	BOOL IsValidPacket();

	//��� �����ϱ�
	void SetID( unsigned short protocolID );
	//����� �������� ID���
	unsigned short GetID();

	//��Ŷ �� �����/ û��
	void PacketClear();

	//��Ŷ�� ������ũ��(�ش�������) ���
	inline unsigned short GetDateFiledSize() const { return *m_packetHeader.m_sDataSize; }
	//���� size���
	inline int GetRecvSize() const { return m_iReceivedSize; }
	//��Ŷ�� ��üũ�� ���(������+�ش�)
	inline int GetPacketSize() const { return GetDateFiledSize() + PACKET_HEADER_SIZE; }

	//��Ŷ�� ������ ���� ��ġ ���
	inline char* GetDataBufferPtr() const { return (char*)m_chDataBuffer; }

	//��Ŷ�� �� ��ġ ���
	inline char* GetDataBufferEndPtr() const { return m_chpEndofDataField; }

	//=======================================================================================
	// readPos/ writePos
	//---------------------------------------------------------------------------------------
	//���� ��ġ�� ���� ��ġ ���
	inline char* GetReadPos() const { return m_chpReadPos; }
	inline char* GetWritePos() const { return m_chpWritePos; }
	//---------------------------------------------------------------------------------------
	//���� ��ġ�� ���� ��ġ �̵�
	int MoveReadPos( int size );
	int MoveWritePos( int size );
	//=======================================================================================

	//=======================================================================================
	// ������ ����/ �б�
	//---------------------------------------------------------------------------------------
	//���ڿ��� ��Ŷ����
	void CopyToPacket( char* buf, int size );
	//---------------------------------------------------------------------------------------
	//��Ƽ����Ʈ ��( in dataBuf, in size )( return size )
	int PutData( char* data, int size );
	int GetData( char* data, int size );

	int PutDataA( char* data, int size );
	int GetDataA( char* data, int size );
	//---------------------------------------------------------------------------------------
	//�����ڵ� ��( in dataBuf, in size )( return size )
	int PutData( TCHAR* data, int size );
	int GetData( TCHAR* data, int size );

	int PutDataW( TCHAR* data, int size );
	int GetDataW( TCHAR* data, int size );
	//=======================================================================================

	////////////////////////////////////////////////////////////
	//	������ ���۷�����
	////////////////////////////////////////////////////////////
	//���� ������
	SPacket	&operator = ( SPacket &clSPacket );

	//�����͸� �ִ� ������
	SPacket	&operator << ( BYTE _value );
	SPacket	&operator << ( short _value );
	SPacket	&operator << ( int _value );
	SPacket	&operator << ( DWORD _value );
	SPacket &operator << ( float _value );
	SPacket &operator << ( __int64 _value );
	SPacket &operator << ( SPacket &_value );

	//�����͸� �о���� ������
	SPacket	&operator >> ( BYTE &_value );
	SPacket	&operator >> ( short &_value );
	SPacket	&operator >> ( int &_value );
	SPacket	&operator >> ( DWORD &_value );
	SPacket &operator >> ( float &_value );
	SPacket &operator >> ( __int64 _value );
	SPacket &operator >> ( SPacket &_value );

};


#endif

