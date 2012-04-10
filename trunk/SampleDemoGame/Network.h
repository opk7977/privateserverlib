#pragma once

#include "SAcceptor.h"
#include "SSocket.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	//���� �ڽ��� �����
	SAcceptor	m_accepter;

public:
	Network(void);
	~Network(void);

	//�������� & �������� MGR�ʱ�ȭ
	BOOL	Init( int count );

	//���� ����
	BOOL	SettingSrv( WORD port );

	//���� ����
	void	Release();
};

#define GetNetwork Network::GetInstance()

