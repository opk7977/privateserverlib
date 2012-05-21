#pragma once

#include "SServerObj.h"

//문자열 길이
#define TMPSTRING_LENTH		1024

class SPacket;
class LogSrvNet;

class LogMgr : public SSingleton <LogMgr>, public SServerObj
{
private:
	friend class SSingleton<LogMgr>;

private:
	//======================================
	// singleTon 객체
	//======================================
	LogSrvNet*		m_logSrv;

public:
	//사용자 지정 flag
	enum LOG_LEVEL
	{
		LOG_LEVEL_INFORMATION	= 1,
		LOG_LEVEL_WORRNING,
		LOG_LEVEL_ERROR,
	};

private:
	LogMgr(void);
	~LogMgr(void);

public:
	void SendLog( int SrvID, int errLv, char* lpszFmt, ... );
	void SendLog( int SrvID, int errLv, TCHAR* lpszFmt, ... );

	void SendErrLog( int SrvID, INT32 errorcode, char* lpszFmt );
	void SendErrLog( int SrvID, INT32 errorcode, TCHAR* lpszFmt );

	void SendPacket( int SrvID, int LogLv, TCHAR* stringLog );
};

#define GetLog LogMgr::GetInstance()
