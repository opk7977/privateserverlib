#include "WinMgr.h"
#include "LoginMain.h"

#include "DataLeader.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// 로그 초기화
	//======================================
#ifdef _DEBUG
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "LoginSrv" );
#endif
	
	//======================================
	// 윈도우 생성
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Login"), _T("LoginSrv"), hWnd, 800, 600, nCmdShow ) )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n윈도우 생성 실패!\n\n") );
#endif
		return 0;
	}

	//======================================
	// 서버 데이터 로드
	//======================================
	if( !GetDocument.DataSetting() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n데이터 로드 실패!\n\n") );
#endif
		return 0;
	}

	//======================================
	// login 메인 실행
	//======================================
	LoginMain* lMain = new LoginMain;
	lMain->Init();

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
	delete lMain;

	return (int)Message.wParam;
}

