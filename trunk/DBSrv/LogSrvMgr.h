#pragma once

#include "SSocket.h"

//문자열 길이
#define SEND_LOG_STRING_LENTH		4096

class SLogger;
class DBSession;
class SPacket;

class LogSrvMgr : public SSingleton <LogSrvMgr>, public SServerObj
{
private:
	friend class SSingleton<LogSrvMgr>;

private:
	//======================================
	// singleTon객체
	//======================================
	SLogger*			m_logger;

private:
	SSocket				m_sock;
	SOCKADDR			m_sockAddr;

	DBSession*			m_logSrv;
	BOOL				m_isConnect;

	int					m_serverID;

public:
	//사용자 지정 flag
	enum LOG_LEVEL
	{
		LOG_LEVEL_INFORMATION	= 1,
		LOG_LEVEL_WORRNING,
		LOG_LEVEL_ERROR,
	};

private:
	LogSrvMgr(void);
	~LogSrvMgr(void);

public:
	//--------------------------------------
	// 네트워크 연결 관련
	//--------------------------------------
	BOOL Init( int SrvID );
	BOOL ConnectToLogSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( DBSession* session );
	inline DBSession* GetSession() const { return m_logSrv; }

	inline BOOL IsConnect() { return m_isConnect; }

	BOOL SendToLogSrv( SPacket& packet );

	//--------------------------------------
	// 로그 문장 관련
	//--------------------------------------
	void SendLog( int errLv, TCHAR* lpszFmt, ... );
	void SendErrLog( INT32 errorCode, TCHAR* lpszFmt, ... );
};

#define GetLogSrvMgr LogSrvMgr::GetInstance()

