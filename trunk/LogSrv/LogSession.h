#pragma once

#include "SSessionObj.h"

class LogDataBase;
class SPacketQueue;

class LogSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(LogSession)
	SDECLARE_DYNCREATE(LogSession)

private:
	//======================================
	// single ton °´Ã¼
	//======================================
	LogDataBase		*m_database;
	SPacketQueue	*m_packetQ;

public:
	LogSession(void);
	~LogSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	

};
