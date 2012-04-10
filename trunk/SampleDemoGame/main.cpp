#include "main.h"
#include "STime.h"

#include "WinMgr.h"

#include "CharacterMgr.h"
#include "Network.h"
#include "SDump.h"

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

//////////////////////////////////////////////////////////////////////////
BOOL Init();
void Release();
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void BeginScene( HWND hWnd );
void EndScene( HWND hWnd );

void FrameMove( HWND hWnd );
void Render( HWND hWnd );
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
HDC			g_hDc;
HDC			g_memoryDc;
HBITMAP		g_memoryBit;
HBITMAP		g_oldBit;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
Network					g_net;
STime					g_timer;
float					g_frame = 0.f;
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
			BeginScene( hWnd );

			FrameMove( hWnd );
			Render( hWnd );

			EndScene( hWnd );
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
		// 윈도우 재정렬
		{
			RECT	rc;
			SetRect(&rc, 0, 0 , g_WindowWidth, g_WindowHeight );
			if(AdjustWindowRectEx( &rc , GetWindowLong(hWnd, GWL_STYLE) , GetMenu( hWnd ) ? TRUE : FALSE , GetWindowLong(hWnd, GWL_EXSTYLE) ) )
			{
				// 성공하면 윈도우를 화면 중앙에 배치하자.
				int x, y;
				x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left ) ) / 2;
				y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top ) ) / 2;

				MoveWindow( hWnd , x, y , rc.right - rc.left , rc.bottom - rc.top, TRUE);
			}
		}
		
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
  	if( !g_net.Init(10) )
  		return FALSE;
 
 	//서버와의 연결
 	if( !g_net.SettingSrv( 9000 ) )
 		return FALSE;

	//시간 초기화 하고
	g_timer.Init();

	return TRUE;
}

void Release()
{
	//캐릭터 지워주고
}

void BeginScene( HWND hWnd )
{
	g_hDc			= GetDC(hWnd);
	g_memoryDc		= CreateCompatibleDC( g_hDc );
	g_memoryBit		= CreateCompatibleBitmap( g_hDc, g_WindowWidth, g_WindowHeight );
	g_oldBit		= (HBITMAP)SelectObject( g_memoryDc, g_memoryBit);
}

void EndScene( HWND hWnd )
{
	// 빽 버퍼를 화면으로 복사하자.
	BitBlt(g_hDc, 0, 0, g_WindowWidth, g_WindowHeight, g_memoryDc, 0, 0, SRCCOPY );

	// DC를 사용했으면 원래의 설정으로 되돌리자.
	SelectObject(g_hDc, g_oldBit);
	DeleteObject(g_memoryBit);
	DeleteDC(g_memoryDc);

	ReleaseDC(hWnd, g_hDc);
}

void FrameMove( HWND hWnd )
{
	g_timer.ProcessTime();

	g_frame += g_timer.GetElapsedTime();

	float frame = 0.11f;

	//1초마다 갱신하기
	if( g_frame >= frame )
	{
		g_frame -= frame;

		//캐릭터들을 움직여 준다.
		GetCharMgr.MoveCharacter();
	}
}

void Render( HWND hWnd )
{
	//우선 흰바탕
	RECT rc;
	rc.top = rc.left = 0;
	rc.bottom = g_WindowHeight;
	rc.right= g_WindowWidth;

	Rectangle( g_memoryDc, rc.left, rc.top, rc.right, rc.bottom );

	//캐릭터들도 그려주자
	GetCharMgr.RenderAll( &g_memoryDc );
}



