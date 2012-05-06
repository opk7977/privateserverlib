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
	//서버 초기화
	if( !GetNetwork.Init( GetData.SessionCount ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( GetData.LobbySrvPortNum ) )
		return FALSE;

	//방 셋팅
	GetRoomMgr.CreateRoomSpace();

	//캐릭터 공간 셋팅
	GetCharMgr.Init();


	//DB연결
	if( !GetDB.Init( _T("GameAccount.mdb")) )
		return FALSE;

	return TRUE;
}

void LobbyMain::Release()
{
}
