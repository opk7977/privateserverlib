#include "SWindow.h"
#include "TmpPacketQueue.h"
#include "PreUDPNet.h"
#include "STime.h"
#include "SLogger.h"


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	//======================================
	// 로그 초기화
	//======================================
	GetLogger.Create( "testUDP2" );

	SWindow window;
	if( !window.CreateWindows( hInstance, _T("Test"), _T("TestUdp2"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	TmpPacketParser* tmpParser;
	tmpParser = &GetPrePacketParser;
	tmpParser->Init();

	PreUDPNet* preNet;
	preNet = &GetPreUDP;
	preNet->Init();



	MSG Message;

	STime timer;
	timer.Init();

	float timeF = 0.0f;
	float endTiem = 0.0f;

	BOOL isGOOD = TRUE;

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
// 			timer.ProcessTime();
// 
// 			timeF += timer.GetElapsedTime();
// 			if( timeF >= 0.5f )
// 			{
// 				timeF = 0.0f;
// 
// 				if( isGOOD )
// 					tmpParser->PacketProcess();
// 				else
// 					GetLogger.PutLog( SLogger::LOG_LEVEL_CONINFO, _T("끗\n") );
// 			}
// 			if( isGOOD )
// 			{
// 				endTiem += timer.GetElapsedTime();
// 				if( endTiem >= 5.0f )
// 				{
// 					preNet->Release();
// 					tmpParser->Release();
// 
// 					isGOOD = FALSE;
// 				}
// 			}	
			timer.ProcessTime();

			timeF += timer.GetElapsedTime();
			if( timeF >= 0.5f )
			{
				timeF = 0.0f;

				tmpParser->PacketProcess();
			}	
		}
	}

	preNet->Release();
	tmpParser->Release();
	GetLogger.Release();
	return (int)Message.wParam;
}
