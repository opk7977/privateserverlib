#include "WinMgr.h"

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
		ResizeWindows( hWnd, 500, 100 );
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
