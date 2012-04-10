#pragma once

#define MAXPROCESS_PER_FRAME 30

#include "SPacket.h"


class CScheduler
{
private:
	SPacket m_packet;

	CSampleDemoGameDoc* pDoc;

public:
	CScheduler(void);
	~CScheduler(void);

	void Run();


	void PacketParsing();

	//패킷을 처리하는 함수
	//SC_LOGIN_CONNECT_OK
	void RecvConnectOK();

	//SC_LOGIN_CHECK_ID_RESULT
	void RecvCheckID();

	//SC_LOGIN_CREATE_RESULT
	void RecvCreateResult();

	//SC_LOGIN_LOGIN_RESULT
	void RecvLoginResult();
};
