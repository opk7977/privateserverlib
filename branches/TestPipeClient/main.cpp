#include "WinMgr.h"

void Init();
void Relase();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Try"), _T("TestPipeCli"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	//Setting
	Init();


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

	Relase();
	return (int)Message.wParam;
}

void Init()
{
	AllocConsole();
}

void Relase()
{
	FreeConsole();
}
