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
	// singleTon��ü��
	//======================================
	SrvNet*				m_srvNet;
	SLogger*			m_logger;
	GameMgr*			m_gameMgr;
	CharMgr*			m_charMgr;
	ItemMgr*			m_itemMgr;
	//======================================

	//�� ����
	CharObj*			m_myCharInfo;
	//�� ���� ���μ���(��)
	GameProc*			m_myGameProc;


	//������ �����ǰ�?(�κ�� ������ �ƴϴ�)
	BOOL				isEndGame;

public:
	GameSession(void);
	~GameSession(void);

	void Clear();

	void OnCreate();
	void OnDestroy();

	CharObj*	GetMyInfo() const { return m_myCharInfo; }
	GameProc*	GetMyGame() const { return m_myGameProc; }

	//�� ������ ��´�
	//void PackageMyInfo( SPacket& packet );
	//�� ip������ ����
	void PackageMyNetInfo( SPacket& packet );

	//��Ŷ ó��
	void PacketParsing( SPacket& packet );

	//======================================
	// ���� ��Ŷ ó��
	//======================================
	//--------------------------------------
	// lobby �������� Ŀ�´����̼�
	//--------------------------------------
	//SC_LOBBY_CONNECT_OK
	void RecvLobbyConnectOK();

	//LG_START_GAME
	void RecvLobbyStartGame( SPacket &packet );

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//CS_GAME_INGAME
	void RecvInGame( SPacket &packet );

	//CS_GAME_GAME_READY_OK

	//CS_GAME_MOVE_CHAR
	void RecvMoveChar( SPacket &packet );

	//CS_GAME_ATTECT
	void RecvGameAttec( SPacket &packet );

	//CS_GAME_SYNC
	void RecvGameSync( SPacket &packet );

	//CS_GAME_CHANGE_STATE
	void RecvGameChangeState( SPacket &packet );

	//CS_GAME_ROTATION
	void RecvGameRotation( SPacket &packet );

	//CS_GAME_CHATTING
	void RecvGameChatting( SPacket &packet );

	//CS_GAME_INSTALL_BOOM

	//CS_GAME_UNINSTALL_BOOM

	//CS_GAME_INSTALL_ITEM

	//CS_GAME_GOTO_LOBBY


	//======================================
	// ���� ��Ŷ ó��
	//======================================
	//--------------------------------------
	// lobby �������� Ŀ�´����̼�
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
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//SC_GAME_CONNECT_OK
	//�Լ� ����

	//SC_GAME_CHARINFO_INGAME
	BOOL SendOtherCharInfoToMe();				//���ӳ��� ������ ������� ������ �޴´�
	BOOL SendMyCharInfoToInGamePlayer();		//���ӳ��� ����鿡�� �� ������ ������

	//SC_GAME_START_GAME
	//����Proc�� ����

	//SC_GAME_MOVE_CHAR
	BOOL SendMoveChar();

	//SC_GAME_ATTECT
	BOOL SendGameAttec( int weapon, int attectedSessionID, int part, int damage );

	//SC_GAME_CHAR_DIE

	//SC_GAME_SYNC
	BOOL SendGameSync();

	//SC_GAME_CHANGE_STATE
	BOOL SendGameChangeState();

	//SC_GAME_ROTATION
	BOOL SendGameRotation();

	//SC_GAME_CHATTING
	BOOL SendGameChatting( TCHAR* chatting, int size );

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
