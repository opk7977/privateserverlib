#include "GameSrvMain.h"
#include "Network.h"
//GameProc


GameSrvMain::GameSrvMain(void)
{
}

GameSrvMain::~GameSrvMain(void)
{
	Release();
}

BOOL GameSrvMain::Init()
{
	//서버 초기화
	if( !GetNetwork.Init( 100 ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( 7900 ) )
		return FALSE;

	//게임proc셋팅
	
	return TRUE;
}

void GameSrvMain::Release()
{
}
