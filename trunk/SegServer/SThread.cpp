#include "SThread.h"
#include "SThreadMgr.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

SThread::SThread(void) : m_bIsStart(FALSE)
					   , m_iThread(0)
{
}

SThread::~SThread(void)
{
	//GetLogger.PutLog( 0, _T("[Thread] %d�� �����尡 ���� �˴ϴ�\n"), m_iThread );
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Thread::~SThread()] %d�� �����尡 ���� �˴ϴ�\n"), m_iThread );
#endif
}

void SThread::BeginThread()
{
	//�̹� �����ϰ� �ִ� �������� �ٽ� ������ �� �� �ʿ���� (����ڵ�)
	if( m_bIsStart )
		return;

	//Mgr�� �ڽ��� �Ű� ������ �Լ� ȣ��
	m_handleThread = GetThreadMgr.Create( HandleRunner, this, &m_iThread );

	m_bIsStart = TRUE;
}

unsigned int __stdcall SThread::HandleRunner( LPVOID parameter )
{
	//Mgr���� this�����ͷ� ���� Ŭ������ ���� ��Ű��
	//�ش� Ŭ������ HandleRunner�Լ��� �ߵ��ϰ�
	//���� �Լ����� �ش� Ŭ������ Run�Լ��� ȣ���Ѵ�
	SThread* tmpThread = (SThread*)parameter;
	tmpThread->Run();
	//������ ���̸�
	//((SThread*)parameter)->Run();
	//�� ����.

	return 0;
}

void SThread::EndThread()
{

}

