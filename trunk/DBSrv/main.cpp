#include "WinMgr.h"
#include "SLogger.h"

#include "DataLeader.h"
#include "DBMain.h"

#include "DBSession.h"
#include "SPacket.h"

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
	if( !CheckValid( _T("DBSrv") ) )
		return 0;

	//======================================
	// �α� �ʱ�ȭ2
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "DBSrv" );

	//======================================
	// ������ ����
	//======================================
	HWND hWnd;
	WinMgr window;
	//if( !window.CreateWindows( hInstance, _T("DB"), _T("DBSrv"), hWnd, 800, 600, nCmdShow ) )
	if( !window.CreateHideWindows( hInstance, _T("DB"), _T("DBSrv"), hWnd, nCmdShow ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n������ ���� ����!\n\n") );
		return 0;
	}

	window.SetMyWindowText( hWnd, _T("DBSrv_�غ���") );

	//======================================
	// ���� ������ �ε�
	//======================================
	if( !GetDocument.DataSetting() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n������ �ε� ����!\n\n") );
		return 0;
	}

	//======================================
	// login ���� ����
	//======================================
	DBMain dMain;
	if( !dMain.Init() )
		return 0;

	window.SetMyWindowText( hWnd, _T("DBSrv_������") );

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
	dMain.Release();

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