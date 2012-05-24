#pragma once

#include "SSocket.h"
#include "SPacket.h"

#include "SThread.h"

class SPacketQueue;

class SNetwork : public SSingleton <SNetwork>, public SThread
{
private:
	friend class SSingleton<SNetwork>;

private:
	SSocket			m_conSock;
	SPacket			m_recvPack;

	char			m_sendStream[DEFAULT_BUFFER_SIZE];
	int				m_sendSize;

	//쓰레드 동작 핸들이벤트
	HANDLE			m_hStartEvent;

	//연결상태를 나타내는 BOOL값
	BOOL			m_isConnect;
	//재 연결을 위해 연결을 끊었다는 BOOL값
	BOOL			m_isReConnect;

	SPacketQueue*	m_packetQ;

private:
	SNetwork(void);
	~SNetwork(void);

public:
	//소켓 생성시 nonblock설정flag
	BOOL Init();
	void Release();

	//로그인 서버에 연결
	void ConToLoginSrv( char* ipaddr, int port );

	//연결
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL ReConnect( char* ipAddr, int port );

	//현재 연결 상태인지를 return
	BOOL IsCounnect();
	void SetConnectOK() { m_isConnect = TRUE; }

	void DisConnect();

	//쓰레드 동작함수
	BOOL Run();

	//stream에 쌓는 함수
	BOOL PilePacket( SPacket* packet );
	//보내기 함수
	//int SendPacket( SPacket* packet );
	BOOL SendPacket();

};

#define GetNetwork SNetwork::GetInstance()
