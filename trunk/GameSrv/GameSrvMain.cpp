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
	//���� �ʱ�ȭ
	if( !GetNetwork.Init( BASIC_SESSIONSPACE ) )
		return FALSE;

	//���� ����
	if( !GetNetwork.SrvSetting( GetData.GameSrvProtNum ) )
		return FALSE;

	//����proc����
	
	return TRUE;
}

void GameSrvMain::Release()
{
}
