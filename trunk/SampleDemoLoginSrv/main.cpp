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
 	//�������Ϳ� ��ϵ��� �������� program ����
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
 	if( !GetNetwork.Init(10) )
 		return FALSE;
 
 	//�������� ����
 	if( !GetNetwork.SettingSrv( 8900 ) )
 		return FALSE;

	//�ð� �ʱ�ȭ �ϰ�
	//g_timer.Init();

	return TRUE;
}

void Release()
{
	//ĳ���� �����ְ�
}

void FrameMove( HWND hWnd )
{
}

void Render( HWND hWnd )
{
}



