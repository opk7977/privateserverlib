#include "WinMgr.h"
#include "SUDPNet.h"
#include "SPacketQueue.h"
#include "SPacket.h"
#include "SLogger.h"

#include "SSocket.h"

SSocket	sock;
sockaddr_in serveraddr;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Test"), _T("TestGame"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	/*SSocket	sock;*/

	sock.Init();
	sock.CreateUDPSock();

/*	struct sockaddr_in serveraddr;*/

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("192.168.0.56");
	serveraddr.sin_port = htons(7901);



	//Setting
// 	if( !GetUNet.Init( 3000 ) )
// 		return FALSE;

//	SPacket pack;
// 
// 	GetUNet.AddSockAddr( "192.168.0.56", 3000 );
// 	GetUNet.AddSockAddr( "192.168.0.59", 5000 );

	MSG Message;
//	int size;
/*	TCHAR tmp[50];*/

	while(1)
	{
		Sleep( 1 );

		if( PeekMessage( &Message, NULL, 0, 0, PM_REMOVE ) )
		{
			if( Message.message == WM_QUIT )
				break;

			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
		else
		{
// 			int dataCount = GetPacketQ.GetDataCount();
// 
// 			for( int i=0; i<dataCount; ++i )
// 			{
// 				ZeroMemory( tmp, 100 );
// 
// 				GetPacketQ.GetPacket( pack );
// 				pack >> size;
// 				pack.GetData( tmp, size );
// 
// 				GetLogger.PutLog( SLogger::LOG_LEVEL_CONINFO, _T("%s\n"), tmp );
// 			}
		}
	}

/*	GetUNet.ClientClear();*/
	sock.Release();
	return (int)Message.wParam;
}
