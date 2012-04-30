#include "main.h"
#include "STime.h"

#include "WinMgr.h"

#include "CharacterMgr.h"
#include "Network.h"

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
	CWinMgr			winMgr;
	//�������Ϳ� ��ϵ��� �������� program ����
	if( !winMgr.CreateWindows( hInstance, nCmdShow ) )
		return 0;

	HWND hWnd = CreateWindowEx( WS_EX_APPWINDOW , szWindowClass , szTitle , WS_OVERLAPPEDWINDOW , CW_USEDEFAULT , CW_USEDEFAULT , 400 , 300 , NULL , NULL , hInstance , NULL );
	//�� ����� ������ Ȯ��
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

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	switch( iMessage )
	{
	case WM_CREATE:
		// ������ ������
		{
			RECT	rc;
			SetRect(&rc, 0, 0 , g_WindowWidth, g_WindowHeight );
			if(AdjustWindowRectEx( &rc , GetWindowLong(hWnd, GWL_STYLE) , GetMenu( hWnd ) ? TRUE : FALSE , GetWindowLong(hWnd, GWL_EXSTYLE) ) )
			{
				// �����ϸ� �����츦 ȭ�� �߾ӿ� ��ġ����.
				int x, y;
				x = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left ) ) / 2;
				y = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top ) ) / 2;

				MoveWindow( hWnd , x, y , rc.right - rc.left , rc.bottom - rc.top, TRUE);
			}
		}
		
		//�ʱ�ȭ
		if( !Init() )
			PostQuitMessage(0);

		break;
	case WM_DESTROY:
		//ĳ���� �����ְ�
		Release();
		PostQuitMessage(0);
		return 0;
	}

	return (DefWindowProc( hWnd, iMessage, wParam, lParam ) );
}

BOOL Init()
{
	//�������� �ʱ�ȭ
	if( !g_net.Init() )
		return FALSE;

	//�������� ����
	if( !g_net.ConnectSrv( "192.168.0.56", 8900 ) )
	//if( !g_net.ConnectSrv( "127.0.0.1", 8900 ) )
		return FALSE;

	//�ð� �ʱ�ȭ �ϰ�
	g_timer.Init();

	return TRUE;
}

void Release()
{
	//ĳ���� �����ְ�
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
	// �� ���۸� ȭ������ ��������.
	BitBlt(g_hDc, 0, 0, g_WindowWidth, g_WindowHeight, g_memoryDc, 0, 0, SRCCOPY );

	// DC�� ��������� ������ �������� �ǵ�����.
	SelectObject(g_hDc, g_oldBit);
	DeleteObject(g_memoryBit);
	DeleteDC(g_memoryDc);

	ReleaseDC(hWnd, g_hDc);
}

void FrameMove( HWND hWnd )
{
	//���� ������ �Ǽ� ĳ���Ͱ� ������ٸ� �����Ѵ�
	if( !g_isStart )
		return;

	g_timer.ProcessTime();

	g_frame += g_timer.GetElapsedTime();

	float frame = 0.1f;

	//1�ʸ��� �����ϱ�
	if( g_frame >= frame )
	{
		g_frame -= frame;

		//ĳ���͵��� ������ �ش�.
		GetCharMgr.MoveCharacter();
	}
}

void Render( HWND hWnd )
{
	//���� ������ �Ǽ� ĳ���Ͱ� ������ٸ� �����Ѵ�
	if( !g_isStart )
		return;

	//�켱 �����
	RECT rc;
	rc.top = rc.left = 0;
	rc.bottom = g_WindowHeight;
	rc.right= g_WindowWidth;

	Rectangle( g_memoryDc, rc.left, rc.top, rc.right, rc.bottom );

	//ĳ���͵鵵 �׷�����
	GetCharMgr.RenderAll( g_memoryDc );
}



