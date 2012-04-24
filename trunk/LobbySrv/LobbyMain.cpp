#include "LobbyMain.h"
#include "Network.h"
/*#include "SThreadMgr.h"*/
#include "Room.h"
#include "CheckDB.h"

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
	if( !GetNetwork.Init( BASIC_SESSIONSPACE ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( 8900 ) )
		return FALSE;

	//방 셋팅
	GetRoomMgr.CreateRoomSpace();

	//DB연결
	if( !GetDB.Init( _T("GameAccount.mdb")) )
		return FALSE;

	return TRUE;
}

void LobbyMain::Release()
{
}
