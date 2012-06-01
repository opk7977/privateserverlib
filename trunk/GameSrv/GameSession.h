#pragma once

#include "SSessionObj.h"

//--------------------------------------
// singleTon
//--------------------------------------
class SrvNet;
class SLogger;
class GameMgr;
class CharMgr;
class ItemMgr;
//--------------------------------------
class CharObj;
class GameProc;

class GameSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(GameSession)
	SDECLARE_DYNCREATE(GameSession)

private:
	//======================================
	// singleTon객체들
	//======================================
	SrvNet*				m_srvNet;
	SLogger*			m_logger;
	GameMgr*			m_gameMgr;
	CharMgr*			m_charMgr;
	ItemMgr*			m_itemMgr;
	//======================================

	//내 정보
	CharObj*			m_myCharInfo;
	//내 게임 프로세스(방)
	GameProc*			m_myGameProc;


	//게임을 끝낸건가?(로비로 간것이 아니다)
	BOOL				isEndGame;

public:
	GameSession(void);
	~GameSession(void);

	void Clear();

	void OnCreate();
	void OnDestroy();

	CharObj*	GetMyInfo() const { return m_myCharInfo; }
	GameProc*	GetMyGame() const { return m_myGameProc; }

	//내 정보를 담는다
	//void PackageMyInfo( SPacket& packet );
	//내 ip정보를 담자
	void PackageMyNetInfo( SPacket& packet );

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
	void RecvLobbyStartGame( SPacket &packet );

	//--------------------------------------
	// client와의 커뮤니케이션
	//--------------------------------------
	//CS_GAME_INGAME
	void RecvInGame( SPacket &packet );

	//CS_GAME_GAME_READY_OK
	void RecvGameReadyOK();

	//CS_GAME_MOVE_CHAR
	//void RecvMoveChar( SPacket &packet );

	//CS_GAME_ATTACK
	void RecvGameAttec( SPacket &packet );

	//CS_GAME_TRY_ATTACK
	void RecvGameTryAttact( SPacket &packet );

	//CS_GAME_SYNC
	//void RecvGameSync( SPacket &packet );

	//CS_GAME_CHANGE_STATE
	//void RecvGameChangeState( SPacket &packet );

	//CS_GAME_ROTATION
	//void RecvGameRotation( SPacket &packet );

	//CS_GAME_CHATTING
	void RecvGameChatting( SPacket &packet );

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
	BOOL SendConnectOK();

	//GL_START_FAILD
	BOOL SendStartFaild( int roomNum );

	//GL_START_OK
	BOOL SendStartOK( int roomNum );

	//GL_GAME_END
	BOOL SendGameEnd( int roomNum );

	//GL_PLAYER_DISCONNECT
	BOOL SendToSrvThatPlayerDisconnect();

	//--------------------------------------
	// client와의 커뮤니케이션
	//--------------------------------------
	//SC_GAME_CONNECT_OK
	//함수 없음

	//SC_GAME_CHARINFO_INGAME
	BOOL SendOtherCharInfoToMe();				//게임내에 접속한 사람들의 정보를 받는다
	BOOL SendMyCharInfoToInGamePlayer();		//게임내의 사람들에게 내 정보를 보낸다

	//SC_GAME_START_GAME
	//게임Proc에 있음

	//SC_GAME_MOVE_CHAR
	//BOOL SendMoveChar();

	//SC_GAME_ATTACK
	BOOL SendGameAttact( int weapon, CharObj* attactedChar );

	//SC_GAME_YOU_ATTACKED
	BOOL SendGameYouAttact( int weapon, int damage, CharObj* attactedChar );

	//SC_GAME_TRY_ATTACK
	BOOL SendGameTryAttact( SPacket& packet );

	//SC_GAME_CHAR_DIE /SC_GAME_YOU_DIE
	BOOL SendGameDie( int weapon, CharObj* dieChar );

	//SC_GAME_SYNC
	//BOOL SendGameSync();

	//SC_GAME_CHANGE_STATE
	//BOOL SendGameChangeState();

	//SC_GAME_ROTATION
	//BOOL SendGameRotation();

	//SC_GAME_CHATTING
	BOOL SendGameChatting( TCHAR* chatting, int size );

	//SC_GAME_INSTALL_BOOM

	//SC_GAME_BOOM_COUNTDOWN

	//SC_GAME_UNINSTALL_BOOM

	//SC_GAME_TIME_COUNTDOWN
	//게임Proc에 있음

	//SC_GAME_END

	//SC_GAME_INSTALL_ITEM

	//SC_GAME_RUN_ITEM

	//SC_GAME_GOTO_LOBBY

	//SC_GAME_CHAR_GOTO_LOBBY

	//SC_GAME_CHAR_DISCONNECT
	BOOL SendCharDisconnect();

};
