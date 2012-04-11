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
	//db �ʱ�ȭ/ ����
	if( !GetDBMgr.Init( _T("GameAccount.mdb") ) )
		return FALSE;

	//���� �ʱ�ȭ
	if( !GetNetwork.Init( 30 ) )
		return FALSE;

	//���� ����
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