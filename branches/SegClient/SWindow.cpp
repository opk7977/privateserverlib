#include "SWindow.h"

static SWindow* gWinProc;

LRESULT WINAPI gMsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return gWinProc->WindowMegProc( hWnd, msg, wParam, lParam );
}


SWindow::SWindow()
{
	gWinProc = this;
}

SWindow::~SWindow(void)
{
}

BOOL SWindow::CreateWindows( HINSTANCE hInst, TCHAR* className, TCHAR* appName, HWND &hWnd, int width, int height, int nCmdShow )
{
	m_hInst = hInst;

	m_wcex.cbSize					= sizeof(WNDCLASSEX);
	m_wcex.style					= CS_HREDRAW | CS_VREDRAW;
	m_wcex.lpfnWndProc				= (WNDPROC)gMsgProc;
	m_wcex.cbClsExtra				= 0;
	m_wcex.cbWndExtra				= 0;
	m_wcex.hInstance				= hInst;
	m_wcex.hIcon					= LoadIcon(hInst, IDI_APPLICATION);
	m_wcex.hIconSm					= LoadIcon(hInst, IDI_APPLICATION);
	m_wcex.hCursor					= LoadCursor(NULL, IDC_ARROW);
	m_wcex.hbrBackground			= (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_wcex.lpszMenuName				= NULL;
	m_wcex.lpszClassName			= className;

	if( !RegisterClassEx( &m_wcex ) )
		return FALSE;

	hWnd = CreateWindowEx( WS_EX_APPWINDOW
						, className
						, appName
						, WS_OVERLAPPEDWINDOW
						, CW_USEDEFAULT
						, CW_USEDEFAULT
						, width
						, height
						, NULL
						, NULL
						, hInst
						, NULL );

	if( !hWnd )
		return FALSE;

	ShowWindow( hWnd, nCmdShow );

	return TRUE;
}

BOOL SWindow::ResizeWindows( HWND &hWnd, int width, int height )
{
	RECT	rc;
	SetRect(&rc, 0, 0 , width, height );

	if( AdjustWindowRectEx(&rc , GetWindowLong(hWnd, GWL_STYLE) , GetMenu( hWnd ) ? TRUE : FALSE , GetWindowLong(hWnd, GWL_EXSTYLE)) )
	{
		// 성공하면 윈도우를 화면 중앙에 배치하자.
		int x, y;
		x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left ) ) / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top ) ) / 2;

		MoveWindow( hWnd , x, y , rc.right - rc.left , rc.bottom - rc.top, TRUE);

		return TRUE;
	}

	return FALSE;
}

LRESULT SWindow::WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
