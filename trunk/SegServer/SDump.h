#ifndef __MINI_DUMP__
#define __MINI_DUMP__

#include "SServerObj.h"
#include <dbghelp.h>

typedef BOOL ( WINAPI *MINIDUMPWRITEDUMP ) 
(
	HANDLE									hProcess,
	DWORD									dwPid,
	HANDLE									hFile,
	MINIDUMP_TYPE							DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION	UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION	CallbackParam
);

//������ ����...
//�׳� ������ ������ ��� ������ ������ ������ �ʰ� �׳� ������ ��찡 �ִµ�
//�װ� ��� �α� ���� Ŭ����

class SDump : public SServerObj
{
private:
	//�����߻��� ���� ȣ��Ǵ� �Լ��� ������ �ش�
	//oldmemoryDc����
	LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter;

public:
	SDump(void);
	~SDump(void);
	
	static LONG WINAPI UnhandledExceptionFilter( struct _EXCEPTION_POINTERS *exceptionInfo );

	BOOL Begin();
	BOOL End();
};


#endif