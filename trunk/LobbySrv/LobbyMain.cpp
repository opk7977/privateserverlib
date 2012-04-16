#include "LobbyMain.h"
#include "Network.h"
#include "SThreadMgr.h"
#include "Room.h"

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

	//�� ����
	GetRoomMgr.CreateRoomSpace();

	return TRUE;
}

void LobbyMain::Release()
{

}
