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
	//�� ����
	CharObj*			m_myCharInfo;
	//�� ���� ���μ���(��)
	GameProc*			m_myGameProc;

	//������ �����ǰ�?(�κ�� ������ �ƴϴ�)
	BOOL				isEndGame;

public:
	GameSession(void);
	~GameSession(void);

	void Init();

	void OnCreate();
	void OnDestroy();

	//�� ������ ��´�
	void PackageMyInfo( SPacket& packet );

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

	//--------------------------------------
	// client���� Ŀ�´����̼�
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
	// ���� ��Ŷ ó��
	//======================================
	//--------------------------------------
	// lobby �������� Ŀ�´����̼�
	//--------------------------------------
	//GL_CONNECT_SERVER

	//GL_START_OK

	//GL_PLAYER_DISCONNECT

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//SC_GAME_CONNECT_OK
	//�Լ� ����

	//SC_GAME_CHARINFO_INGAME
	BOOL SendOtherCharInfoToMe();				//���ӳ��� ������ ������� ������ �޴´�
	BOOL SendMyCharInfoToInGamePlayer();		//���ӳ��� ����鿡�� �� ������ ������

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
