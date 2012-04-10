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
	//�������ڵ�� id
	HANDLE		m_recvThread;
	int			m_iThread;

	SPacket		m_recvPack;

private:
	Network(void);
	~Network(void);

	//�ʱ�ȭ/����
	void Init( BOOL isNon = TRUE );
	void Release();

	//���� ����
	void ConnectToSrv( char* ipAddr, int port );
	void Reconnect( char* ipAddr, int port );
	void DisConnect();

	//������ �����Լ�
	unsigned int RecvEvent();

	//��Ŷ id �ؼ�
	//void PacketParsing( SPacket& packet );

	//������
	int SendPacket( SPacket* packet );

};

#define GetNetwork Network::GetInstance()

