#pragma once

#include "SAcceptor.h"

class Network : public SSingleton <Network>
{
private:
	friend class SSingleton<Network>;

private:
	//���� �ڽ��� �����
	SAcceptor	m_acceptor;

public:
	Network(void);
	~Network(void);

	//�������� �ʱ�ȭ
	BOOL Init( int count );

	//��������
	BOOL SettingSrv( WORD port );

	//���� ����
	void Release();
};

#define GetNetwork Network::GetInstance()
