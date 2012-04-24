#include "GameSrvMain.h"
#include "Network.h"
#include "DataLeader.h"

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
	if( !GetNetwork.Init( BASIC_SESSIONSPACE ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( GetData.GameSrvProtNum ) )
		return FALSE;

	//게임proc셋팅
	
	return TRUE;
}

void GameSrvMain::Release()
{
}
