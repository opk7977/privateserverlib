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

	//������ ���� �ڵ��̺�Ʈ
	HANDLE			m_hStartEvent;

private:
	SNetwork(void);
	~SNetwork(void);

public:
	//���� ������ nonblock����flag
	BOOL Init( BOOL isNon = TRUE );
	void Release();

	//����
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL ReConnect( char* ipAddr, int port );
	void DisConnect();

	//������ �����Լ�
	BOOL Run();

	//������ �Լ�
	int SendPacket( SPacket* packet );

};

#define GetNetwork SNetwork::GetInstance()
