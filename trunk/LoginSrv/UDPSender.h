#pragma once

#include "SThread.h"
#include "SSocket.h"
#include "STime.h"

class DataLeader;


#define SERVER_NICNAME_LENTH	32

class UDPSender : public SThread, public SSingleton<UDPSender>
{
private:
	friend class SSingleton<UDPSender>;

private:
	DataLeader*		m_document;

private:
	SSocket			m_sock;
	BOOL			m_startThread;
	STime			m_timer;

	TCHAR			m_serverName[SERVER_NICNAME_LENTH];
	int				m_srvNameSize;
	char			m_serverIp[16];
	int				m_portNum;

public:
	UDPSender(void);
	~UDPSender(void);

	BOOL Init( TCHAR* serverName, char* serverIp, int PortNum );
	void Release();

	virtual BOOL Run();
	virtual void EndThread();
};

#define GetUDPSender UDPSender::GetInstance()

