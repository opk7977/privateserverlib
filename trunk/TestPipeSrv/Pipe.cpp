#include "Pipe.h"
#include <conio.h>

#include <locale.h>

Pipe::Pipe(void)
{
	m_hPipe = 0;
	_tsetlocale(LC_ALL, _T("Korean"));
}

Pipe::~Pipe(void)
{
	Release();
}

BOOL Pipe::Init()
{
	m_hPipe = CreateNamedPipe( _T("\\\\.\\pipe\\TestPipe")
							 , PIPE_ACCESS_INBOUND
							 , PIPE_TYPE_BYTE
							 , PIPE_UNLIMITED_INSTANCES
							 , 512
							 , 0
							 , 0
							 , NULL );


	if( m_hPipe == 0 )
	{
		MessageBox( NULL, _T("pipe init()"), _T("error"), MB_OK );
		return FALSE;
	}

	BeginThread();

	return TRUE;
}

void Pipe::Release()
{
	CloseHandle( m_hPipe );
	DisconnectNamedPipe( m_hPipe );
}

BOOL Pipe::Run()
{
	BOOL retbool = ConnectNamedPipe( m_hPipe, NULL );

	_cprintf( "연결되었습니다.\n" );

	if( retbool == FALSE && (GetLastError() == ERROR_PIPE_CONNECTED ) )
		retbool = TRUE;

	char* buf[1024] = {0,};
	DWORD recvSize = 0;
	BOOL  retval;

	while(1)
	{
		retval = ReadFile( m_hPipe, buf, 1024, &recvSize, NULL );
		FlushFileBuffers( m_hPipe );

		if( retval == 0 )
			break;

		//다 받았으면 출력
		_cprintf( "문자 왔숑 size : %d\n", recvSize );
	}

	_cprintf( "연결이 끊겼습니다\n" );

	return TRUE;
}
