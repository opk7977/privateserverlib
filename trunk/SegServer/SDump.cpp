#include "SDump.h"

//���� �߻� �� �Ѿ���� �ݹ�
LONG WINAPI SDump::UnhandledExceptionFilter( struct _EXCEPTION_POINTERS *exceptionInfo )
{
	HMODULE		DllHandle = NULL;
	//Windows 2000�������� ���� DBGHELP�� �����ؼ� �����ؾ� ��
	DllHandle = LoadLibrary( _T("DBGHELP.DLL") );

	if( DllHandle )
	{
		//dump�� �޾� ���Ϸ� �����
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP) GetProcAddress( DllHandle, "MiniDumpWriteDump" );

		if( Dump )
		{
			TCHAR		DumpPath[1024] = {0, };
			SYSTEMTIME	SysTime;

			//���� �ð� ��������
			GetLocalTime( &SysTime );

			//���� �ð� �������� �α� ������ ����ϴ�.
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
			//���� ����⿡ ����?
			if( FileHandle != INVALID_HANDLE_VALUE )
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				//MiniDump���� ���� ���� ����ü
				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers	= NULL;

				//���� ���μ����� ���� ������ �����մϴ�.
				BOOL Success = Dump( GetCurrentProcess()
									, GetCurrentProcessId()
									, FileHandle
									, MiniDumpNormal
									, &MiniDumpExceptionInfo
									, NULL
									, NULL );

				if( Success )
				{
					//���������� �ݴ´�
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
	//�߸��� ���� â�� ��Ÿ���� �ʰ� �մϴ�.
	SetErrorMode( SEM_FAILCRITICALERRORS );

	//���� �߻��� ����� �ݹ��� ����
	PreviousExceptionFilter = SetUnhandledExceptionFilter( UnhandledExceptionFilter );

	return TRUE;
}

BOOL SDump::End()
{
	SetUnhandledExceptionFilter( PreviousExceptionFilter );

	return TRUE;
}
