#pragma once

#include "SServerStd.h"

class SPacket;

#define VECBUFFER_SIZE 300
#define PACKETDATA_SIZE 8192

class SPacketQueue : public SSingleton <SPacketQueue>
{
private:
	friend class SSingleton<SPacketQueue>;

private:
	std::vector<char*>		m_vecPacketPool;
	std::vector<int>		m_vecPacketSize;

	//�а� �� ��ġ
	int						m_iReadPos;
	int						m_iWritePos;

	//�׿� �ִ� ������ ��
	int						m_iDataCount;

	//ť�� ũ��
	int						m_iQueueSize;

private:
	SPacketQueue(void);
	~SPacketQueue(void);

public:
	void Init( int PoolSize = VECBUFFER_SIZE );
	void Release();

	BOOL PutPacket( char* buf, int dataSize );
	//��Ŷ ó���� ������MoveReadPos()�� ȣ���� ��� �Ѵ�!
	BOOL GetPacket( SPacket& packet );
	void MoveReadPos();

	//�׿� �ִ� ������ ���� return
	int GetDataCount();

};

#define GetPacketQ SPacketQueue::GetInstance()

