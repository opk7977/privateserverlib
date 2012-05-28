#include "NL_Window.h"

//===============================================================
//  NAME	  : NWindow
//  COMMENTS  : 윈도우 클래스 구현
//===============================================================
// MsgProc Bind
NWindow*	__window_pNWindow = NULL;
LRESULT CALLBACK __window_MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return __window_pNWindow->WindowMsgProc( hWnd, msg, wParam, lParam );
}

//-------------------------------
//	Constructor & Destructor
//-------------------------------
NWindow::NWindow(void):
	m_hInst(0), m_hWnd(0), m_bActive(false), m_clntWidth(0), m_clntHeight(0)
{
	__window_pNWindow = this;
}

NWindow::~NWindow(void) {}

//-------------------------------
//	Function
//-------------------------------
HWND NWindow::CreateWin(HINSTANCE		hInst, 
						const TCHAR*	titleName, 
						DWORD			width, 
						DWORD			height, 
						const TCHAR*	clsName)
{
	m_hInst			= hInst;
	m_clntWidth		= width;
	m_clntHeight	= height;

	RECT realRt = {	0, 0, m_clntWidth, m_clntHeight };
	::AdjustWindowRectEx( &realRt, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW );

	WNDCLASSEX wcex = { 0 };
	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)__window_MsgProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszClassName	= clsName;
	wcex.lpszMenuName	= NULL;
	
	if( !RegisterClassEx( &wcex ) )	return NULL;

	m_hWnd = CreateWindowEx(	WS_EX_APPWINDOW,
								clsName,
								titleName,
								WS_OVERLAPPEDWINDOW,
								0, 0,
								realRt.right - realRt.left,
								realRt.bottom - realRt.top,
								NULL, NULL,	m_hInst, NULL	);

	if( !m_hWnd )	return NULL;

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return m_hWnd;
}

LRESULT NWindow::WindowMsgProc(HWND	hWnd, UINT	msg, WPARAM	wParam, LPARAM	lParam)
{
	static LRESULT ret;
	
	ret = EngineMsgProc( hWnd, msg, wParam, lParam );
	if( ret >= 0 )
		return ret;

	switch( msg )
	{
	case WM_ACTIVATEAPP:
		{
			SetActive((BOOL)wParam);
			return 0;
		}
		break;

	case WM_DESTROY:
		{
			SetActive( false );
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int NWindow::Run(void)
{
	MSG	msg = { 0 };

	while( msg.message != WM_QUIT )
	{
		if( ::GetMessage( &msg, NULL, 0, 0 ) )
		{
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}
	}

	return (int)msg.wParam;
}

void NWindow::InitWin( HWND hWnd )
{
	m_hWnd = hWnd;
}
