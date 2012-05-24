#pragma once

#include "SThread.h"
#include "SIndexQueue.h"
#include "SList.h"

class SPacketQueue;

struct SockAddr
{
private:
	int				m_index;
	sockaddr_in		m_sockAddr;

public:
	SockAddr( int i )
	{
		m_index = i;
		ZeroMemory( m_sockAddr, sizeof( SOCKADDR ) );
	}
	void Clear()
	{
		ZeroMemory( m_sockAddr, sizeof( SOCKADDR ) );
	}
	int GetIndex()
	{
		return m_index;
	}
};

class SUDPNet : public SSingleton <SUDPNet>, public SThread
{
	SSocket								m_conSock;
	SPacket								m_recvPack;

	ATL::CAtlMap<int, SockAddr*>		m_clientSpace;
	SIndexQueue							m_clientIndexQ;
	//게임중인 player의 list	
	SList<SockAddr*>					m_clientList;

	//패킷 큐 클레스
	SPacketQueue*						m_packetQ;

	//연결 flag
	BOOL								m_gameStart;
	
private:
	SUDPNet(void);
	~SUDPNet(void);

	BOOL Init();
	BOOL AddSockAddr( char* _ip, int port );
	//게임 종료후 모든 정보를 초기화/ 자신의 소켓도 초기화
	void ClientClear();
	//쓰레드 동작함수
	BOOL Run();

	//모든 클라에게 패킷을 보낸다.
	void SendPacketAllClient( SPacket& packet );

public:
};
