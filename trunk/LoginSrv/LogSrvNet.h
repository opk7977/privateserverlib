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
	// singleTon��ü
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
	//���� ������ nonblock����flag
	BOOL Init();

	//����
	BOOL ConnectToSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( LoginSession* session );
	inline LoginSession* GetSession() const { return m_LogSrv; }

	inline BOOL IsConnect() { m_isConnect; }

	BOOL SendToLogServer( SPacket &packet );

};

#define GetSrvNet LogSrvNet::GetInstance()

