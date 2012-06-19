#include "WinMgr.h"
#include "GameSrvMain.h"
#include "SLogger.h"
#include "DataLeader.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// 로그 초기화
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "GameSrv" );

	//======================================
	// 윈도우 생성
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("GameSrv"), _T("GameSrv"), hWnd, 800, 600, nCmdShow ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("main::WinMain()\n윈도우 생성에 실패했습니다.\n\n") );
		return 0;
	}

	//======================================
	// 서버 데이터 로드
	//======================================
	if( !GetDocument.DataSetting() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("main\n데이터 로드 실패!\n\n") );
		return 0;
	}

	//======================================
	// Game 메인 실행
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
	// 할당 해제
	//======================================
	delete myMain;

	return (int)Message.wParam;
}

