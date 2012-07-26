#ifndef __WORKTHREAD__
#define __WORKTHREAD__

#include "SThread.h"

class SSessionMgr;

class SWorkThread : public SThread
{
	//---------------------------------------
	// �� Ŭ������ IOCP class������ ���ȴ�
	//---------------------------------------

private:
	//======================================
	// SingleTon ��ü
	//======================================
	SSessionMgr*	m_sessionMgr;

private:
	//IOCP���� ����� �� �ֵ��� friend������ �� ��� �Ѵ�.
	friend class	SegIOCP;
	static BOOL		m_bThreadLoop;

private:
	//�ܺο��� ������ �����Ƿ� private������ �� �ش�.
	SWorkThread(void);
	virtual ~SWorkThread(void);

public:
	BOOL Init();
	BOOL Run();
	void EndThread();
};


#endif
