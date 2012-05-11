#include "WinMgr.h"

BOOL WINAPI ConsoleHandler( DWORD CEvent )
{
	switch( CEvent )
	{
	case CTRL_C_EVENT:
		MessageBox( NULL, _T("CTRL_C_EVENT"), _T("info"), MB_OK );
		break;
	case CTRL_BREAK_EVENT:
		MessageBox( NULL, _T("CTRL_BREAK_EVENT"), _T("info"), MB_OK );
		break;
	case CTRL_CLOSE_EVENT:
		MessageBox( NULL, _T("CTRL_CLOSE_EVENT"), _T("info"), MB_OK );
		break;
	case CTRL_LOGOFF_EVENT:
		MessageBox( NULL, _T("CTRL_LOGOFF_EVENT"), _T("info"), MB_OK );
		break;
	case CTRL_SHUTDOWN_EVENT:
		PostQuitMessage(0);
		return TRUE;
	}

	return TRUE;
}



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
		ResizeWindows( hWnd, 200, 100 );
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

BOOL WinMgr::InitConsolHandler()
{
	return SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleHandler, TRUE );
}
