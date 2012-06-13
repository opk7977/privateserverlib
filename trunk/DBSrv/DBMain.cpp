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
	// db����
	//======================================
	if( !m_dbMgr->Init( _T("Unknown"), _T("sa"), _T("1234") ) )
		return FALSE;

	//======================================
	// rankData ������ �α�
	//======================================
	if( !m_dbMgr->RankDataSetting() )
		return FALSE;

	//======================================
	// ĳ���� ���� ����� �α�
	//======================================
	m_playerMgr->Init();

	//======================================
	// ���� �ʱ�ȭ
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	//======================================
	// ���� ����
	//======================================
	if( !m_network->SrvSetting( 9000 ) )
		return FALSE;

	//======================================
	// �α� ���� ����
	//======================================

	return TRUE;
}

void DBMain::Release()
{
	//======================================
	// db����
	//=====================================
	m_dbMgr->Release();
}
