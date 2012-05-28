#include "WinMgr.h"

#include "SUDPNet.h"
#include "SPacket.h"

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
	case WM_RBUTTONDOWN:
		{
			SPacket packet(0);
			packet << 10;
			packet.PutData( _T("����������"), 10 );

			GetUNet.SendPacketAllClient( packet );
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
