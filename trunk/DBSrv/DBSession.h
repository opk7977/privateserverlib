#pragma once

#include "SSessionObj.h"
#include "SPacket.h"

class SrvMgr;
class DataBaseObj;
//class DBMgr;

class PlayerObj;
class PlayerMgr;

class DataLeader;
class SLogger;
class LogSrvMgr;

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
	// Log
	//======================================
	static SLogger*			m_logger;
	static LogSrvMgr*		m_logSrv;

	//======================================
	// single Ton��
	//======================================
	static DataLeader*		m_document;
	//static DBMgr*			m_dbMgr;
	static DataBaseObj*		m_dbMgr;
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
	void RecvLogSrvConnectOK();

	//LOG_SERVER_SHOTDOWN
	void RecvLogServerShotdown();

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

	//LOBBY_TO_DB_CHARINSERT_READY_RESULT
	void RecvLobbyCharInsertReadyResult( SPacket& packet );

	//GAME_TO_DB_UPDATE_USERDATA
	void RecvGameCharDataUpdate( SPacket& packet );
	
	//OTHER_TO_DB_DISCONNECT_CHARACTER
	void RecvOtherSrvCharDisconnect( SPacket& packet );

	//======================================
	// ������ ó��
	//======================================
	//RECV_SERVER_LOG
	//LogSrvMgr���� ó��

	//DB_TO_OTHER_CONNECT_OK
	//�Լ��� ����

	//DB_TO_LOGIN_CHECK_ID_RESULT
	BOOL SendLoginCheckIDResult( int indexId, BOOL result );

	//DB_TO_LOGIN_CREATE_ACCOUNT_RESULT
	BOOL SendLoginCreateAccountResult( int indexId, BOOL result );

	//DB_TO_LOBBY_CHARACTER_LOGIN
	BOOL SendLobbyChacterLogin( int index, PlayerObj* playe );

	//DB_TO_LOGIN_LOGINRESULT
	BOOL SendLoginTryLoginResult( int indexId, int result );
	
	//DB_TO_OTHER_DROP_PLAYER
	BOOL SendLobbyNGameDropPlayer( int sessionID );

	//DB_TO_GAME_LEVEL_UP_PLAYER
	////RecvOtherSrvCharDisconnect�Լ� ���ο��� �׳� �θ�

	//DB_TO_LOBBY_UPDATE_USERDATA
	//RecvOtherSrvCharDisconnect�Լ� ���ο��� �׳� �θ�
};
