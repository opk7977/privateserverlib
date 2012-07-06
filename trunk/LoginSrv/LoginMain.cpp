#include "LoginMain.h"
#include "DataLeader.h"
#include "Network.h"
#include "LoginProtocol.h"
#include "TmpSessionSpace.h"

#include "SThreadMgr.h"

#include "DBSrvMgr.h"
#include "LogSrvMgr.h"


LoginMain::LoginMain(void)
{
	m_network	= &GetNetwork;
	m_document	= &GetDocument;
	m_tmpSpace	= &GetTmpSpace;
#ifdef CONNECT_LOG_SERVER
	m_logSrv	= &GetLogSrvMgr;
#endif
	m_dbSrvMgr	= &GetDBSrv;
	
}

LoginMain::~LoginMain(void)
{
	Relase();
}

BOOL LoginMain::Init()
{
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
#ifdef CONNECT_LOG_SERVER
	if( !m_logSrv->Init( LOGIN_SERVER_ID ) )
		return FALSE;

	if( !m_logSrv->ConnectToLogSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
		return FALSE;
#endif
	
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
	//======================================
	// network����
	//======================================
	m_network->Release();
}
