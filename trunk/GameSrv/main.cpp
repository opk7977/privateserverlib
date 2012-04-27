#include "WinMgr.h"
#include "GameSrvMain.h"
#include "SLogger.h"
#include "DataLeader.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("GameSrv"), _T("GameSrv"), hWnd, 800, 600, nCmdShow ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("main::WinMain()\n윈도우 생성에 실패했습니다.\n\n") );
		return 0;
	}

	//데이터 부터 셋팅 하자
	if( !GetData.DataSetting() )
		return -1;

	GameSrvMain  myMain;
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

