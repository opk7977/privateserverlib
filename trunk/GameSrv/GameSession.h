#pragma once

#include "SSessionObj.h"

class CharObj;
class GameProc;

class GameSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(GameSession)
	SDECLARE_DYNCREATE(GameSession)

private:
	//내 정보
	CharObj*			m_myCharInfo;
	//내 게임 프로세스(방)
	GameProc*			m_myGameProc;

	//게임을 끝낸건가?(로비로 간것이 아니다)
	BOOL				isEndGame;

public:
	GameSession(void);
	~GameSession(void);

	void Init();

	void OnCreate();
	void OnDestroy();

	//내 정보를 담는다
	void PackageMyInfo( SPacket& packet );

	//패킷 처리
	void PacketParsing( SPacket& packet );

	//======================================
	// 받은 패킷 처리
	//======================================
	//--------------------------------------
	// lobby 서버와의 커뮤니케이션
	//--------------------------------------
	//SC_LOBBY_CONNECT_OK
	void RecvLobbyConnectOK();

	//LG_START_GAME

	//--------------------------------------
	// client와의 커뮤니케이션
	//--------------------------------------
	//CS_GAME_INGAME
	void RecvInGame( SPacket &packet );

	//CS_GAME_MOVE_CHAR
	void RecvMoveChar( SPacket &packet );

	//CS_GAME_ATTECT

	//CS_GAME_SYNC

	//CS_GAME_INSTALL_BOOM

	//CS_GAME_UNINSTALL_BOOM

	//CS_GAME_INSTALL_ITEM

	//CS_GAME_GOTO_LOBBY


	//======================================
	// 보낼 패킷 처리
	//======================================
	//--------------------------------------
	// lobby 서버와의 커뮤니케이션
	//--------------------------------------
	//GL_CONNECT_SERVER

	//GL_START_OK

	//GL_PLAYER_DISCONNECT

	//--------------------------------------
	// client와의 커뮤니케이션
	//--------------------------------------
	//SC_GAME_CONNECT_OK
	//함수 없음

	//SC_GAME_CHARINFO_INGAME
	BOOL SendOtherCharInfoToMe();				//게임내에 접속한 사람들의 정보를 받는다
	BOOL SendMyCharInfoToInGamePlayer();		//게임내의 사람들에게 내 정보를 보낸다

	//SC_GAME_START_GAME

	//SC_GAME_MOVE_CHAR
	BOOL SendMoveChar();

	//SC_GAME_ATTECT

	//SC_GAME_CHAR_DIE

	//SC_GAME_SYNC

	//SC_GAME_INSTALL_BOOM

	//SC_GAME_BOOM_COUNTDOWN

	//SC_GAME_UNINSTALL_BOOM

	//SC_GAME_TIME_COUNTDOWN

	//SC_GAME_END

	//SC_GAME_INSTALL_ITEM

	//SC_GAME_RUN_ITEM

	//SC_GAME_GOTO_LOBBY

	//SC_GAME_CHAR_GOTO_LOBBY

	//SC_GAME_CHAR_DISCONNECT
	BOOL SendCharDisconnect();

};
