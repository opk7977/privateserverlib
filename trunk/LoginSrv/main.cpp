#include "WinMgr.h"
#include "LoginMain.h"
#include "SThreadMgr.h"

#include "DataLeader.h"
#include "SLogger.h"

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
	if( !CheckValid( _T("LoginSrv") ) )
		return 0;

	//======================================
	// 로그 초기화
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "LoginSrv" );

	//======================================
	// 서버 데이터 로드
	//======================================
	if( !GetDocument.DataSetting( IpzCmdParam ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n데이터 로드 실패!\n\n") );
		return 0;
	}
	
	//======================================
	// 윈도우 생성
	//======================================
	HWND hWnd;
	WinMgr window;
	window.SetInstance( hInstance );
	if( !window.CreateHideWindows( hInstance, _T("Login"), _T("LoginSrv"), hWnd, nCmdShow ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n윈도우 생성 실패!\n\n") );
		return 0;
	}

	//======================================
	// login 메인 실행
	//======================================
	LoginMain lMain;
	lMain.Init();

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
	lMain.Release();

	//======================================
	// 로그 해제
	//======================================
	m_logger->Release();

	ReleaseMutex();

	return (int)Message.wParam;
}

