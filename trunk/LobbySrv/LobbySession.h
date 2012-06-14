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
//class CheckDB;
class SrvNet;
class DBSrvMgr;
class SLogger;

class LobbySession : public SSessionObj
{
private:
	//======================================
	// singleTon객체
	//======================================
	SSessionMgr*	m_sessionMgr;
	DataLeader*		m_document;
	LobbyMgr*		m_lobbyMgr;
	RoomMgr*		m_roomMgr;
	CharMgr*		m_charMgr;
	//CheckDB*		m_DBMgr;
	SrvNet*			m_srvNet;
	DBSrvMgr*		m_dbMgr;
	SLogger*		m_logger;

private:
	//--------------------------------------
	// 유저정보
	//--------------------------------------
	LobbyChar*		m_myCharInfo;
	//--------------------------------------
	// 방정보
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

	//정보들 초기화
	void clear();

	Room*		GetMyRoom() const { return m_myRoom; }
	LobbyChar*	GetMyInfo() const { return m_myCharInfo; }

	//======================================
	// 패킷 해석 
	//======================================
	void PacketParsing( SPacket& packet );

	//======================================
	// 보내는 함수
	//======================================
//	int SendPacket( SPacket& packet, BOOL tudp = FALSE );

	//======================================
	// 받은 패킷 처리 함수
	//======================================
	//--------------------------------------
	// DBSrv와의 커뮤니케이션
	//--------------------------------------
	//DB_TO_OTHER_CONNECT_OK
	void RecvDBConnectOK();

	//DB_TO_LOBBY_CHARACTER_LOGIN
	void RecvCharacterLogin( SPacket& packet );

	//DB_TO_OTHER_DROP_PLAYER

	//--------------------------------------
	// GameSrv와의 커뮤니케이션
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
	// client와의 커뮤니케이션
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

	//CS_ROOM_TEAM_CHANGE
	void RecvTeamChange();

	//CS_LOBBYSRV_ALL_CHAT
	void RecvAllChat( SPacket& packet );

	//CS_LOBBYSRV_TARGET_CHAT
	void RecvTargetChat( SPacket& packet );

	//CS_ROOM_START
	void RecvRoomStartGame();

	//======================================
	// 보내는 패킷 생성함수
	//======================================
	//--------------------------------------
	// DBSrv와의 커뮤니케이션
	//--------------------------------------
	//LOBBY_TO_DB_CHARINSERT_READY_RESULT
	BOOL SendToDBCharInsertReadyResult( int index, int sessionId );

	//OTHER_TO_DB_DISCONNECT_CHARACTER
	BOOL SendToDBCharDiscconect( int sessionID );

	//--------------------------------------
	// GameSrv의 커뮤니케이션
	//--------------------------------------
	//LG_START_GAME
	BOOL SendCreateGameProc();

	//--------------------------------------
	// client와의 커뮤니케이션
	//--------------------------------------
	//SC_LOBBY_CONNECT_OK
	//함수 필요 없음!

	//SC_LOBBY_OTHER_CHARINFO
	BOOL SendOtherCharInfo();		//나에게 모두의 정보를 보낸다.
	BOOL SendMyCharInfo();			//남에게 입장한 나의 정보를 보낸다.

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

	//SC_ROOM_CHAR_READY
	BOOL SendRoomCharReady();

	//SC_ROOM_MAP_CHANGE
	BOOL SendRoomMapChange();

	//SC_LOBBY_MAP_CHANGE
	BOOL SendLobbyMapChange();

	//SC_ROOM_MODE_CHANGE
	BOOL SendRoomModeChange();

	//SC_LOBBY_MODE_CHANGE
	BOOL SendLobbyModeChange();

	//SC_ROOM_TEAM_CHANGE
	BOOL SendRoomTeamChange();

	//SC_LOBBYSRV_ALL_CHAT
	BOOL SendAllChat( TCHAR* chat );
	
	//SC_LOBBYSRV_TARGET_CHAT
	BOOL SendTargetChatToTarget( int target, TCHAR* chat );
	BOOL SendTargetChatToMe( int target, TCHAR* chat );

	//시작 버튼에 관련된 함수를 호출하는 함수
	BOOL SendStartBtnForVisible( int oldReadyCount );
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

	//SC_LOBBY_GAME_END
	BOOL SendLobbyGameEnd( int roomNum );

	//SC_LOBBY_PLAYER_DISCONNECT
	BOOL SendPlayerDisconnect();
	BOOL SendPlayerDisconnect( int sessionId );
};
