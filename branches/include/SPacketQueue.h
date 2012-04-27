#pragma once

#include "SServerStd.h"

class SPacket;

#define VECBUFFER_SIZE 100
#define PACKETDATA_SIZE 8192

class SPacketQueue : public SSingleton <SPacketQueue>
{
private:
	friend class SSingleton<SPacketQueue>;

private:
	std::vector<char*>		m_vecPacketPool;
	std::vector<int>		m_vecPacketSize;

	//읽고 쓸 위치
	int						m_iReadPos;
	int						m_iWritePos;

	//쌓여 있는 데이터 수
	int						m_iDataCount;

private:
	SPacketQueue(void);
	~SPacketQueue(void);

public:

	BOOL PutPacket( char* buf, int dataSize );
	//패킷 처리가 끝나면MoveReadPos()를 호출해 줘야 한다!
	BOOL GetPacket( SPacket& packet );
	void MoveReadPos();

	//쌓여 있는 데이터 수를 return
	int GetDataCount();

};

#define GetPacketQ SPacketQueue::GetInstance()

