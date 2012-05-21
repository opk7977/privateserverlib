#pragma once

#include "SServerObj.h"

//���ڿ� ����
#define TMPSTRING_LENTH		1024

class SPacket;
class LogSrvNet;

class LogMgr : public SSingleton <LogMgr>, public SServerObj
{
private:
	friend class SSingleton<LogMgr>;

private:
	//======================================
	// singleTon ��ü
	//======================================
	LogSrvNet*		m_logSrv;

public:
	//����� ���� flag
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
