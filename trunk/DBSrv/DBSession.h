#pragma once

#include "SSessionObj.h"
#include "SPacket.h"

class SrvMgr;
class DBMgr;

class PlayerObj;
class PlayerMgr;

class DBSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(DBSession)
	SDECLARE_DYNCREATE(DBSession)

private:
	//======================================
	// SrvMgr��
	//======================================
	static SrvMgr*			m_login;
	static SrvMgr*			m_lobby;
	static SrvMgr*			m_game;

	//======================================
	// single Ton��
	//======================================
	static DBMgr*			m_dbMgr;
	static PlayerMgr*		m_playerMgr;

public:
	DBSession(void);
	virtual ~DBSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// �޾Ƽ� ó��
	//======================================
	//LOG_SERVER_CONNECT_OK

	//LOG_SERVER_SHOTDOWN

	//SC_LOGIN_CONNECT_OK
	void RecvLoginConnectOK();

	//SC_LOBBY_CONNECT_OK
	void RecvLobbyConnectOK();

	//SC_GAME_CONNECT_OK
	void RecvGameConnectOK();

	//LOGIN_TO_DB_CHECK_ID
	void RecvLoginCheckID( SPacket& packet );

	//LOGIN_TO_DB_CREATE_ACCOUNT
	void RecvLoginCreateAccount( SPacket& packet );

	//LOGIN_TO_DB_TRYLOGIN
	void RecvLoginTryLogin( SPacket& packet );

	//GAME_TO_DB_UPDATE_USERDATA
	

	//OTHER_TO_DB_DISCONNECT_CHARACTER

	//======================================
	// ������ ó��
	//======================================
	//RECV_SERVER_LOG

	//DB_TO_OTHER_CONNECT_OK
	//�Լ��� ����

	//DB_TO_LOGIN_CHECK_ID_RESULT
	BOOL SendLoginCheckIDResult( int indexId, BOOL result );

	//DB_TO_LOGIN_CREATE_ACCOUNT_RESULT
	BOOL SendLoginCreateAccountResult( int indexId, BOOL result );

	//DB_TO_LOBBY_CHARACTER_LOGIN
	BOOL SendLobbyChacterLogin( PlayerObj* playe );

	//DB_TO_LOGIN_LOGINRESULT
	BOOL SendLoginTryLoginResult( int indexId, int result );
	
	//DB_TO_OTHER_DROP_PLAYER

	//DB_TO_GAME_LEVEL_UP_PLAYER

	//DB_TO_LOBBY_UPDATE_USERDATA
};