#include "LobbyMain.h"
#include "Network.h"
#include "Room.h"
#include "CheckDB.h"
#include "LobbyChar.h"

#include "DataLeader.h"

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
	if( !GetNetwork.Init( GetData.SessionCount ) )
		return FALSE;

	//���� ����
	if( !GetNetwork.SrvSetting( GetData.LobbySrvPortNum ) )
		return FALSE;

	//�� ����
	GetRoomMgr.CreateRoomSpace();

	//ĳ���� ���� ����
	GetCharMgr.Init();


	//DB����
	if( !GetDB.Init( _T("GameAccount.mdb")) )
		return FALSE;

	return TRUE;
}

void LobbyMain::Release()
{
}
