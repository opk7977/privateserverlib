#include "DBMain.h"

#include "DataLeader.h"
#include "PlayerMgr.h"
#include "Network.h"
#include "DBMgr.h"


DBMain::DBMain(void)
{
	m_document	= &GetDocument;
	m_playerMgr = &GetPlayerMgr;
	m_network	= &GetNetwork;
	m_dbMgr		= &GetDBMgr;
}

DBMain::~DBMain(void)
{
}

BOOL DBMain::Init()
{
	//======================================
	// db설정
	//======================================
	if( !m_dbMgr->Init( _T("Unknown"), _T("sa"), _T("1234") ) )
		return FALSE;

	//======================================
	// rankData 저장해 두기
	//======================================
	if( !m_dbMgr->RankDataSetting() )
		return FALSE;

	//======================================
	// 캐릭터 공간 만들어 두기
	//======================================
	m_playerMgr->Init();

	//======================================
	// 서버 초기화
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	//======================================
	// 서버 셋팅
	//======================================
	if( !m_network->SrvSetting( 9000 ) )
		return FALSE;

	//======================================
	// 로그 서버 접속
	//======================================

	return TRUE;
}

void DBMain::Release()
{
	//======================================
	// db해제
	//=====================================
	m_dbMgr->Release();
}
