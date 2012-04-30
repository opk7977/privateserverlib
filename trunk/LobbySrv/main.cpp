#include "WinMgr.h"
#include "LobbyMain.h"
#include "SLogger.h"
#include "DataLeader.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Lobby"), _T("LobbySrv"), hWnd, 800, 600, nCmdShow ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("main::WinMain()\n������ ������ �����߽��ϴ�.\n\n") );
		return 0;
	}

	//�α��ʱ�ȭ
	GetLogger.Create( "LobbySrv" );

	//������ ���� ���� ����
	if( !GetData.DataSetting() )
		return -1;

	LobbyMain  myMain;
	myMain.Init();


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

