#pragma once

#include "SSocket.h"
#include "SPacket.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	SSocket		m_conSock;

	HANDLE		m_startWork;
	//쓰레드핸들과 id
	HANDLE		m_recvThread;
	int			m_iThread;

	SPacket		m_recvPack;

private:
	Network(void);
	~Network(void);

	//초기화/해제
	void Init( BOOL isNon = TRUE );
	void Release();

	//연결 관련
	void ConnectToSrv( char* ipAddr, int port );
	void Reconnect( char* ipAddr, int port );
	void DisConnect();

	//쓰레드 동작함수
	unsigned int RecvEvent();

	//패킷 id 해석
	//void PacketParsing( SPacket& packet );

	//보내기
	int SendPacket( SPacket* packet );

};

#define GetNetwork Network::GetInstance()

