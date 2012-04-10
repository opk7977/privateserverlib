#include "main.h"
#include "WinMgr.h"

extern LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

CWinMgr::CWinMgr(void)
{
}

CWinMgr::~CWinMgr(void)
{
}

BOOL CWinMgr::CreateWindows( HINSTANCE hInst, int nCmdShow )
{
	wcex.cbSize					= sizeof(WNDCLASSEX);
	wcex.style					= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc			= (WNDPROC)WndProc;
	wcex.cbClsExtra				= 0;
	wcex.cbWndExtra				= 0;
	wcex.hInstance				= hInst;
	wcex.hIcon					= LoadIcon(hInst, IDI_APPLICATION);
	wcex.hIconSm				= LoadIcon(hInst, IDI_APPLICATION);
	wcex.hCursor				= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground			= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName			= NULL;
	wcex.lpszClassName			= szWindowClass;

	if( !RegisterClassEx( &wcex ) )
		return FALSE;

	return TRUE;
}