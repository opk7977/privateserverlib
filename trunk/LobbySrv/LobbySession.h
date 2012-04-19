#pragma once

#include "SSessionObj.h"

class LobbySession : public SSessionObj
{
private:
	//������ ����( ���������� ������ ��������)
	BOOL	IsPlayNow;

	//--------------------------------------
	// ��������
	//--------------------------------------
	int		m_SessionId;
	TCHAR	m_tstrId[30];
	//--------------------------------------
	// ������
	//--------------------------------------
	int		m_roomNo;
	int		m_team;	//0: ����, 1:����

public:
	SDECLARE_DYNAMIC(LobbySession)
	SDECLARE_DYNCREATE(LobbySession)

public:
	LobbySession(void);
	~LobbySession(void);

	void OnCreate();
	void OnDestroy();

	int GetSessionID() const { return m_SessionId; }

	//������ �ʱ�ȭ
	void clear();

	void PacketParsing( SPacket& packet );

	//======================================
	// �� ������ ��Ŷ�� ��� �Լ�
	//======================================
	void PackageMyInfo( SPacket& packet, BOOL isTeam = FALSE );

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

	//CS_ROOM_CHAR_READY
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
	BOOL SendResultCreate( int result );

	//SC_LOBBY_OPEN_ROOM
	BOOL SendOpenRoom( int room, TCHAR* title, int titleSize );

	//SC_LOBBY_CLOSE_ROOM
	BOOL SendCloseRoom( int roomNum );

	//SC_ROOM_RESULT_INSERT
	BOOL SendResultInsert( int result );

	//SC_LOBBY_INSERT_ROOM
	BOOL SendInsertRoom();

	//SC_ROOM_OTHER_CHARINFO
	BOOL SendRoomOtherCharInfo();
	BOOL SendRoomMyInfoToOtherChar();

	//SC_ROOM_LEADER
	BOOL SendRoomLeader();
	BOOL SendRoomLeaderToAll( int leader );

	//SC_ROOM_CHAR_OUT
	BOOL SendRoomCharOut();

	//SC_ROOM_CHAR_READY
	BOOL SendRoomCharReady();

	//SC_ROOM_TEAM_CHANGE
	BOOL SendRoomTeamChange();

	//SC_ROOM_CHAT
	BOOL SendChat( TCHAR* chat );

	//SC_LOBBY_PLAYER_DISCONNECT
	BOOL SendPlayerDisconnect();
};
