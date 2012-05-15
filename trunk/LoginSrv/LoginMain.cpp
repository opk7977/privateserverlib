#include "LoginMain.h"
#include "LoginDB.h"
#include "Network.h"

#include "DataLeader.h"


LoginMain::LoginMain(void)
{
	m_network	= &GetNetwork;
	m_dbMgr		= &GetDBMgr;
	m_document	= &GetDocument;
}

LoginMain::~LoginMain(void)
{
	Relase();
}

BOOL LoginMain::Init()
{
	//======================================
	// db 초기화/ 설정
	//======================================
	if( !m_dbMgr->Init( _T("GameAccount.mdb") ) )
		return FALSE;

	//======================================
	// 서버 초기화
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	//======================================
	// 서버 셋팅
	//======================================
	if( !m_network->SrvSetting( m_document->LoginSrvPortNum ) )
		return FALSE;

	return TRUE;
}

void LoginMain::Relase()
{
	m_dbMgr->Release();
}
