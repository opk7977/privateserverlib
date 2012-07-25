#ifndef __ACCEPTOR__
#define __ACCEPTOR__

#include "SThread.h"
#include "SSocket.h"

class SLogger;
class SSessionMgr;

class SAcceptor : public SThread
{
private:
	//======================================
	// SingleTon ��ü
	//======================================
	SLogger*		m_logger;
	SSessionMgr*	m_sessionMgr;

private:
	//acceptor���� ����
	SSocket			m_srvSock;

public:
	static BOOL		m_bThreadLoop;

public:
	SAcceptor(void);
	virtual ~SAcceptor(void);

	BOOL Init();
	//���� accept�غ�
	BOOL SetAcceptor( int port, int backlogCount  = SOMAXCONN );
	//accept�� Ȯ���ϰ� accept�� �Ǹ� �۾��� ����
	BOOL Run();
	void EndThread();

	void Release();
};


#endif


