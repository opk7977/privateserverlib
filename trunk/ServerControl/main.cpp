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
	//icon�� ����..
	hInst = hInstance;

	//======================================
	// ���ؽ� �˻�
	//======================================
	if( !CheckValid( _T("ServerControl") ) )
	{
		MessageBox( NULL, _T("�̹� ���α׷��� ���� �ֽ��ϴ�."), _T("�ٽ� Ȯ���ϼ���"), MB_OK );
		return 0;
	}

 	DialogBox( hInstance, MAKEINTRESOURCE( IDD_DLG_SRVCtrl ), HWND_DESKTOP, DlgProc );

	//======================================
	// ���ؽ� ����
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

			//ū ������
			SendMessage( hWnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)bIcon );
			//���� ������
			SendMessage( hWnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)sIcon );

			SetDlgItemTextA( hWnd, IDC_EDIT_SRVNAME, "Unknown_����" );
			EnableWindow( GetDlgItem( hWnd, ID_SRV_END ), FALSE );
			
			//======================================
			// �ʿ��� ���� ���ϵ��� ��� �ִ��� üũ
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
				// ���� ����
				//======================================
				if( !g_srvControl->StartServer( serverName ) )
				{
					//���� ���� ����!
					MessageBox( NULL, _T("���� ���� ����!"), _T("error"), MB_OK );
					EndDialog( hWnd, 0 );
				}

				//======================================
				// ���� �̸��� ���� �������� �ʾƵ� ��
				//======================================
				EnableWindow( GetDlgItem( hWnd, IDC_EDIT_SRVNAME ), FALSE );

				//======================================
				// ������ �Ѵ� ��ư�� ��Ȱ��ȭ
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_START ), FALSE );

				//======================================
				// ���� ���� ��ư�� Ȱ��ȭ
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_END ), TRUE );

				//======================================
				// ���� message�� �ٲ� �ش�
				//======================================
				char msg[64]={0,};
				sprintf_s( msg, 64, "������(%s)", serverName );
				SetDlgItemTextA( hWnd, IDC_STATE_MESSAGE, msg );
			}
			break;
		case ID_SRV_END:
			{
				//======================================
				// ���� ����
				//======================================
				g_srvControl->EndServer();

				SetDlgItemTextA( hWnd, IDC_STATE_MESSAGE, "������ ����Ǿ� ���� �ʽ��ϴ�.");

				//======================================
				// ���� �̸��� �ٽ� ������ �� �ִ�
				//======================================
				EnableWindow( GetDlgItem( hWnd, IDC_EDIT_SRVNAME ), TRUE );

				//======================================
				// ���� ���� ��ư Ȱ��ȭ
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_START ), TRUE );

				//======================================
				// ���� ��ư�� ��Ȱ��ȭ
				//======================================
				EnableWindow( GetDlgItem( hWnd, ID_SRV_END ), FALSE );
			}
			break;
		case WM_DESTROY:
			{
				//======================================
				// Ȥ�� ������ ���� �մٸ� ������
				//======================================
				g_srvControl->EndServer();

				//======================================
				// ����
				//======================================
				EndDialog( hWnd, 0 );
			}
		}
	}
	return FALSE;
}
