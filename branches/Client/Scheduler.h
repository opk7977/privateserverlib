#pragma once

#define MAXPROCESS_PER_FRAME 30


#include "SPacket.h"

#include "STime.h"

class CScheduler : public SSingleton <CScheduler>
{
private:
	friend class SSingleton<CScheduler>;

private:
	SPacket m_packet;

	HANDLE m_schThread;
	HANDLE m_startWork;

	STime m_timer;
	float m_frame;

private:
	CScheduler(void);
	~CScheduler(void);

public:
	void Init();

	//������ �����Լ�
	unsigned int Run();

	void PacketProcess();


	void PacketParsing();

	//��Ŷ�� ó���ϴ� �Լ�
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
