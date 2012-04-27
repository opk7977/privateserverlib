#include "WinMgr.h"
#include "SPacket.h"
#include "SNetwork.h"
#include "NetProtocol.h"

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
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_RBUTTONDOWN:
		{
			SPacket sendPacket;
			sendPacket.SetID( CS_GAME_MOVE_CHAR );
			sendPacket << 1;	//����
			sendPacket << 2.f;	//��ġ
			sendPacket << 2.f;
			sendPacket << 2.f;
			sendPacket << 1.f;	//����
			sendPacket << 1.f;
			sendPacket << 1.f;

			if( GetNetwork.SendPacket( &sendPacket ) != sendPacket.GetPacketSize() )
				MessageBox( NULL, _T("CS_GAME_MOVE_CHAR\n���۷��� ��Ŷũ�Ⱑ �ٸ�"), _T("����"), MB_OK );
		}
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
