#pragma once

#include "SSocket.h"

//���ڿ� ����
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
	// singleTon��ü
	//======================================
	SLogger*			m_logger;

private:
	SSocket				m_sock;
	SOCKADDR			m_sockAddr;

	DBSession*			m_logSrv;
	BOOL				m_isConnect;

	int					m_serverID;

public:
	//����� ���� flag
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
	// ��Ʈ��ũ ���� ����
	//--------------------------------------
	BOOL Init( int SrvID );
	BOOL ConnectToLogSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( DBSession* session );
	inline DBSession* GetSession() const { return m_logSrv; }

	inline BOOL IsConnect() { return m_isConnect; }

	BOOL SendToLogSrv( SPacket& packet );

	//--------------------------------------
	// �α� ���� ����
	//--------------------------------------
	void SendLog( int errLv, TCHAR* lpszFmt, ... );
	void SendErrLog( INT32 errorCode, TCHAR* lpszFmt, ... );
};

#define GetLogSrvMgr LogSrvMgr::GetInstance()

