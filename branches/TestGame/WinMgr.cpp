#include "resource.h"
#include "WinMgr.h"

extern char ip[20];
extern int sessionID;



int CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );

WinMgr::WinMgr(void)
{
}

WinMgr::~WinMgr(void)
{
}

LRESULT WinMgr::WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CREATE:
		{
			int retval = DialogBox( m_hInst, MAKEINTRESOURCE( IDD_DIALOG1 ), hWnd, DlgProc );

			//연결을 하지 않고 취소를 눌렀으면 그냥 끄자
			if( retval == IDCANCEL )
				PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int CALLBACK DlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		SetDlgItemInt( hDlg, IDC_IP1, 192, FALSE );
		SetDlgItemInt( hDlg, IDC_IP2, 168, FALSE );
		SetDlgItemInt( hDlg, IDC_IP3, 0, FALSE );
		SetDlgItemInt( hDlg, IDC_IP4, 70, FALSE );
		SetDlgItemInt( hDlg, IDC_SESSIONID, 0, FALSE );
		return TRUE;

	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDOK:
			{
				//모든 항목이 적혀 있는지 확인을 하고
				int ip1 = GetDlgItemInt( hDlg, IDC_IP1, NULL, FALSE );
				int ip2 = GetDlgItemInt( hDlg, IDC_IP2, NULL, FALSE );
				int ip3 = GetDlgItemInt( hDlg, IDC_IP3, NULL, FALSE );
				int ip4 = GetDlgItemInt( hDlg, IDC_IP4, NULL, FALSE );
				int sid = GetDlgItemInt( hDlg, IDC_SESSIONID, NULL, FALSE );

				if( sid == 0 )
				{
					MessageBox( NULL, _T("id에 0을 입력할 수 없습니다."), _T("Info"), MB_OK ); 
					return TRUE;
				}

				//다 적혀 있으면 값을 넣는다.
				sessionID = sid;
				sprintf_s( ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4 );
			}
			
			EndDialog( hDlg, IDOK );
			return TRUE;
		case IDCANCEL:
			EndDialog( hDlg, IDCANCEL );
			return TRUE;
		}
		break;
	}

	return FALSE;
}
