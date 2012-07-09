#include "GameSrvMain.h"

#include "DataLeader.h"

#include "GameMgr.h"
#include "CharMgr.h"

#include "Network.h"
#include "GameProtocol.h"
#include "ItemMgr.h"

#include "SrvNet.h"
#include "LogSrvMgr.h"
#include "DBSrvMgr.h"

GameSrvMain::GameSrvMain(void)
{
	m_document	= &GetDocument;
	m_gameMgr	= &GetGameMgr;
	m_charMgr	= &GetCharMgr;
	m_network	= &GetNetwork;
	m_itemMgr	= &GetItemMgr;
#ifdef CONNECT_LOG_SERVER
	m_logSrv	= &GetLogSrvMgr;
#endif
	m_dbMgr		= &GetDBSrv;
}

GameSrvMain::~GameSrvMain(void)
{
	//Release();
}

BOOL GameSrvMain::Init()
{
	//======================================
	// ĳ���� ���� ����
	//======================================
	m_charMgr->Init();

	//======================================
	// ����ITEM(?) ���� ����
	//======================================
	m_itemMgr->Init();

	//======================================
	// ����proc����
	//======================================
	m_gameMgr->CreateGameProc( m_document->RoomCount );

	//======================================
	// ���� �ʱ�ȭ/ ����
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	if( !m_network->SrvSetting( m_document->GameSrvPortNum ) )
		return FALSE;

	//======================================
	// �α׼��� ����
	//======================================
#ifdef CONNECT_LOG_SERVER
	if( !m_logSrv->Init( LOBBY_SERVER_ID ) )
		return FALSE;

	if( !m_logSrv->ConnectToLogSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
		return FALSE;
#endif

	//======================================
	// db �ʱ�ȭ/ ����
	//======================================
	if( !m_dbMgr->Init() )
		return FALSE;

	if( !m_dbMgr->ConnectToDBSrv( m_document->DBSrvIp, m_document->DBSrvPortNum ) )
		return FALSE;
	
	//======================================
	// �κ񼭹��� ����
	//======================================
	if( !GetSrvNet.ConnectToSrv( m_document->LobbySrvIP, m_document->LobbySrvPortNum ) )
		return FALSE;

	return TRUE;
}

void GameSrvMain::Release()
{
	//======================================
	// network����
	//======================================
	m_network->Release();
}
