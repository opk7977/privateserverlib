#include "main.h"

#include "WinMgr.h"
#include "Network.h"
#include "SDump.h"

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

//////////////////////////////////////////////////////////////////////////
BOOL Init();
void Release();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
BOOL			g_isStart = FALSE;
//////////////////////////////////////////////////////////////////////////

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//////////////////////////////////////////////////////////////////////////
	SDump			tmpDump;
	//////////////////////////////////////////////////////////////////////////

 	CWinMgr			winMgr;
 	//레지스터에 등록되지 못했으면 program 종료
 	if( !winMgr.CreateWindows( hInstance, nCmdShow ) )
 		return 0;
 
	HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW
							  , szWindowClass
							  , szTitle
							  , WS_OVERLAPPEDWINDOW
							  , CW_USEDEFAULT
							  , CW_USEDEFAULT
							  , 400
							  , 300
							  , NULL
							  , NULL
							  , hInstance
							  , NULL );

	//잘 만들어 졌는지 확인
	if( !hWnd )
		return 0;
 	
 	ShowWindow( hWnd, nCmdShow );

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

	//////////////////////////////////////////////////////////////////////////
	tmpDump.End();
	//////////////////////////////////////////////////////////////////////////
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	switch( iMessage )
	{
	case WM_CREATE:
		//초기화
		if( !Init() )
			PostQuitMessage(0);

		break;
	case WM_DESTROY:
		//캐릭터 지워주고
		Release();
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc( hWnd, iMessage, wParam, lParam ) );
}

BOOL Init()
{
	//연결전에 초기화
 	if( !GetNetwork.Init(10) )
 		return FALSE;
 
 	//서버와의 연결
 	if( !GetNetwork.SettingSrv( 8900 ) )
 		return FALSE;

	//시간 초기화 하고
	//g_timer.Init();

	return TRUE;
}

void Release()
{
	//캐릭터 지워주고
}

void FrameMove( HWND hWnd )
{
}

void Render( HWND hWnd )
{
}



