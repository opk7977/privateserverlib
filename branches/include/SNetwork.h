#pragma once

#include "SSocket.h"
#include "SPacket.h"

#include "SThread.h"

class SNetwork : public SSingleton <SNetwork>, public SThread
{
private:
	friend class SSingleton<SNetwork>;

private:
	SSocket			m_conSock;
	SPacket			m_recvPack;

	//쓰레드 동작 핸들이벤트
	HANDLE			m_hStartEvent;

private:
	SNetwork(void);
	~SNetwork(void);

public:
	//소켓 생성시 nonblock설정flag
	BOOL Init( BOOL isNon = TRUE );
	void Release();

	//연결
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL ReConnect( char* ipAddr, int port );
	void DisConnect();

	//쓰레드 동작함수
	BOOL Run();

	//보내기 함수
	int SendPacket( SPacket* packet );

};

#define GetNetwork SNetwork::GetInstance()
