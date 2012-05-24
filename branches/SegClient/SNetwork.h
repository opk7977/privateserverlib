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

	//������ ���� �ڵ��̺�Ʈ
	HANDLE			m_hStartEvent;

	//������¸� ��Ÿ���� BOOL��
	BOOL			m_isConnect;
	//�� ������ ���� ������ �����ٴ� BOOL��
	BOOL			m_isReConnect;

	SPacketQueue*	m_packetQ;

private:
	SNetwork(void);
	~SNetwork(void);

public:
	//���� ������ nonblock����flag
	BOOL Init();
	void Release();

	//�α��� ������ ����
	void ConToLoginSrv( char* ipaddr, int port );

	//����
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL ReConnect( char* ipAddr, int port );

	//���� ���� ���������� return
	BOOL IsCounnect();
	void SetConnectOK() { m_isConnect = TRUE; }

	void DisConnect();

	//������ �����Լ�
	BOOL Run();

	//stream�� �״� �Լ�
	BOOL PilePacket( SPacket* packet );
	//������ �Լ�
	//int SendPacket( SPacket* packet );
	BOOL SendPacket();

};

#define GetNetwork SNetwork::GetInstance()
