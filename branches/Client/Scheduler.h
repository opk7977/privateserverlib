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

	//======================================
	// LOGIN
	//======================================
	//SC_LOGIN_CONNECT_OK
	void RecvLoginConnectOK();

	//SC_LOGIN_CHECK_ID_RESULT
	void RecvLoginCheckID();

	//SC_LOGIN_CREATE_RESULT
	void RecvLoginCreateResult();

	//SC_LOGIN_LOGIN_RESULT
	void RecvLoginLoginResult();

	//======================================
	// LOBBY
	//======================================
	//SC_LOBBY_CONNECT_OK
	void RecvLobbyConnectOK();

	//SC_LOBBY_OTHER_CHARINFO

	//SC_LOBBY_ROOMINFO,

	//SC_ROOM_RESULT_CREATE,

	//SC_LOBBY_OPEN_ROOM,

	//SC_ROOM_RESULT_INSERT,

	//SC_LOBBY_INSERT_ROOM,

	//SC_ROOM_CHAR_INSERT,

	//SC_ROOM_OTHER_CHARINFO,

	//SC_ROOM_LEADER,

	//SC_ROOM_CHAR_OUT,

	//SC_ROOM_CHAR_READY,

	//SC_ROOM_TEAM_CHANGE,

	//SC_ROOM_CHAT,
};

#define GetScheduler CScheduler::GetInstance()
