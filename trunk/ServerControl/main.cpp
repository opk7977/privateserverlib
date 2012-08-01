#include "resource.h"
#include "SrvCtrl.h"

BOOL CALLBACK DlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

SrvCtrl*	g_srvControl = &GetSrvCtrl;
HANDLE		m_hMutex;
HINSTANCE	hInst;


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
	//icon을 위해..
	hInst = hInstance;

	//======================================
	// 뮤텍스 검사
	//======================================
	if( !CheckValid( _T("ServerControl") ) )
	{
		MessageBox( NULL, _T("이미 프로그램이 켜져 있습니다."), _T("다시 확인하세요"), MB_OK );
		return 0;
	}

 	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DLG_SRVCtrl ), HWND_DESKTOP, DlgProc );

	//======================================
	// 뮤텍스 해제
	//======================================
	ReleaseMutex();
	return 0;
}

BOOL CALLBACK DlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		{
			HICON sIcon, bIcon;
			sIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_16 ) );
			bIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON_48 ) );

			//큰 아이콘
			SendMessage( hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)bIcon );
			//작은 아이콘
			SendMessage( hWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)sIcon );

			SetDlgItemTextA( hWnd, IDC_EDIT_SRVNAME, "Unknown_서버" );
			EnableWindow( GetDlgItem( hWnd, ID_SRV_END ), FALSE );
			
			//======================================
			// 필요한 서버 파일들이 모두 있는지 체크
			//======================================
			if( !g_srvControl->CheckFindNeedAllFile() )
				EndDialog( hWnd, 0 );
		}

		break;
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case ID_SRV_START:
			{
				char serverName[32]={0,};
				GetDlgItemTextA( hWnd, IDC_EDIT_SRVNAME, serverName, 32 );

				//======================================
				// 서버 실행
				//======================================
				if( !g_srvControl->StartServer( serverName ) )
				{
					//서버 실행 실패!
					MessageBox( NULL, _T("서버 실행 실패!"), _T("error"), MB_OK );
					EndDialog( hWnd, 0 );
				}

				//======================================
				// 서버 이름은 이제 설정하지 않아도 됨
				//======================================
				EnableWindow( GetDlgItem( hWnd, IDC_EDIT_SRVNAME ), FALSE );

				//======================================
				// 서버를 켜는 버튼은 비활성화
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_START ), FALSE );

				//======================================
				// 서버 종료 버튼은 활성화
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_END ), TRUE );

				//======================================
				// 상태 message를 바꿔 준다
				//======================================
				char msg[64]={0,};
				sprintf_s( msg, 64, "실행중(%s)", serverName );
				SetDlgItemTextA( hWnd, IDC_STATE_MESSAGE, msg );
			}
			break;
		case ID_SRV_END:
			{
				//======================================
				// 서버 종료
				//======================================
				g_srvControl->EndServer();

				SetDlgItemTextA( hWnd, IDC_STATE_MESSAGE, "서버가 연결되어 있지 않습니다.");

				//======================================
				// 서버 이름을 다시 설정할 수 있다
				//======================================
				EnableWindow( GetDlgItem( hWnd, IDC_EDIT_SRVNAME ), TRUE );

				//======================================
				// 서버 시작 버튼 활성화
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_START ), TRUE );

				//======================================
				// 종료 버튼은 비활성화
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_END ), FALSE );
			}
			break;
		case WM_DESTROY:
			{
				//======================================
				// 혹시 서버가 켜져 잇다면 꺼주자
				//======================================
				g_srvControl->EndServer();

				//======================================
				// 종료
				//======================================
				EndDialog( hWnd, 0 );
			}
		}
	}
	return FALSE;
}
