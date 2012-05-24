#include "LoginMain.h"
#include "LoginDB.h"
#include "Network.h"
#include "LogSrvNet.h"

#include "DataLeader.h"


LoginMain::LoginMain(void)
{
	m_network	= &GetNetwork;
	m_dbMgr		= &GetDBMgr;
	m_document	= &GetDocument;
	m_logSrv	= &GetSrvNet;
	
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
	if( !m_dbMgr->Init( _T("GameAccount.mdb") ) )
		return FALSE;

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

	return TRUE;
}

void LoginMain::Relase()
{
	m_dbMgr->Release();
}
