#include "LobbyMain.h"
#include "Network.h"
#include "SThreadMgr.h"

LobbyMain::LobbyMain(void)
{
}

LobbyMain::~LobbyMain(void)
{
	Release();
}

BOOL LobbyMain::Init()
{
	//���� �ʱ�ȭ
	if( !GetNetwork.Init( 100 ) )
		return FALSE;

	//���� ����
	if( !GetNetwork.SrvSetting( 8900 ) )
		return FALSE;

	return TRUE;
}

void LobbyMain::Release()
{

}
