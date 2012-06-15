#include "WinMgr.h"
#include "LoginMain.h"

#include "DataLeader.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// �α� �ʱ�ȭ
	//======================================
#ifdef _DEBUG
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "LoginSrv" );
#endif
	
	//======================================
	// ������ ����
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Login"), _T("LoginSrv"), hWnd, 800, 600, nCmdShow ) )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n������ ���� ����!\n\n") );
#endif
		return 0;
	}

	//======================================
	// ���� ������ �ε�
	//======================================
	if( !GetDocument.DataSetting() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("main\n������ �ε� ����!\n\n") );
#endif
		return 0;
	}

	//======================================
	// login ���� ����
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
	// �Ҵ� ����
	//======================================
	delete lMain;

	return (int)Message.wParam;
}

