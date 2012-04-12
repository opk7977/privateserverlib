#pragma once

#include "SSessionObj.h"

class LobbySession : public SSessionObj
{
private:
	//������ ����( ���������� ������ ��������)
	BOOL	IsPlayNow;

	//--------------------------------------
	// ���� ����
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
	// �� ������ ��Ŷ�� ��� �Լ�
	//======================================
	void PackageMyInfo( SPacket& packet );

	//======================================
	// ���� ��Ŷ ó�� �Լ�
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
	// ������ ��Ŷ �����Լ�
	//======================================
	//SC_LOBBY_CONNECT_OK
	//�Լ� �ʿ� ����!

	//SC_LOBBY_OTHER_CHARINFO
	BOOL SendOtherCharInfo();		//������ ����� ������ ������.
	BOOL SendMyCharInfo();			//������ ������ ���� ������ ������.

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
