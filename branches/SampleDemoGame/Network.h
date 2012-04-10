#pragma once

#include "SSocket.h"
#include "SPacket.h"


class CNetwork : public SSingleton <CNetwork>
{
private:
	friend class SSingleton<CNetwork>;

private:
	SSocket		m_srvSock;
	HANDLE		m_recvThread;
	HANDLE		m_startWork;

	SPacket		m_recvPack;


private:
	CNetwork(void);
	~CNetwork(void);

public:
	//���� �ʱ�ȭ
	BOOL Init( BOOL isNon = FALSE );

	//��Ʈ��ũ ����
	BOOL ConnectNet( char* ipAddr, int port );

	//�ٽ� ����
	BOOL ReConnectNet( char* ipAddr, int port );

	//��Ʈ��ũ ���� ����
	void DisConnect();

	void Release();

	//�����忡�� ���ư� Recv�Լ�
	unsigned int RecvEvent();
	//���� ��Ŷ id�� �м��ϴ� �Լ�
	void PacketParsing( SPacket* packet );

	//Send�Լ�
	int SendPacket( SPacket* packet );

	//======================================
	// Send ���� ����� �Լ���
	//======================================
	// id�� pw�� ����� ������ �ѱ��
	BOOL SendLogin( TCHAR* id, TCHAR* pw );

	// ������ �Ǿ����� ���� ID�� ������ ĳ���͸� �Ҵ�(?)�޴´�
	BOOL SendSessionId( int sessionID, TCHAR* charID );

	// �ڽ��� �����ӿ� ���� ��ȭ�� ������ �ѱ��.
	BOOL SendChangeState( int indexId, int state, int x, int y );

	//======================================
	// Recv ���� ����� �Լ���
	//======================================
	// �α��� ���� ���� �Ϸ� ��Ŷ�� ������ �����ϴ� �Լ�
	void RecvConnectComplete();
	
	// �α����� ����� �޾� �����ϴ� �Լ�
	void RecvResultNonId();
	void RecvResultNotPw();
	void RecvResultLogin( SPacket* packet );

	//////////////////////////////////////////////////////////////////////////
	// ���Ӽ����� ������ ���������� ��
	void RecvConnectCompleteToG();

	// ���Ӽ����� ������ ���������� �ʾ� ������ ���� �ؾ� ��
	void RecvConnectFailed();
	
	// ĳ���͵��� �����϶�� ����� �޾� �����ϴ� �Լ�
	void RecvCreateCharacter( SPacket* packet );

	// �������� �ִ� ĳ���Ϳ� ���� ��Ŷ�� �޾� �����ϴ� �Լ�
	void RecvMoveCharacter( SPacket* packet );

	//�ٸ� ĳ���Ͱ� ������ ���� �ߴ�
	void RecvDeleteCharacter( SPacket* packet );
};

#define GetNetwork CNetwork::GetInstance()


