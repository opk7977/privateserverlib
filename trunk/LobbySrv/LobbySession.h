#pragma once

#include "SSessionObj.h"

class LobbyChar;
class Room;

//--------------------------------------
// For SingleTon
//--------------------------------------
class SSessionMgr;
class DataLeader;
class LobbyMgr;
class RoomMgr;
class CharMgr;
class CheckDB;
class SrvNet;
class SLogger;

class LobbySession : public SSessionObj
{
private:
	//======================================
	// singleTon��ü
	//======================================
	SSessionMgr*	m_sessionMgr;
	DataLeader*		m_document;
	LobbyMgr*		m_lobbyMgr;
	RoomMgr*		m_roomMgr;
	CharMgr*		m_charMgr;
	CheckDB*		m_DBMgr;
	SrvNet*			m_srvNet;
	SLogger*		m_logger;

private:
	//--------------------------------------
	// ��������
	//--------------------------------------
	LobbyChar*		m_myCharInfo;
	//--------------------------------------
	// ������
	//--------------------------------------
	Room*			m_myRoom;
	

public:
	SDECLARE_DYNAMIC(LobbySession)
	SDECLARE_DYNCREATE(LobbySession)

public:
	LobbySession(void);
	~LobbySession(void);

	void OnCreate();
	void OnDestroy();

	//������ �ʱ�ȭ
	void clear();

	Room*		GetMyRoom() const { return m_myRoom; }
	LobbyChar*	GetMyInfo() const { return m_myCharInfo; }

	//======================================
	// ��Ŷ �ؼ� 
	//======================================
	void PacketParsing( SPacket& packet );

	//======================================
	// ������ �Լ�
	//======================================
	int SendPacket( SPacket& packet, BOOL tudp = FALSE );

	//======================================
	// ���� ��Ŷ ó�� �Լ�
	//======================================
	//--------------------------------------
	// server���� Ŀ�´����̼�
	//--------------------------------------
	//GL_CONNECT_SERVER
	void RecvConnectServer();

	//GL_START_FAILD
	void RecvStartFaild( SPacket& packet );

	//GL_START_OK
	void RecvGameStart( SPacket& packet );

	//GL_GAME_END
	void RecvGameEnd( SPacket& packet );

	//GL_PLAYER_DISCONNECT
	void RecvPlayerDiconnectInGame( SPacket& packet );

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//CS_LOBBY_INSERT_LOBBY
	void RecvInsertLobby( SPacket& packet );

	//CS_LOBBY_CREATE_ROOM
	void RecvCreateRoom( SPacket& packet );

	//CS_LOBBY_INSERT_ROOM
	void RecvInsertRoom( SPacket& packet );

	//CS_ROOM_OUT_ROOM
	void RecvOutRoom();

	//CS_ROOM_CHAR_READY
	void RecvReady();

	//CS_ROOM_MAP_CHANGE
	void RecvChangeMap( SPacket& packet );

	//CS_ROOM_TEAM_CHANGE
	void RecvTeamChange();

	//CS_LOBBYSRV_ALL_CHAT
	void RecvAllChat( SPacket& packet );

	//CS_LOBBYSRV_TARGET_CHAT
	void RecvTargetChat( SPacket& packet );

	//CS_ROOM_START
	void RecvRoomStartGame();

	//======================================
	// ������ ��Ŷ �����Լ�
	//======================================
	//--------------------------------------
	// server���� Ŀ�´����̼�
	//--------------------------------------
	//LG_START_GAME
	BOOL SendCreateGameProc();

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
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

	//SC_LOBBY_ROOM_PLAYER_OUT
	BOOL SendLobbyRoomCharOut();

	//SC_ROOM_CHAR_READY
	BOOL SendRoomCharReady();

	//SC_ROOM_MAP_CHANGE
	BOOL SendRoomMapChange( int mapNum );

	//SC_LOBBY_MAP_CHANGE
	BOOL SendLobbyMapChange( int mapNum );

	//SC_ROOM_TEAM_CHANGE
	BOOL SendRoomTeamChange();

	//SC_LOBBYSRV_ALL_CHAT
	BOOL SendAllChat( TCHAR* chat );
	
	//SC_LOBBYSRV_TARGET_CHAT
	BOOL SendTargetChatToTarget( int target, TCHAR* chat );
	BOOL SendTargetChatToMe( int target, TCHAR* chat );

	//SC_ROOM_START_RESULT
	BOOL SendStartGameResult();

	//SC_ROOM_GAME_START
	BOOL SendStartGameInRoom( int roomNum );

	//SC_LOBBY_GAME_START
	BOOL SendLobbyGameStart( int roomNum );

	//SC_LOBBY_GAME_END
	BOOL SendLobbyGameEnd( int roomNum );

	//SC_LOBBY_PLAYER_DISCONNECT
	BOOL SendPlayerDisconnect();
	
	//SC_ROOM_PLAYER_DISCONNECT
	BOOL SendPlayerDisconnect( int roomNum, int sessionId );
};
