#ifndef __THREAD__
#define __THREAD__


#include "SServerobj.h"

class SThread : public SServerObj
{
private:
	//�������� id��
	DWORD	m_iThread;
	//�����尡 �����ϴ°��� ���� flag��
	BOOL	m_bIsStart;

	//������ �ڵ鰪
	HANDLE	m_handleThread;

public:
	SThread(void);
	virtual ~SThread(void);

	//virtual_ ��ӹ��� Ŭ������ �۾��ϴ� �Լ���
	//���� �۾��� �°� �� �����Ͽ� ���
	inline virtual BOOL Run() { return TRUE; }

	//������ �ڽ��� ID�� return
	inline DWORD GetThreadId() { return m_iThread; }

	//������ �ڵ鰪 return
	inline HANDLE GetThreadHandle() { return m_handleThread; }

	//������ ����!!
	void BeginThread();

	//������ ����
	virtual void EndThread();

private:
	//--------------------------------------------------------
	//����ƽ �������� �����尡 ���� ������ �� �ֵ��� �� �Լ�
	//Mgr���� �Ű������� Ŭ���� �ڽ��� �Ѱ��־�
	//�� Ŭ������ Run�Լ��� ȣ���ϵ��� �Ѵ�.
	//--------------------------------------------------------
	static DWORD __stdcall HandleRunner( LPVOID parameter );
};

#endif

