#pragma once

#include "SServerStd.h"
#include "SPacket.h"

#define PACKETDATA_SIZE 8192
#define PACKETPOOL_SIZE 10

class TmpPacketQueue : public SSingleton <TmpPacketQueue>
{
private:
	friend class SSingleton<TmpPacketQueue>;

private:
	std::vector<char*>		m_vecPacPool;
	std::vector<int>		m_vecPacketSize;

	//�а� �� ��ġ
	int						m_iReadPos;
	int						m_iWritePos;

	//�׿� �ִ� ������ ��
	int						m_iDataCount;
	//ť�� ũ��
	int						m_iQSize;

public:
	TmpPacketQueue(void);
	~TmpPacketQueue(void);

	void Init( int poolsize );
	void Release();

	BOOL PutPacket( char* buf, int dataSize );
	BOOL GetPacket( SPacket& packet );
	void MoveReadPos();
	int GetDataCount();
};

#define GetPrePacketQ TmpPacketQueue::GetInstance()

//==============================================================

class SLogger;

class TmpPacketParser : public SSingleton <TmpPacketParser>
{
private:
	friend class SSingleton<TmpPacketParser>;

private:
	//�̱���
	TmpPacketQueue*		m_packetQ;
	SLogger*			m_logger;


	SPacket				m_packet;

private:
	TmpPacketParser();
	~TmpPacketParser();

public:
	void Init();
	void Release();

	void PacketProcess();
	void PacketParsing();

private:
	void AddPacket();
	void DelPacket();

};

#define GetPrePacketParser TmpPacketParser::GetInstance()

