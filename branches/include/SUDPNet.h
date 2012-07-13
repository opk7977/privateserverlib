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
	//���������� ���̴� index
	int				m_index;
	//ĳ������ id
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
	//�������� player�� list	
	SList<SockAddr*>					m_clientList;

	//��Ŷ ť Ŭ����
	//SPacketQueue*						m_packetQ;
	UPacketQueue*						m_packetQ;

	//���� flag
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
	//���� ������ ��� ������ �ʱ�ȭ/ �ڽ��� ���ϵ� �ʱ�ȭ
	void ClientClear();
	//������ �����Լ�
	BOOL Run();

	//��� Ŭ�󿡰� ��Ŷ�� ������.
	void SendPacketAllClient( SPacket& packet );
};

#define GetUNet SUDPNet::GetInstance()

