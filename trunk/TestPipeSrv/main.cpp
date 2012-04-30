#include "SWindow.h"
#include "Pipe.h"

#include "SLogger.h"


BOOL Init();
void Relase();

Pipe g_pipe;


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	SWindow window;
	if( !window.CreateWindows( hInstance, _T("Try"), _T("TestPipeSrv"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	//로그초기화
	GetLogger.Create( "TestPipeSrv" );

	//Setting
	if( !Init() )
		return 0;


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

BOOL Init()
{
	if( !g_pipe.Init() )
		return FALSE;

	AllocConsole();

	return TRUE;
}

void Relase()
{
	FreeConsole();
}
