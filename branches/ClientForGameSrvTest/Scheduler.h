#pragma once

#define MAXPROCESS_PER_FRAME 30


#include "SPacket.h"
#include "STime.h"

class CScheduler : public SSingleton <CScheduler>
{
private:
	friend class SSingleton<CScheduler>;

private:
	SPacket m_packet;

	HANDLE m_schThread;
	HANDLE m_startWork;

	STime m_timer;
	float m_frame;

private:
	CScheduler(void);
	~CScheduler(void);

public:
	void Init();

	//쓰레드 동작함수
	unsigned int Run();

	void PacketProcess();
	void PacketParsing();

	//패킷을 처리하는 함수

	//======================================
	// LOGIN
	//======================================

	//======================================
	// LOBBY
	//======================================
	
	//======================================
	// GAME
	//======================================
	//SC_GAME_CONNECT_OK
	void RecvGameConnectOK();

	//SC_GAME_CHARINFO_INGAME
	void RecvGameCharInfoInGame();

	//SC_GAME_START_GAME

	//SC_GAME_MOVE_CHAR
	void RecvGameMoveChar();

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

#define GetScheduler CScheduler::GetInstance()
