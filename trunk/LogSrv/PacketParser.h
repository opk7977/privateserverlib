#pragma once
#include "SPacket.h"

class LogDataBase;
class SPacketQueue;

class PacketParser : public SSingleton <PacketParser>
{
private:
	friend class SSingleton<PacketParser>;

private:
	SPacket		m_packet;

	//======================================
	// singleton °´Ã¼
	//======================================
	LogDataBase		*m_database;
	SPacketQueue	*m_packetQ;

private:
	PacketParser(void);
	~PacketParser(void);

private:
	void PacketParsing();

public:
	void Init();
	void Release();

	void PacketProcess();

};

#define GetParser PacketParser::GetInstance()

