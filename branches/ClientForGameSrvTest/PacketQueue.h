#pragma once

#include "SServerStd.h"

#define VECBUFFER_SIZE 100
#define PACKETDATA_SIZE 8192


class PacketQueue : public SSingleton <PacketQueue>
{
private:
	friend class SSingleton<PacketQueue>;

private:
	//버퍼를 담고 있을 공간
	std::vector<char*>	m_vecPacketPool;

	//읽고 쓸 위치
	int					m_iReadPos;
	int					m_iWritePos;

	//쌓여 있는 데이터 수
	int					m_iDataCount;


public:
	PacketQueue(void);
	~PacketQueue(void);

	BOOL PutPacket( char* buf, int dataSize );
	//패킷 처리가 끝나면MoveReadPos()를 호출해 줘야 한다!
	char* GetPacket();
	
	void MoveReadPos();

	//쌓여 있는 데이터 수를 return
	int GetDataCount();
};

#define GetPacketQ PacketQueue::GetInstance()

