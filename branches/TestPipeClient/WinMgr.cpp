#include "WinMgr.h"

TCHAR* str = _T("헤이헤이");
WinMgr::WinMgr(void)
{
}

WinMgr::~WinMgr(void)
{
}

LRESULT WinMgr::WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CREATE:
		{
			//초기화
			m_hPipe = CreateFile( _T("\\\\.\\pipe\\TestPipe"), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
			if( m_hPipe == INVALID_HANDLE_VALUE )
				PostQuitMessage(0);

			Sleep(10);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			//전송!
			DWORD size;
			WriteFile( m_hPipe, (LPCVOID)str, wcslen( str ), &size, NULL );
			FlushFileBuffers( m_hPipe );
		}
		break;
	case WM_DESTROY:
		Release();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void WinMgr::Release()
{
	DisconnectNamedPipe( m_hPipe );
	CloseHandle( m_hPipe );
}
