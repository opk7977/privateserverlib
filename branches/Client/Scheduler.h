#pragma once

<<<<<<< .mine
#define MAXPROCESS_PER_FRAME 30


#include "SPacket.h"

#include "STime.h"

class CScheduler : public SSingleton <CScheduler>
=======
#define MAXPROCESS_PER_FRAME 30

#include "SPacket.h"


class CScheduler
>>>>>>> .r10
{
<<<<<<< .mine
private:
	friend class SSingleton<CScheduler>;

private:
	SPacket m_packet;

	HANDLE m_schThread;
	HANDLE m_startWork;

	STime m_timer;
	float m_frame;

private:
=======
private:
	SPacket m_packet;

	CSampleDemoGameDoc* pDoc;

public:
>>>>>>> .r10
	CScheduler(void);
	~CScheduler(void);

public:
	void Init();

	//쓰레드 동작함수
	unsigned int Run();

	void PacketProcess();


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

#define GetScheduler CScheduler::GetInstance()
