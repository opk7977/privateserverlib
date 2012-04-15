#include "WinMgr.h"
#include "LoginMain.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Login"), _T("LoginSrv"), hWnd, 800, 600, nCmdShow ) )
		return 0;


	LoginMain lMain;
	lMain.Init();


	MSG Message;

	while(1)
	{
		if( PeekMessage( &Message, NULL, 0, 0, PM_REMOVE ) )
		{
			if( Message.message == WM_QUIT )
				break;

			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
		else
		{
		}
	}
	return (int)Message.wParam;
}

