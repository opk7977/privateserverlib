#pragma once

#include "SSocket.h"
#include "SPacket.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;
private:
	SSocket		m_srvSock;
	HANDLE		m_recvThread;
	HANDLE		m_startWork;

	SPacket		m_recvPack;

private:
	Network(void);
	~Network(void);

public:
	//���� ���� �ʱ�ȭ
	BOOL Init( BOOL isNon = FALSE );

	//��Ʈ��ũ�� ����
	BOOL ConnectToSrv( char* ipAddr, int port );
	BOOL Reconnect( char* ipAddr, int port );

	//��Ʈ��ũ ���� ����
	void DisConnect();

	//������ ��;��
	void Release();

	//�����尡 �����ϴ� recv�Լ�
	unsigned int RecvEvent();

	void PacketParsing( SPacket* packet );

	//send
	int SendPacket( SPacket* packet );

	//======================================
	// Recv���� ����� �Լ���
	//		: Ŭ�� �����͸� �޴� �Լ�
	//======================================
	//SC_TGAME_CONNECT_OK
	void RecvConnectOK();

	//SC_TGAME_CREATECHAR
	void RecvCreateCharacter( SPacket* packet );

	//SC_TGAME_STARTGAME
	void RecvStartGame();

	//SC_TGAME_STATEROTATE
	void RecvChangeRotation( SPacket* packet );

	//SC_TGAME_STATECHANGE
	void RecvChangeState( SPacket* packet );

	//SC_TGAME_DELETECHAR
	void RecvDeleteCharacter( SPacket* packet );

	//SC_TGAME_COUNTDWON_TIME
	void RecvTimeCountDwon();

	//SC_TGAME_ENDGAME
	void RecvEndGame();

	//======================================
	// Send���� ����� �Լ���
	//		: Ŭ�� ����ϴ� �Լ�
	//======================================
	//CS_TGAME_SENDINFO
	BOOL SendMyInfo( int sessionId, int roomNo );

	//CS_TGAME_READYOK
	BOOL SendReadyOK();

	//CS_TGAME_STATEROTATE
	BOOL SendChangeRotation( int sessionId, int rot );

	//CS_TGAME_STATECHANGE
	BOOL SendChangeState( int sessionId, int state, float dirX, float dirY, float dirZ, float posX, float posY, float posZ );

};

#define GetNetwork Network::GetInstance()

