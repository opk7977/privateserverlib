#pragma once

#include "SServerObj.h"

class SPacket;

#define VECBUFFER_SIZE 300
#define PACKETDATA_SIZE 8192

struct CharData
{
private:
	int		m_isData;
	int		m_sessionId;
	char	m_data[PACKETDATA_SIZE];
	int		m_dataTime;

public:
	CharData( int sessionId );
	~CharData();

	void SetData( char* data, int size );
	char* GetData();

	int GetSessionID();

	void SetDataTime( int dwTime );
	DWORD GetDataTime();

	void SetNonData();
	BOOL IsData();

	CharData &operator = ( CharData &clSPacket );
};

class UPacketQueue : public SSingleton <UPacketQueue>, public SServerObj
{
private:
	friend class SSingleton<UPacketQueue>;

private:
	ATL::CAtlMap<int, CharData*>	m_uPacketPool;

	int								m_spaceCount;

private:
	UPacketQueue(void);
	~UPacketQueue(void);

public:
	void Init();

	void Release();
	void AddItem( int session );
	void DelItem( int session );

	BOOL PutPacket( SPacket& packet );
	//패킷 처리가 끝나면MoveReadPos()를 호출해 줘야 한다!
// 	BOOL GetPacket( SPacket& packet );
// 	void MoveReadPos();
// 
// 	//쌓여 있는 데이터 수를 return
// 	int GetDataCount();
	POSITION GetPackPoolStartPos();
	void NextPos( POSITION &pos, CharData** data );
};

#define GetUPacketQ UPacketQueue::GetInstance()
