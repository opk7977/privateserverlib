#include "WinMgr.h"
#include "WorldMain.h"
#include "SLogger.h"
#include "Document.h"

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
	// ���ؽ� �˻�
	//======================================
	if( !CheckValid( _T("WorldSrv") ) )
		return 0;

	//======================================
	// �α� �ʱ�ȭ
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "WorldSrv" );

	//======================================
	// ������ ����
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("WorldSrv"), _T("WorldSrv"), hWnd, 800, 600, nCmdShow ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("main::WinMain()\n������ ������ �����߽��ϴ�.\n\n") );
		return 0;
	}

	//======================================
	// WorldMain ����
	//======================================
	WorldMain myMain;
	myMain.Init();

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
	// �Ҵ� ����
	//======================================
	myMain.Release();

	//======================================
	// log����
	//======================================
	m_logger->Release();

	//======================================
	// ���ؽ� ����
	//======================================
	ReleaseMutex();

	return (int)Message.wParam;
}

