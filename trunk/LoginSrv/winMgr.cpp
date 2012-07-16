#include "WinMgr.h"
#include "resource.h"
#include "DataLeader.h"

BOOL CALLBACK DlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

WinMgr::WinMgr(void)
{
}

WinMgr::~WinMgr(void)
{
}

void WinMgr::SetInstance( HINSTANCE& hInstance )
{
	m_hInst = hInstance;
}

LRESULT WinMgr::WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CREATE:
		ResizeWindows( hWnd, 500, 100 );
		DialogBox( m_hInst, MAKEINTRESOURCE( IDD_IN_SRVNAME ), HWND_DESKTOP, DlgProc );

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

BOOL CALLBACK DlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		SetDlgItemText( hWnd, IDC_EDIT_SRVNAME, GetDocument.SrvName );
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDOK:
			GetDlgItemText( hWnd, IDC_EDIT_SRVNAME, GetDocument.SrvName, 32 );
			EndDialog( hWnd,0 );
			return TRUE;
		}
	}

	return FALSE;
}
