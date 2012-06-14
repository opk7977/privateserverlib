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
	// db �ʱ�ȭ/ ����
	//======================================
// 	if( !m_dbMgr->Init( _T("GameAccount.mdb") ) )
// 		return FALSE;

	//======================================
	// ���� �ʱ�ȭ
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	//======================================
	// ���� ����
	//======================================
	if( !m_network->SrvSetting( m_document->LoginSrvPortNum ) )
		return FALSE;

	//======================================
	// �α� ���� ����
	//======================================
// 	if( !m_logSrv->Init() )
// 		return FALSE;
// 
// 	if( !m_logSrv->ConnectToSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
// 		return FALSE;

	//======================================
	// �ӽ� session���� �ʱ�ȭ
	//======================================
	m_tmpSpace->Init( m_document->SessionCount );

	//======================================
	// DB ���� ����
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
