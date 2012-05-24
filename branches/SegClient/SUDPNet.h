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
	//�������� player�� list	
	SList<SockAddr*>					m_clientList;

	//��Ŷ ť Ŭ����
	SPacketQueue*						m_packetQ;

	//���� flag
	BOOL								m_gameStart;
	
private:
	SUDPNet(void);
	~SUDPNet(void);

	BOOL Init();
	BOOL AddSockAddr( char* _ip, int port );
	//���� ������ ��� ������ �ʱ�ȭ/ �ڽ��� ���ϵ� �ʱ�ȭ
	void ClientClear();
	//������ �����Լ�
	BOOL Run();

	//��� Ŭ�󿡰� ��Ŷ�� ������.
	void SendPacketAllClient( SPacket& packet );

public:
};
