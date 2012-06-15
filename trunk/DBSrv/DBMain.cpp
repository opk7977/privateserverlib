#include "DBMain.h"

#include "DataLeader.h"
#include "PlayerMgr.h"
#include "Network.h"
#include "DBMgr.h"
#include "LogSrvMgr.h"

#include "DBProtocol.h"

DBMain::DBMain(void)
{
	m_document	= &GetDocument;
	m_playerMgr = &GetPlayerMgr;
	m_network	= &GetNetwork;
	m_dbMgr		= &GetDBMgr;

#ifdef CONNECT_LOG_SERVER
	m_logSrv	= &GetLogSrvMgr;
#endif
}

DBMain::~DBMain(void)
{
	Release();
}

BOOL DBMain::Init()
{
	//======================================
	// db����
	//======================================
	if( !m_dbMgr->Init( _T("Unknown"), _T("sa"), _T("1234") ) )
		return FALSE;

	//======================================
	// ĳ���� ���� ����� �α�
	//======================================
	m_playerMgr->Init();

	//======================================
	// ���� �ʱ�ȭ ����
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	if( !m_network->SrvSetting( 9000 ) )
		return FALSE;

#ifdef CONNECT_LOG_SERVER
	//======================================
	// �α� ���� ����
	//======================================
	if( !m_logSrv->Init( DB_SERVER_ID ) )
		return FALSE;

	if( !m_logSrv->ConnectToLogSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
		return FALSE;
#endif

	//======================================
	// rankData ������ �α�
	//======================================
	if( !m_dbMgr->RankDataSetting() )	
		return FALSE;

	return TRUE;
}

void DBMain::Release()
{
	//======================================
	// db����
	//=====================================
	m_dbMgr->Release();
}
