#include "SThread.h"
#include "SThreadMgr.h"

SThread::SThread(void) : m_bIsStart(FALSE)
					   , m_iThread(0)
{
}

SThread::~SThread(void)
{
}

void SThread::BeginThread()
{
	//이미 동작하고 있는 쓰레드라면 다시 시작을 해 줄 필요없음 (방어코드)
	if( m_bIsStart )
		return;

	//Mgr에 자신을 매개 변수로 함수 호출
	m_handleThread = GetThreadMgr.Create( HandleRunner, this, &m_iThread );

	m_bIsStart = TRUE;
}

DWORD __stdcall SThread::HandleRunner( LPVOID parameter )
{
	//Mgr에서 this포인터로 받은 클래스를 시행 시키면
	//해당 클래스의 HandleRunner함수가 발동하고
	//현재 함수에서 해당 클래스의 Run함수를 호출한다
	SThread* tmpThread = (SThread*)parameter;
	tmpThread->Run();
	//두줄을 줄이면
	//((SThread*)parameter)->Run();
	//과 같다.

	return 0;
}

void SThread::EndThread()
{

}

