#pragma once

#include "SSessionObj.h"

enum LOG_SERVER
{
	//========================================================
	// 로그서버 -> 다른서버
	// 로그 서버로 정상적으로 접속되었다
	//--------------------------------------------------------
	// 데이터 없음
	//========================================================
	LOG_SERVER_CONNECT_OK = 10000,

	//========================================================
	// 다른서버 -> 로그서버
	// 로그 서버로 넘어 오는 모든 패킷이 동일한 ID를 갖는다
	//--------------------------------------------------------
	// int			-			ServerID
	// int			-			LogLevel
	// int			-			LogDataLenth
	// TCHAR		-			LogString
	//========================================================
	RECV_SERVER_LOG,

	//========================================================
	// 로그서버 -> 다른서버
	// 로그 서버 문 닫음
	//--------------------------------------------------------
	// 데이터 없음
	//========================================================
	LOG_SERVER_SHOTDOWN,
};

class LogDataBase;
class SPacketQueue;

class LogSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(LogSession)
	SDECLARE_DYNCREATE(LogSession)

private:
	//======================================
	// single ton 객체
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
