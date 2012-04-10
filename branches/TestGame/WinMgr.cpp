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

			//������ ���� �ʰ� ��Ҹ� �������� �׳� ����
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
				//��� �׸��� ���� �ִ��� Ȯ���� �ϰ�
				int ip1 = GetDlgItemInt( hDlg, IDC_IP1, NULL, FALSE );
				int ip2 = GetDlgItemInt( hDlg, IDC_IP2, NULL, FALSE );
				int ip3 = GetDlgItemInt( hDlg, IDC_IP3, NULL, FALSE );
				int ip4 = GetDlgItemInt( hDlg, IDC_IP4, NULL, FALSE );
				int sid = GetDlgItemInt( hDlg, IDC_SESSIONID, NULL, FALSE );

				if( sid == 0 )
				{
					MessageBox( NULL, _T("id�� 0�� �Է��� �� �����ϴ�."), _T("Info"), MB_OK ); 
					return TRUE;
				}

				//�� ���� ������ ���� �ִ´�.
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
