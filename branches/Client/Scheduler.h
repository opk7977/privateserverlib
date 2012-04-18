#pragma once

#define MAXPROCESS_PER_FRAME 30


#include "SPacket.h"
#include "STime.h"

#include "ClientDoc.h"

class CRoom;

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

public:
	CRoom*		m_pRoom;

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

	//SC_ROOM_OTHER_CHARINFO,
	void RecvRoomOtherChar();

	//SC_ROOM_LEADER,
	void RecvRoomLeader();

	//SC_ROOM_CHAR_OUT,
	void RecvRoomCharOut();

	//SC_LOBBY_ROOMPLAYER_COUNTDOWN,
	void RecvRoomPlayerCountDown();

	//SC_ROOM_CHAR_READY,
	void RecvCharReady();

	//SC_ROOM_TEAM_CHANGE,
	void RecvTeamChange();

	//SC_LOBBY_CHAT,
	void RecvLobbyChat();

	//SC_LOBBY_PLAYER_DISCONNECT,
	void RecvLobbyPlayerDisconnect();
};

#define GetScheduler CScheduler::GetInstance()
