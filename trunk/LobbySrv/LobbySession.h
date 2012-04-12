#pragma once

#include "SSessionObj.h"

class LobbySession : public SSessionObj
{
private:
	//유저의 상태( 게임중인지 게임을 끊은건지)
	BOOL	IsPlayNow;

	//--------------------------------------
	// 유저 정보
	//--------------------------------------
	int		m_SessionId;
	TCHAR	m_tstrId[30];
	int		m_roomNo;

public:
	SDECLARE_DYNAMIC(LobbySession)
	SDECLARE_DYNCREATE(LobbySession)

public:
	LobbySession(void);
	~LobbySession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// 내 정보를 패킷에 담는 함수
	//======================================
	void PackageMyInfo( SPacket& packet );

	//======================================
	// 받은 패킷 처리 함수
	//======================================
	//CS_LOBBY_INSERT_LOBBY
	void RecvInsertLobby( SPacket& packet );

	//CS_LOBBY_CREATE_ROOM
	void RecvCreateRoom( SPacket& packet );

	//CS_LOBBY_INSERT_ROOM
	void RecvInsertRoom( SPacket& packet );

	//CS_ROOM_OUT_ROOM
	void RecvOutRoom();

	//CS_ROOM_READY
	void RecvReady( SPacket& packet );

	//CS_ROOM_TEAM_CHANGE
	void RecvTeamChange( SPacket& packet );

	//CS_ROOM_PLAY
	void RecvPlay();

	//CS_ROOM_CHAT
	void RecvChat( SPacket& packet );

	//======================================
	// 보내는 패킷 생성함수
	//======================================
	//SC_LOBBY_CONNECT_OK
	//함수 필요 없음!

	//SC_LOBBY_OTHER_CHARINFO
	BOOL SendOtherCharInfo();		//나에게 모두의 정보를 보낸다.
	BOOL SendMyCharInfo();			//남에게 입장한 나의 정보를 보낸다.

	//SC_LOBBY_ROOMINFO
	BOOL SendRoomInfo();

	//SC_ROOM_RESULT_CREATE
	BOOL SendResultCreate();

	//SC_LOBBY_OPEN_ROOM
	BOOL SendOpenRoom();

	//SC_ROOM_RESULT_INSERT
	BOOL SendResultInsert();

	//SC_LOBBY_INSERT_ROOM
	BOOL SendInsertRoom();

	//SC_ROOM_CHAR_INSERT
	BOOL SendCharInsert();

	//SC_ROOM_OTHER_CHARINFO
	BOOL SendRoomOtherCharInfo();

	//SC_ROOM_LEADER
	BOOL SendRoomLeader();

	//SC_ROOM_CHAR_OUT
	BOOL SendRoomCharOut();

	//SC_ROOM_CHAR_READY
	BOOL SendRoomCharReady();

	//SC_ROOM_TEAM_CHANGE
	BOOL SendRoomTeamChange();

	//SC_ROOM_CHAT
	BOOL SendRoomChat();
};
