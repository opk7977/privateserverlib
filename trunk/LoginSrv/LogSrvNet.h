#pragma once

#include "SSocket.h"

class LoginSession;
class SLogger;
class SPacket;

class LogSrvNet : public SSingleton <LogSrvNet>
{
private:
	friend class SSingleton<LogSrvNet>;

private:
	//======================================
	// singleTon객체
	//======================================
	SLogger*		m_logger;

private:
	SSocket			m_sock;
	SOCKADDR		m_sockAddr;

	LoginSession*	m_LogSrv;

	BOOL			m_isConnect;

private:
	LogSrvNet(void);
	~LogSrvNet(void);

public:
	//소켓 생성시 nonblock설정flag
	BOOL Init();

	//연결
	BOOL ConnectToSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( LoginSession* session );
	inline LoginSession* GetSession() const { return m_LogSrv; }

	inline BOOL IsConnect() { m_isConnect; }

	BOOL SendToLogServer( SPacket &packet );

};

#define GetSrvNet LogSrvNet::GetInstance()

