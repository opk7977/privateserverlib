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

	DBSession* tmpSession = new DBSession;
	TCHAR tmpID[10] = _T("areah");
	TCHAR tmpPW[15] = _T("areah");
	SPacket packet( 0 );
	packet << 1;
	int size = _tcslen(tmpID)*sizeof(TCHAR);
	packet << size;
	packet.PutData( tmpID, size );
	size = _tcslen(tmpPW)*sizeof(TCHAR);
	packet << size;
	packet.PutData( tmpPW, size );

	tmpSession->RecvLoginTryLogin( packet );

	SPacket packet2( 0 );
	packet2 << 1;
	int size2 = _tcslen(tmpID)*sizeof(TCHAR);
	packet2 << size2;
	packet2.PutData( tmpID, size2 );
	size2 = _tcslen(tmpPW)*sizeof(TCHAR);
	packet2 << size2;
	packet2.PutData( tmpPW, size2 );

	tmpSession->RecvLoginTryLogin( packet2 );


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