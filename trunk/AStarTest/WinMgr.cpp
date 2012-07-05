#include "WinMgr.h"
#include "MyData.h"


WinMgr::WinMgr(void)
{
	m_document = &GetDocument;
}

WinMgr::~WinMgr(void)
{
}

LRESULT WinMgr::WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_CREATE:
		ResizeWindows( hWnd, 600, 600 );
		break;
	case WM_RBUTTONDOWN:
		{
			int posX = LOWORD(lParam);
			int posY = HIWORD(lParam);

			int rowIndex = posX/60;
			int colIndex = posY/60;

			m_document->AddWell( rowIndex, colIndex );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			int posX = LOWORD(lParam);
			int posY = HIWORD(lParam);

			int rowIndex = posX/60;
			int colIndex = posY/60;

			m_document->SetPoint( rowIndex, colIndex );
		}
		break;

	case WM_MBUTTONDOWN:
		m_document->SetLoad();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
