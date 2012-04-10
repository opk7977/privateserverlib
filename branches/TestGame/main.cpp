#include "main.h"

#include "WinMgr.h"
#include "STime.h"
#include "SInput.h"

#include "CharacterMgr.h"
#include "Network.h"


void Init();
void Relase();
//////////////////////////////////////////////////////////////////////////
void BeginScene( HWND hWnd );
void EndScene( HWND hWnd );

void FrameMove( HWND hWnd );
void Render( HWND hWnd );
//////////////////////////////////////////////////////////////////////////
void KeyFrame();


//////////////////////////////////////////////////////////////////////////
HDC			g_hDc;
HDC			g_memoryDc;
HBITMAP		g_memoryBit;
HBITMAP		g_oldBit;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
SInput					g_input;
STime					g_timer;
float					g_frame = 0.f;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 시작
BOOL isStart = FALSE;
int Min = 5;
int Sec = 0;
//////////////////////////////////////////////////////////////////////////


char ip[20] = {0};
int sessionID = 0;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpzCmdParam, int nCmdShow )
{
	HWND hWnd;

	WinMgr window;
	if( !window.CreateWindows( hInstance, _T("Test"), _T("TestGame"), hWnd, 800, 600, nCmdShow ) )
		return 0;

	//Setting
	Init();

	MSG Message;

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
			BeginScene( hWnd );

			FrameMove( hWnd );
			Render( hWnd );

			EndScene( hWnd );
		}
	}

	Relase();
	return (int)Message.wParam;
}

void Init()
{
	// Server Setting
	if( !GetNetwork.Init( TRUE ) )
		return;

	if( !GetNetwork.ConnectToSrv( ip, 8800 ) )
		return;

	g_timer.Init();

	GetCharMgr.SetHeroId( sessionID );
	GetCharMgr.SetPlayerCount( 2 );
}

void Relase()
{

}

void BeginScene( HWND hWnd )
{
	g_hDc			= GetDC(hWnd);
	g_memoryDc		= CreateCompatibleDC( g_hDc );
	g_memoryBit		= CreateCompatibleBitmap( g_hDc, 800, 600 );
	g_oldBit		= (HBITMAP)SelectObject( g_memoryDc, g_memoryBit);
}

void EndScene( HWND hWnd )
{
	// 빽 버퍼를 화면으로 복사하자.
	BitBlt(g_hDc, 0, 0, 800, 600, g_memoryDc, 0, 0, SRCCOPY );

	// DC를 사용했으면 원래의 설정으로 되돌리자.
	SelectObject(g_hDc, g_oldBit);
	DeleteObject(g_memoryBit);
	DeleteDC(g_memoryDc);

	ReleaseDC(hWnd, g_hDc);
}

void FrameMove( HWND hWnd )
{
	g_timer.ProcessTime();
	g_frame += g_timer.GetElapsedTime();

	float frame = 0.1f;

	//1초마다 갱신하기
	if( g_frame >= frame )
	{
		g_frame -= frame;

		KeyFrame();

		//캐릭터들을 움직여 준다.
		GetCharMgr.MoveCharacter();

	}	
}

void Render( HWND hWnd )
{
	//우선 흰바탕
	RECT rc;
	rc.top = rc.left = 0;
	rc.bottom = 600;
	rc.right= 800;

	Rectangle( g_memoryDc, rc.left, rc.top, rc.right, rc.bottom );
 
 	//캐릭터들도 그려주자
 	GetCharMgr.RenderAll( &g_memoryDc );

	//시간을 표시
	char time[10];
	sprintf_s( time, "%2d : %2d", Min, Sec );
	TextOutA( g_memoryDc, 10, 10, time, strlen(time) );
}


void KeyFrame()
{
	//연결이 되어 있지 않다면 실행될 수 없다!!!
	if( !isStart )
		return;

	g_input.KeyboardProcess();
	unsigned char keyFlag = 0x00;

	CCharacter* myChar;
	myChar = GetCharMgr.GetHero();

	if( myChar == NULL )
		return;

	//Getkey
	//w 위
	if( g_input.GetKey( 0x77 ) || g_input.GetKey( 0x57 ) )
		keyFlag = keyFlag | 0x01;
	//a 왼쪽
	if( g_input.GetKey( 0x61 ) || g_input.GetKey( 0x41 ) )
		keyFlag = keyFlag | 0x08;
	//s 아래
	if( g_input.GetKey( 0x73 ) || g_input.GetKey( 0x53 ) )
		keyFlag = keyFlag | 0x10;
	//d 오른쪽
	if( g_input.GetKey( 0x64 ) || g_input.GetKey( 0x44 ) )
		keyFlag = keyFlag | 0x80;


	switch( keyFlag )
	{
	case STAND:
		myChar->SetTransition( CCharacter::EVENT_KYE_UP );
		break;
	case UP:
		myChar->SetTransition( CCharacter::EVENT_U_DOWN );
		break;
	case LEFT:
		myChar->SetTransition( CCharacter::EVENT_L_DOWN );
		break;
	case LEFT_UP:
		myChar->SetTransition( CCharacter::EVENT_UL_DOWN );
		break;
	case DOWN:
		myChar->SetTransition( CCharacter::EVENT_B_DOWN );
		break;
	case LEFT_DOWN:
		myChar->SetTransition( CCharacter::EVENT_LB_DOWN );
		break;
	case RIGHT:
		myChar->SetTransition( CCharacter::EVENT_R_DOWN );
		break;
	case RIGHT_BOTTOM:
		myChar->SetTransition( CCharacter::EVENT_BR_DOWN );
		break;
	case RIGHT_UP:
		myChar->SetTransition( CCharacter::EVENT_RU_DOWN );
		break;
	}
}

