#include "WinMgr.h"
// #include "Network.h"
// #include "GProcMgr.h"


void Init();
void Relase();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Lobby"), _T("TestGameSrv"), hWnd, 800, 600, nCmdShow ) )
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
	// Server Setting
// 	if( !GetNetwork.Init( 150 ) )
// 		return;
// 
// 	if( !GetNetwork.SettingSrv( 8800 ) )
// 		return;
// 
// 	// GameProcess Setting
// 	GetGPMgr.Init( 3 );
// }

void Relase()
{

}
