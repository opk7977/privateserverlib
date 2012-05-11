#include "WinMgr.h"
#include "LoginMain.h"

#include "DataLeader.h"
#include "SLogger.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Login"), _T("LoginSrv"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	if( !window.InitConsolHandler() )
		return 0;

	//�α��ʱ�ȭ
	GetLogger.Create( "LoginSrv" );

	//������ ���� ���� ����
	if( !GetDocument.DataSetting() )
		return -1;


	LoginMain lMain;
	lMain.Init();


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

	lMain.Relase();
	return (int)Message.wParam;
}

