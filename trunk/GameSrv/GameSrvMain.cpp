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
	//���� �ʱ�ȭ
	if( !GetNetwork.Init( 100 ) )
		return FALSE;

	//���� ����
	if( !GetNetwork.SrvSetting( 7900 ) )
		return FALSE;

	//����proc����
	
	return TRUE;
}

void GameSrvMain::Release()
{
}
