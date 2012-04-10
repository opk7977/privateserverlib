#ifndef __ACCEPTOR__
#define __ACCEPTOR__

#include "SThread.h"
#include "SSocket.h"

class SAcceptor : public SThread
{
private:
	//acceptor���� ����
	SSocket		m_srvSock;

public:
	static BOOL m_bThreadLoop;

public:
	SAcceptor(void);
	virtual ~SAcceptor(void);

	BOOL Init();
	//���� accept�غ�
	BOOL SetAcceptor( int port, int backlogCount  = SOMAXCONN );
	//accept�� Ȯ���ϰ� accept�� �Ǹ� �۾��� ����
	BOOL Run();
	void Release();
};


#endif


