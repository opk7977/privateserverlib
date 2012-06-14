#include "LoginMain.h"
/*#include "LoginDB.h"*/
#include "Network.h"
#include "LogSrvNet.h"
#include "TmpSessionSpace.h"
#include "DBSrvMgr.h"

#include "DataLeader.h"


LoginMain::LoginMain(void)
{
	m_network	= &GetNetwork;
/*	m_dbMgr		= &GetDBMgr;*/
	m_document	= &GetDocument;
	m_logSrv	= &GetSrvNet;
	m_tmpSpace	= &GetTmpSpace;
	m_dbSrvMgr	= &GetDBSrv;
	
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
// 	if( !m_dbMgr->Init( _T("GameAccount.mdb") ) )
// 		return FALSE;

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

	//======================================
	// 로그 서버 접속
	//======================================
// 	if( !m_logSrv->Init() )
// 		return FALSE;
// 
// 	if( !m_logSrv->ConnectToSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
// 		return FALSE;

	//======================================
	// 임시 session공간 초기화
	//======================================
	m_tmpSpace->Init( m_document->SessionCount );

	//======================================
	// DB 서버 접속
	//======================================
	if( !m_dbSrvMgr->Init() )
		return FALSE;

	if( !m_dbSrvMgr->ConnectToDBSrv( m_document->DBSrvIp, m_document->DBSrvPortNum ) )
		return FALSE;

	return TRUE;
}

void LoginMain::Relase()
{
}
