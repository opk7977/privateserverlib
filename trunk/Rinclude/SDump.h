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

//터지는 에러...
//그냥 터지는 에러는 어디서 오류가 난건지 보이지 않고 그냥 꺼지는 경우가 있는데
//그걸 담아 두기 위한 클래스

class SDump : public SServerObj
{
private:
	//에러발생시 원래 호출되는 함수를 저장해 준다
	//oldmemoryDc개념
	LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter;

public:
	SDump(void);
	~SDump(void);
	
	static LONG WINAPI UnhandledExceptionFilter( struct _EXCEPTION_POINTERS *exceptionInfo );

	BOOL Begin();
	BOOL End();
};


#endif