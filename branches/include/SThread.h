#ifndef __THREAD__
#define __THREAD__


#include "SServerobj.h"

class SThread : public SServerObj
{
private:
	//쓰레드의 id값
	DWORD	m_iThread;
	//쓰레드가 동작하는가에 대한 flag값
	BOOL	m_bIsStart;

	//쓰레드 핸들값
	HANDLE	m_handleThread;

public:
	SThread(void);
	virtual ~SThread(void);

	//virtual_ 상속받은 클래스가 작업하는 함수로
	//각각 작업에 맞게 재 정의하여 사용
	inline virtual BOOL Run() { return TRUE; }

	//쓰레드 자신의 ID를 return
	inline DWORD GetThreadId() { return m_iThread; }

	//쓰레드 핸들값 return
	inline HANDLE GetThreadHandle() { return m_handleThread; }

	//쓰레드 동작!!
	void BeginThread();

	//쓰레드 종료
	virtual void EndThread();

private:
	//--------------------------------------------------------
	//스테틱 선언으로 쓰레드가 실제 동작할 수 있도록 한 함수
	//Mgr에서 매개변수로 클래스 자신을 넘겨주어
	//각 클래스의 Run함수를 호출하도록 한다.
	//--------------------------------------------------------
	static DWORD __stdcall HandleRunner( LPVOID parameter );
};

#endif

