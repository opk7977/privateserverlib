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
	// 캐릭터 공간 셋팅
	//======================================
	m_charMgr->Init();

	//======================================
	// 지뢰ITEM(?) 공간 셋팅
	//======================================
	m_itemMgr->Init();

	//======================================
	// 게임proc셋팅
	//======================================
	m_gameMgr->CreateGameProc( m_document->RoomCount );

	//======================================
	// 서버 초기화/ 셋팅
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	if( !m_network->SrvSetting( m_document->GameSrvPortNum ) )
		return FALSE;

	//======================================
	// 로그서버 접속
	//======================================
#ifdef CONNECT_LOG_SERVER
	if( !m_logSrv->Init( LOBBY_SERVER_ID ) )
		return FALSE;

	if( !m_logSrv->ConnectToLogSrv( m_document->LogSrvIP, m_document->LogSrvPortNum ) )
		return FALSE;
#endif

	//======================================
	// db 초기화/ 설정
	//======================================
	if( !m_dbMgr->Init() )
		return FALSE;

	if( !m_dbMgr->ConnectToDBSrv( m_document->DBSrvIp, m_document->DBSrvPortNum ) )
		return FALSE;
	
	//======================================
	// 로비서버와 접속
	//======================================
	if( !GetSrvNet.ConnectToSrv( m_document->LobbySrvIP, m_document->LobbySrvPortNum ) )
		return FALSE;

	return TRUE;
}

void GameSrvMain::Release()
{
	//======================================
	// network해제
	//======================================
	m_network->Release();
}
