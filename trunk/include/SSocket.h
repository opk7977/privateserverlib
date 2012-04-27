#ifndef __SOCKET__
#define __SOCKET__


#include "SServerobj.h"


class SSocket : public SServerObj
{
private:
	SOCKET		m_socket;

public:
	SSocket(void);
	virtual ~SSocket(void);
	
	//�ʱ�ȭ
	BOOL Init();

	//run�Լ�
	BOOL Run();

	//release
	void Release();

	//������ ����
	BOOL CreateSocket();

	//ip���� ����
	BOOL SetScokReuseAddr();

	//���� �ͺ������� ����
	BOOL SetNonBlkSock();

	//Ŭ�� ������ ���� connect�Լ�
	BOOL ConnectSock( char* ipAddr, int port, SOCKADDR *sAddr = NULL );

	//���� ������ ���� bind�Լ�
	BOOL BindSocket( int port );
	//���� ������ ���� listen�Լ�
	BOOL SocketListen( int backlogCount = SOMAXCONN );

	//���� ���� �ϱ�
	BOOL CloseSocket();

	//������ ��ȯ
	inline SOCKET GetSocket() { return m_socket; }
};



#endif


