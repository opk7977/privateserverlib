#ifndef __NETWORK__
#define __NETWORK__

#include "SAcceptor.h"

class Network
{
private:
	SAcceptor	m_accepter;

public:
	Network(void);
	~Network(void);

	//���� ���� accepter�� iocp�� �ʱ�ȭ �ϰ�
	BOOL	Init( int count );

	//���� ����
	BOOL	SettingSrv( WORD port );
	
	//���� ����
	void	Release();

	//////////////////////////////////////////////////////////////////////////

};

#endif

