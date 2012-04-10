#pragma once

#include "SSocket.h"
#include "SPacket.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;
private:
	SSocket		m_srvSock;
	HANDLE		m_recvThread;
	HANDLE		m_startWork;

	SPacket		m_recvPack;

private:
	Network(void);
	~Network(void);

public:
	//연결 소켓 초기화
	BOOL Init( BOOL isNon = FALSE );

	//네트워크에 연결
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL Reconnect( char* ipAddr, int port );

	//네트워크 연결 종료
	void DisConnect();

	//쓰레드 종;료
	void Release();

	//쓰레드가 동작하는 recv함수
	unsigned int RecvEvent();

	void PacketParsing( SPacket* packet );

	//send
	int SendPacket( SPacket* packet );

	//======================================
	// Recv관련 사용자 함수들
	//		: 클라가 데이터를 받는 함수
	//======================================
	//SC_TGAME_CONNECT_OK
	void RecvConnectOK();

	//SC_TGAME_CREATECHAR
	void RecvCreateCharacter( SPacket* packet );

	//SC_TGAME_STARTGAME
	void RecvStartGame();

	//SC_TGAME_STATEROTATE
	void RecvChangeRotation( SPacket* packet );

	//SC_TGAME_STATECHANGE
	void RecvChangeState( SPacket* packet );

	//SC_TGAME_DELETECHAR
	void RecvDeleteCharacter( SPacket* packet );

	//SC_TGAME_COUNTDWON_TIME
	void RecvTimeCountDwon();

	//SC_TGAME_ENDGAME
	void RecvEndGame();

	//======================================
	// Send관련 사용자 함수들
	//		: 클라가 사용하는 함수
	//======================================
	//CS_TGAME_SENDINFO
	BOOL SendMyInfo( int sessionId, int roomNo );

	//CS_TGAME_READYOK
	BOOL SendReadyOK();

	//CS_TGAME_STATEROTATE
	BOOL SendChangeRotation( int sessionId, int rot );

	//CS_TGAME_STATECHANGE
	BOOL SendChangeState( int sessionId, int state, float dirX, float dirY, float dirZ, float posX, float posY, float posZ );

};

#define GetNetwork Network::GetInstance()

