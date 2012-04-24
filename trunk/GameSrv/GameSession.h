#pragma once

#include "SSessionObj.h"

class GameSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(GameSession)
	SDECLARE_DYNCREATE(GameSession)

public:
	GameSession(void);
	~GameSession(void);

	void OnCreate();
	void OnDestroy();

	void PacketParsing( SPacket& packet );

	//======================================
	// ���� ��Ŷ ó��
	//======================================
	//--------------------------------------
	// lobby �������� Ŀ�´����̼�
	//--------------------------------------
	//LG_START_GAME

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//CS_GAME_INGAME

	//CS_GAME_MOVE_CHAR

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

	//GL_PLAYER_DISCONNECT

	//--------------------------------------
	// client���� Ŀ�´����̼�
	//--------------------------------------
	//SC_GAME_CONNECT_OK

	//SC_GAME_CHARINFO_INGAME

	//SC_GAME_START_GAME

	//SC_GAME_MOVE_CHAR

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

};
