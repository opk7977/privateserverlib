#ifndef __WORKTHREAD__
#define __WORKTHREAD__

#include "SThread.h"

class SWorkThread : public SThread
{
	//---------------------------------------
	// 이 클래스는 IOCP class에서만 사용된다
	//---------------------------------------

private:
	//IOCP에서 사용할 수 있도록 friend선언을 해 줘야 한다.
	friend class SegIOCP;
	static BOOL m_bThreadLoop;

private:
	//외부에서 사용되지 않으므로 private선언을 해 준다.
	SWorkThread(void);
	virtual ~SWorkThread(void);

public:
	BOOL Init();
	BOOL Run();
	void Release();
};


#endif
