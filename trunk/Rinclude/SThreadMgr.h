#ifndef __THREAD_MANAGER__
#define __THREAD_MANAGER__

#include "SServerStd.h"

//singleton
class SThreadMgr : public SSingleton <SThreadMgr>
{
private:
	friend class SSingleton<SThreadMgr>;

private:
	//쓰레드 list
	std::list<HANDLE> m_listThread;

private:
	SThreadMgr(void);
	virtual ~SThreadMgr(void);

public:
	//-----------------------------------------------
	// 모든 쓰레드가 종료되는 것을 확인하고 기다린다.
	// 모든 쓰레드가 종료되면 return 됨
	//-----------------------------------------------
	void IsEndAllThread();

	//-----------------------------------------------
	// 모든 쓰레드 강제 종료
	//-----------------------------------------------
	BOOL EndAllThread();

	//-----------------------------------------------
	// 쓰레드를 생성하여 list에 저장
	//-----------------------------------------------
	//HANDLE Create( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* threadID );
	HANDLE Create( LPVOID startAddress, LPVOID parameter, unsigned int *threadID );
};

//Mgr을 쉽게 부르기 위해 정의 해 둔다
#define GetThreadMgr SThreadMgr::GetInstance()

#endif


