#include "WinMgr.h"
#include "GameSrvMain.h"
#include "SLogger.h"
#include "DataLeader.h"

HANDLE m_hMutex;

void ReleaseMutex()
{
	::ReleaseMutex( m_hMutex );
	::CloseHandle( m_hMutex );
	m_hMutex = NULL;
}

BOOL CheckValid( TCHAR* mutexText )
{
	m_hMutex = ::CreateMutex( NULL, TRUE, mutexText );
	DWORD dwError = ::GetLastError();

	if( m_hMutex == NULL )
		return FALSE;

	if( dwError == ERROR_ALREADY_EXISTS )
	{
		ReleaseMutex();
		return FALSE;
	}

	return TRUE;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// 뮤텍스 검사
	//======================================
	if( !CheckValid( _T("GameSrv") ) )
		return 0;

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
	//if( !window.CreateWindows( hInstance, _T("GameSrv"), _T("GameSrv"), hWnd, 800, 600, nCmdShow ) )
	if( !window.CreateHideWindows( hInstance, _T("GameSrv"), _T("GameSrv"), hWnd, nCmdShow ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("main::WinMain()\n윈도우 생성에 실패했습니다.\n\n") );
		return 0;
	}

	window.SetMyWindowText( hWnd, _T("GameSrv_준비중") );

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
	GameSrvMain gMain;
	gMain.Init();

	window.SetMyWindowText( hWnd, _T("GameSrv_실행중") );


	MSG Message;

	while( GetMessage( &Message, NULL, 0, 0 ) )
	{
		if ( !TranslateAccelerator( Message.hwnd, NULL, &Message ) )
		{
			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
	}

	//======================================
	// 할당 해제
	//======================================
	gMain.Release();

	//======================================
	// log해제
	//======================================
	m_logger->Release();

	//======================================
	// 뮤텍스 해제
	//======================================
	ReleaseMutex();

	return (int)Message.wParam;
}

