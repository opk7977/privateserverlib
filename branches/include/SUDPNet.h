#pragma once

#include "SThread.h"
#include "SSocket.h"
#include "SPacket.h"
#include "SIndexQueue.h"
#include "SList.h"

//class SPacketQueue;
class UPacketQueue;

struct SockAddr
{
private:
	//내부적으로 쓰이는 index
	int				m_index;
	//캐릭터의 id
	int				m_sessionID;

public:
	sockaddr_in		m_sockAddr;

public:
	SockAddr( int i )
	{
		m_index = i;
		Clear();
	}
	void Clear()
	{
		m_sessionID = 0;
		ZeroMemory( &m_sockAddr, sizeof( SOCKADDR ) );
	}
	int GetIndex() { return m_index; }

	void SetSessionID( int id ) { m_sessionID = id; }
	int GetSessionID() { return m_sessionID; }
};

class SUDPNet : public SSingleton <SUDPNet>, public SThread
{
private:
	friend class SSingleton<SUDPNet>;

private:
	SSocket								m_conSock;
	SPacket								m_recvPack;

	ATL::CAtlMap<int, SockAddr*>		m_clientSpace;
	SIndexQueue							m_clientIndexQ;
	//게임중인 player의 list	
	SList<SockAddr*>					m_clientList;

	//패킷 큐 클레스
	//SPacketQueue*						m_packetQ;
	UPacketQueue*						m_packetQ;

	//연결 flag
	BOOL								m_isConnect;
	BOOL								m_gameStart;
	
private:
	SUDPNet(void);
	~SUDPNet(void);

public:
	BOOL Init( int port );
	void Release();

	BOOL AddSockAddr( int sessionId, char* _ip, int port );
	BOOL DelSockAddr( int sessionId );
	//게임 종료후 모든 정보를 초기화/ 자신의 소켓도 초기화
	void ClientClear();
	//쓰레드 동작함수
	BOOL Run();

	//모든 클라에게 패킷을 보낸다.
	void SendPacketAllClient( SPacket& packet );
};

#define GetUNet SUDPNet::GetInstance()

