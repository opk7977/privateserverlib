#ifndef __ACCEPTOR__
#define __ACCEPTOR__

#include "SThread.h"
#include "SSocket.h"

class SAcceptor : public SThread
{
private:
	//acceptor서버 소켓
	SSocket		m_srvSock;

public:
	static BOOL m_bThreadLoop;

public:
	SAcceptor(void);
	virtual ~SAcceptor(void);

	BOOL Init();
	//소켓 accept준비
	BOOL SetAcceptor( int port, int backlogCount  = SOMAXCONN );
	//accept를 확인하고 accept가 되면 작업을 시작
	BOOL Run();
	void Release();
};


#endif


