#ifndef __THREAD_MANAGER__
#define __THREAD_MANAGER__

#include "SServerStd.h"

//singleton
class SThreadMgr : public SSingleton <SThreadMgr>
{
private:
	friend class SSingleton<SThreadMgr>;

private:
	//������ list
	std::list<HANDLE> m_listThread;

private:
	SThreadMgr(void);
	virtual ~SThreadMgr(void);

public:
	//-----------------------------------------------
	// ��� �����尡 ����Ǵ� ���� Ȯ���ϰ� ��ٸ���.
	// ��� �����尡 ����Ǹ� return ��
	//-----------------------------------------------
	void IsEndAllThread();

	//-----------------------------------------------
	// ��� ������ ���� ����
	//-----------------------------------------------
	BOOL EndAllThread();

	//-----------------------------------------------
	// �����带 �����Ͽ� list�� ����
	//-----------------------------------------------
	//HANDLE Create( LPTHREAD_START_ROUTINE startAddress, LPVOID parameter, DWORD* threadID );
	HANDLE Create( LPVOID startAddress, LPVOID parameter, unsigned int *threadID );
};

//Mgr�� ���� �θ��� ���� ���� �� �д�
#define GetThreadMgr SThreadMgr::GetInstance()

#endif


