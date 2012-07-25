#pragma once

#include "SSessionObj.h"

class LobbyChar;
class Room;

//--------------------------------------
// For SingleTon
//--------------------------------------
//class SSessionMgr;
class DataLeader;
class LobbyMgr;
class RoomMgr;
class CharMgr;
class SrvNet;
class DBSrvMgr;
class LogSrvMgr;
class SLogger;

class LobbySession : public SSessionObj
{
private:
	//======================================
	// singleTon��ü
	//======================================
	//SSessionMgr*	m_sessionMgr;
	static DataLeader*		m_document;
	static LobbyMgr*		m_lobbyMgr;
	static RoomMgr*			m_roomMgr;
	static CharMgr*			m_charMgr;
	static SrvNet*			m_srvNet;
	static DBSrvMgr*		m_dbMgr;
	static LogSrvMgr*		m_logSrv;
	static SLogger*			m_logger;

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
	// ���� ��Ŷ ó�� �Լ�
	//======================================
	//--------------------------------------
	// LogSrv���� Ŀ�´����̼�
	//--------------------------------------
	//LOG_SERVER_CONNECT_OK
	void RecvLogConnectOK();

	//LOG_SERVER_SHOTDOWN
	//��� �ɵ�...

	//--------------------------------------
	// DBSrv���� Ŀ�´����̼�
	//--------------------------------------
	//DB_TO_OTHER_CONNECT_OK
	void RecvDBConnectOK();

	//DB_TO_LOBBY_CHARACTER_LOGIN
	void RecvCharacterLogin( SPacket& packet );

	//DB_TO_OTHER_DROP_PLAYER
	void RecvCharDrop( SPacket& packet );

	//DB_TO_LOBBY_UPDATE_USERDATA
	void RecvToDBUpdateUserData( SPacket& packet );

	//--------------------------------------
	// GameSrv���� Ŀ�´����̼�
	//--------------------------------------
	//GL_CONNECT_SERVER
	void RecvConnectServer();

	//GL_START_FAILD
	void RecvStartFaild( SPacket& packet );

	//GL_START_OK
	void RecvGameStart( SPacket& packet );

	//GL_GAME_END
	void RecvGameEnd( SPacket& packet );

	//GL_GAME_GOTO_LOBBY
	void RecvGameGotoLobby( SPacket& packet );

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
	void RecvMapChange( SPacket& packet );

	//CS_ROOM_MODE_CHANGE
	void RecvChangeMode( SPacket& packet );

	//CS_ROOM_CHANGE_WEAPON
	void RecvWeaponChange( SPacket& packet );

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
	// DBSrv���� Ŀ�´����̼�
	//--------------------------------------
	//LOBBY_TO_DB_CHARINSERT_READY_RESULT
	BOOL SendToDBCharInsertReadyResult( int index, int sessionId );

	//OTHER_TO_DB_DISCONNECT_CHARACTER
	BOOL SendToDBCharDiscconect( int sessionID );

	//--------------------------------------
	// GameSrv�� Ŀ�´����̼�
	//--------------------------------------
	//LG_START_GAME
	BOOL SendCreateGameProc();

	//LG_END_GAME_READY_OK
	BOOL SendGameEndReadyOK( int roomNum );

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
	BOOL SendOpenRoom();

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

	//SC_ROOM_OUT_RESULT
	BOOL SendRoomOutResult();

	//SC_ROOM_CHAR_OUT
	BOOL SendRoomCharOut();

	//SC_LOBBY_ROOM_PLAYER_OUT
	BOOL SendLobbyRoomCharOut();
	BOOL SendLobbyRoomCharOut( int roomNum, int sessionId );

	//SC_ROOM_CHAR_READY
	BOOL SendRoomCharReady();
	BOOL SendRoomCharReady( int sessionId, BOOL ready );

	//SC_ROOM_MAP_CHANGE
	BOOL SendRoomMapChange();

	//SC_LOBBY_MAP_CHANGE
	BOOL SendLobbyMapChange();

	//SC_ROOM_MODE_CHANGE
	BOOL SendRoomModeChange();

	//SC_LOBBY_MODE_CHANGE
	BOOL SendLobbyModeChange();

	//SC_ROOM_CHANGE_WEAPON
	BOOL SendRoomWeaponChange();

	//SC_ROOM_TEAM_CHANGE
	BOOL SendRoomTeamChange();

	//SC_LOBBYSRV_ALL_CHAT
	BOOL SendAllChat( TCHAR* chat );
	
	//SC_LOBBYSRV_TARGET_CHAT
	BOOL SendTargetChatToTarget( int target, TCHAR* chat );
	BOOL SendTargetChatToMe( int target, TCHAR* chat );

	//���� ��ư�� ���õ� �Լ��� ȣ���ϴ� �Լ�
	BOOL SendStartBtnForVisible();
	//SC_ROOM_START_VISIBLE
	BOOL SendRoomStartVisible();
	//SC_ROOM_START_INVISIBLE
	BOOL SendRoomStartInvisible();

	//SC_ROOM_START_RESULT
	BOOL SendStartGameResult();

	//SC_ROOM_GAME_START
	BOOL SendStartGameInRoom( int roomNum );

	//SC_LOBBY_GAME_START
	BOOL SendLobbyGameStart( int roomNum );

	//SC_LOBBY_UPDATE_USERDATA
	BOOL SendLobbyUpdateUserData( SPacket& packet );

	//SC_LOBBY_GAME_END
	BOOL SendLobbyGameEnd( int roomNum );

	//SC_LOBBY_GAME_SELF_DISCONNECT
	BOOL SendLobbySelfDisconnect();

	//SC_LOBBY_PLAYER_DISCONNECT
	BOOL SendPlayerDisconnect();
	BOOL SendPlayerDisconnect( int sessionId );
};
