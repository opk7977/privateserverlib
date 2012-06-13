#include "WinMgr.h"
#include "SLogger.h"

#include "DataLeader.h"
#include "DBMain.h"

#include "DBSession.h"
#include "SPacket.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	//======================================
	// �α� �ʱ�ȭ
	//======================================
	SLogger* m_logger = &GetLogger;
	m_logger->Create( "DBSrv" );

	//======================================
	// ������ ����
	//======================================
	HWND hWnd;
	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Login"), _T("LoginSrv"), hWnd, 800, 600, nCmdShow ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("main\n������ ���� ����!\n\n") );
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
	// login ���� ����
	//======================================
	DBMain* lMain = new DBMain;
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