#include "WinMgr.h"
#include "MyData.h"

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

MyData* myData = &GetDocument;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// ������ ����
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Lobby"), _T("LobbySrv"), hWnd, 800, 600, nCmdShow ) )
	{
		return 0;
	}

	//�� �ʱ�ȭ
	myData->MapInit();

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

			Render( hWnd );

			EndScene( hWnd );
		}
	}

	return (int)Message.wParam;
}

void BeginScene( HWND hWnd )
{
	g_hDc			= GetDC(hWnd);
	g_memoryDc		= CreateCompatibleDC( g_hDc );
	g_memoryBit		= CreateCompatibleBitmap( g_hDc, 600, 600 );
	g_oldBit		= (HBITMAP)SelectObject( g_memoryDc, g_memoryBit);
}

void EndScene( HWND hWnd )
{
	// �� ���۸� ȭ������ ��������.
	BitBlt(g_hDc, 0, 0, 600, 600, g_memoryDc, 0, 0, SRCCOPY );

	// DC�� ��������� ������ �������� �ǵ�����.
	SelectObject(g_hDc, g_oldBit);
	DeleteObject(g_memoryBit);
	DeleteDC(g_memoryDc);

	ReleaseDC(hWnd, g_hDc);
}

void Render( HWND hWnd )
{
	Rectangle( g_memoryDc, 0, 0, 600, 600 );

	myData->PrintMap( g_memoryDc );
}

