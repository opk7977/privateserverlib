#include "SDump.h"

//에러 발생 시 넘어오는 콜백
LONG WINAPI SDump::UnhandledExceptionFilter( struct _EXCEPTION_POINTERS *exceptionInfo )
{
	HMODULE		DllHandle = NULL;
	//Windows 2000이전에는 따로 DBGHELP를 배포해서 설정해야 함
	DllHandle = LoadLibrary( _T("DBGHELP.DLL") );

	if( DllHandle )
	{
		//dump를 받아 파일로 만든다
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP) GetProcAddress( DllHandle, "MiniDumpWriteDump" );

		if( Dump )
		{
			TCHAR		DumpPath[1024] = {0, };
			SYSTEMTIME	SysTime;

			//현재 시간 가져오기
			GetLocalTime( &SysTime );

			//현재 시간 기준으로 로그 파일을 만듭니다.
			_sntprintf_s( DumpPath, 23, 1024, _T("%d-%d-%d %d_%d_%d.dmp")
												, SysTime.wYear
												, SysTime.wMonth
												, SysTime.wDay
												, SysTime.wHour
												, SysTime.wMinute
												, SysTime.wSecond );

			HANDLE FileHandle = CreateFile( DumpPath
											, GENERIC_WRITE
											, FILE_SHARE_WRITE
											, NULL
											, CREATE_ALWAYS
											, FILE_ATTRIBUTE_NORMAL
											, NULL );
			//파일 만들기에 성공?
			if( FileHandle != INVALID_HANDLE_VALUE )
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				//MiniDump예외 정보 저장 구조체
				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers	= NULL;

				//현재 프로세스에 대한 덤프를 실행합니다.
				BOOL Success = Dump( GetCurrentProcess()
									, GetCurrentProcessId()
									, FileHandle
									, MiniDumpNormal
									, &MiniDumpExceptionInfo
									, NULL
									, NULL );

				if( Success )
				{
					//성공했으면 닫는다
					CloseHandle( FileHandle );

					return EXCEPTION_EXECUTE_HANDLER;
				}
			}

			CloseHandle( FileHandle );
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

SDump::SDump(void)
{
}

SDump::~SDump(void)
{
}

BOOL SDump::Begin()
{
	//잘못된 연산 창이 나타나지 않게 합니다.
	SetErrorMode( SEM_FAILCRITICALERRORS );

	//에러 발생시 진행될 콜백을 정의
	PreviousExceptionFilter = SetUnhandledExceptionFilter( UnhandledExceptionFilter );

	return TRUE;
}

BOOL SDump::End()
{
	SetUnhandledExceptionFilter( PreviousExceptionFilter );

	return TRUE;
}
