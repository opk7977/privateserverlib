#include "DBMain.h"

#include "DataLeader.h"
#include "PlayerMgr.h"
#include "Network.h"
#include "DBMgr.h"
#include "DBFileMgr.h"
#include "LogSrvMgr.h"

#include "DBProtocol.h"

DBMain::DBMain(void)
{
	m_document	= &GetDocument;
	m_playerMgr = &GetPlayerMgr;
	m_network	= &GetNetwork;

	if( m_document->isConnectSrv )
		m_dbMgr		= &GetDBMgr;
	else
		m_dbMgr		= &GetDBFileMgr;

#ifdef CONNECT_LOG_SERVER
	m_logSrv	= &GetLogSrvMgr;
#endif
}

DBMain::~DBMain(void)
{
	//Release();
}

BOOL DBMain::Init()
{
	//======================================
	// db설정
	//======================================
	//if( !m_dbMgr->Init( _T("Unknown"), _T("sa"), _T("1234") ) )
	if( !m_dbMgr->Init( m_document->DBName, m_document->DBSrvID, m_document->DBSrvPW ) )
		return FALSE;

	//======================================
	// 캐릭터 공간 만들어 두기
	//======================================
	m_playerMgr->Init();

	//======================================
	// 서버 초기화 셋팅
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	if( !m_network->SrvSetting( 9000 ) )
		return FALSE;

#ifdef CONNECT_LOG_SERVER
	//======================================
	// 로그 서버 연결
	//======================================
	if( !m_logSrv->Init( DB_SERVER_ID ) )
		return FALSE;

	if( !m_logSrv->ConnectToLogSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
		return FALSE;
#endif

	//======================================
	// rankData 저장해 두기
	//======================================
	if( !m_dbMgr->RankDataSetting() )	
		return FALSE;

	return TRUE;
}

void DBMain::Release()
{
	//======================================
	// network해제
	//======================================
	m_network->Release();

	//======================================
	// db해제
	//======================================
	m_dbMgr->Release();
}
