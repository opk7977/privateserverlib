#include "WinMgr.h"
#ifdef _DEBUG
	#include "SLogger.h"
#endif

#include "DataLeader.h"
#include "DBMain.h"

#include "DBSession.h"
#include "SPacket.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
#ifdef _DEBUG
	//======================================
	// 로그 초기화
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "DBSrv" );
#endif

	//======================================
	// 윈도우 생성
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("DB"), _T("DBSrv"), hWnd, 800, 600, nCmdShow ) )
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
	DBMain* lMain = new DBMain;
	if( !lMain->Init() )
		return 0;

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