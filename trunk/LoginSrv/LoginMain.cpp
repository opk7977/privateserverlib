#include "LoginMain.h"
#include "LoginDB.h"
#include "Network.h"
#include "SThreadMgr.h"

LoginMain::LoginMain(void)
{
}

LoginMain::~LoginMain(void)
{
	Relase();
}

BOOL LoginMain::Init()
{
	//db 초기화/ 설정
	if( !GetDBMgr.Init( _T("GameAccount.mdb") ) )
		return FALSE;

	//서버 초기화
	if( !GetNetwork.Init( 30 ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( 8880 ) )
		return FALSE;

	return TRUE;
}

void LoginMain::Relase()
{

}

void LoginMain::Run()
{
	GetThreadMgr.IsEndAllThread();
}
