#include "LobbyMain.h"
#include "Network.h"
#include "LobbyProtocol.h"
#include "LogSrvMgr.h"
#include "DBSrvMgr.h"

#include "Room.h"
#include "LobbyChar.h"

#include "DataLeader.h"

LobbyMain::LobbyMain(void)
{
	m_network	= &GetNetwork;
	m_roomMgr	= &GetRoomMgr;
	m_charMgr	= &GetCharMgr;
	m_document	= &GetDocument;
#ifdef CONNECT_LOG_SERVER
	m_logSrv	= &GetLogSrvMgr;
#endif
	m_dbMgr		= &GetDBSrv;
}

LobbyMain::~LobbyMain(void)
{
	Release();
}

BOOL LobbyMain::Init()
{
	//======================================
	// 방 셋팅
	//======================================
	m_roomMgr->CreateRoomSpace();

	//======================================
	// 캐릭터 공간 셋팅
	//======================================
	m_charMgr->Init();

	//======================================
	// 서버 초기화
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	//======================================
	// 서버 셋팅
	//======================================
	if( !m_network->SrvSetting( m_document->LobbySrvPortNum ) )
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

	return TRUE;
}

void LobbyMain::Release()
{
	m_dbMgr->DisConnect();
}
