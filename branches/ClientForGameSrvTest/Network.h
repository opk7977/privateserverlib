#pragma once

#include "SSocket.h"
#include "SPacket.h"

#include "NetProtocol.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	SSocket			m_conSock;

	HANDLE			m_startWork;
	//�������ڵ�� id
	HANDLE			m_recvThread;
	unsigned int	m_iThread;

	SPacket			m_recvPack;

private:
	Network(void);
	~Network(void);

public:
	//�ʱ�ȭ/����
	BOOL Init( BOOL isNon = TRUE );
	void Release();

	//���� ����
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL Reconnect( char* ipAddr, int port );
	void DisConnect();

	//������ �����Լ�
	unsigned int RecvEvent();

	//��Ŷ id �ؼ�
	//void PacketParsing( SPacket& packet );

	//������
	int SendPacket( SPacket* packet );

};

#define GetNetwork Network::GetInstance()

