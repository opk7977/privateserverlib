#include "WinMgr.h"
#include "SNetwork.h"
#include "Scheduler.h"

void Init();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Test"), _T("TestGame"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	//Setting
	Init();

	MSG Message;

	while(1)
	{
		Sleep( 1 );

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

void Init()
{
	//�����ٷ�
	GetScheduler.Init();

	// Server Setting
	if( !GetNetwork.Init( TRUE ) )
	{
		MessageBox( NULL, _T("���� �ʱ�ȭ����"), _T("����"), MB_OK );
		return;
	}

	if( !GetNetwork.ConnectToSrv( "192.168.0.56", 7900 ) )
	{
		MessageBox( NULL, _T("���� �������"), _T("����"), MB_OK );
		return;
	}
}

