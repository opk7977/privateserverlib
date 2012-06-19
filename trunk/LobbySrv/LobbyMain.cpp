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
	// �� ����
	//======================================
	m_roomMgr->CreateRoomSpace();

	//======================================
	// ĳ���� ���� ����
	//======================================
	m_charMgr->Init();

	//======================================
	// ���� �ʱ�ȭ
	//======================================
	if( !m_network->Init( m_document->SessionCount ) )
		return FALSE;

	//======================================
	// ���� ����
	//======================================
	if( !m_network->SrvSetting( m_document->LobbySrvPortNum ) )
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

	return TRUE;
}

void LobbyMain::Release()
{
	m_dbMgr->DisConnect();
}
