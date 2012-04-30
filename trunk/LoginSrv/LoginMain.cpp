#include "LoginMain.h"
#include "LoginDB.h"
#include "Network.h"
#include "SThreadMgr.h"

#include "DataLeader.h"


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
	if( !GetNetwork.Init( GetDocument.SessionCount ) )
		return FALSE;

	//서버 셋팅
	if( !GetNetwork.SrvSetting( GetDocument.LoginSrvPortNum ) )
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
