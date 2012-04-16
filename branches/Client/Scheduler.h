#pragma once

#define MAXPROCESS_PER_FRAME 30


#include "SPacket.h"
#include "STime.h"

#include "ClientDoc.h"

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

	CClientDoc* m_pDoc;

private:
	CScheduler(void);
	~CScheduler(void);

public:
	void Init();

	//쓰레드 동작함수
	unsigned int Run();

	void PacketProcess();
	void PacketParsing();

	//패킷을 처리하는 함수

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
	void RecvLobbyPlayerInfo();

	//SC_LOBBY_ROOMINFO,
	void RecvLobbyRoomInfo();

	//SC_ROOM_RESULT_CREATE,
	void RecvRoomResultCreate();

	//SC_LOBBY_OPEN_ROOM,
	void RecvLobbyOpenRoom();

	//SC_LOBBY_CLOSE_ROOM
	void RecvLobbyCloseRoom();

	//SC_ROOM_RESULT_INSERT,
	void RecvRoomResultInsert();

	//SC_LOBBY_INSERT_ROOM,
	void RecvLobbyInsertRoom();

	//SC_ROOM_CHAR_INSERT,

	//SC_ROOM_OTHER_CHARINFO,

	//SC_ROOM_LEADER,

	//SC_ROOM_CHAR_OUT,

	//SC_ROOM_CHAR_READY,

	//SC_ROOM_TEAM_CHANGE,

	//SC_ROOM_CHAT,

	//SC_LOBBY_PLAYER_DISCONNECT,
	void RecvLobbyPlayerDisconnect();
};

#define GetScheduler CScheduler::GetInstance()
