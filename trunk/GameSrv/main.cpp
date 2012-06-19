#include "WinMgr.h"
#include "GameSrvMain.h"
#include "SLogger.h"
#include "DataLeader.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// �α� �ʱ�ȭ
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "GameSrv" );

	//======================================
	// ������ ����
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("GameSrv"), _T("GameSrv"), hWnd, 800, 600, nCmdShow ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("main::WinMain()\n������ ������ �����߽��ϴ�.\n\n") );
		return 0;
	}

	//======================================
	// ���� ������ �ε�
	//======================================
	if( !GetDocument.DataSetting() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("main\n������ �ε� ����!\n\n") );
		return 0;
	}

	//======================================
	// Game ���� ����
	//======================================
	GameSrvMain* myMain = new GameSrvMain;
	myMain->Init();


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

	//======================================
	// �Ҵ� ����
	//======================================
	delete myMain;

	return (int)Message.wParam;
}

