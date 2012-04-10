#ifndef __NETWORK__
#define __NETWORK__


#include "SSocket.h"
#include "SPacket.h"

class Network
{
	SSocket		m_mySock;
	HANDLE		m_recvThread;
	HANDLE		m_startWork;

	SPacket		m_recvPack;

public:
	Network(void);
	virtual ~Network(void);


	//���� �ʱ�ȭ �� ����//������ �ͺ����� �����Ϸ��� TRUE�� �־� �ش�
	BOOL Init( BOOL isNon = FALSE );

	//������ ����
	BOOL ConnectSrv( char* ipAddr, int port );

	//���� ����
	void Release();

	//--------------------------------------
	// Recv�ϴ� ������ �Լ�
	//--------------------------------------
	unsigned int RecvEvent();


	//======================================
	// ��Ŷ�� �޾� ��Ŷ�� ���� ó���ϴ� �Լ��� ȣ��
	//======================================
	void PacketParsing( SPacket* packet );
	//////////////////////////////////////////////////////////////////////////
	//--------------------------------------
	// ��Ŷ�� ó���ϴ� �Լ���
	//--------------------------------------
	//ó�� �����ϰ� ĳ���͸� �����϶�� ��Ŷ�� �޾� ó���ϴ� �Լ�
	void CreateCharacter( SPacket* packet );
	//npc�� ���°� ���ؼ� ��ȭ�� �޾ƿ� ó���ϴ� �Լ�
	void ChangeCharacter( SPacket* packet );
};



#endif

