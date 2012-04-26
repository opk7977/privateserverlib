#pragma once

#include "SServerStd.h"

#define VECBUFFER_SIZE 100
#define PACKETDATA_SIZE 8192


class PacketQueue : public SSingleton <PacketQueue>
{
private:
	friend class SSingleton<PacketQueue>;

private:
	//���۸� ��� ���� ����
	std::vector<char*>	m_vecPacketPool;

	//�а� �� ��ġ
	int					m_iReadPos;
	int					m_iWritePos;

	//�׿� �ִ� ������ ��
	int					m_iDataCount;


public:
	PacketQueue(void);
	~PacketQueue(void);

	BOOL PutPacket( char* buf, int dataSize );
	//��Ŷ ó���� ������MoveReadPos()�� ȣ���� ��� �Ѵ�!
	char* GetPacket();
	
	void MoveReadPos();

	//�׿� �ִ� ������ ���� return
	int GetDataCount();
};

#define GetPacketQ PacketQueue::GetInstance()

