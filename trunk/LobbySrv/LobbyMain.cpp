#include "LobbyMain.h"
//#include "CheckDB.h"
#include "Network.h"
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
	m_dbMgr		= &GetDBSrv;
	//m_dbMgr		= &GetDB;
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
	// 로그서버 접속
	//======================================

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
	// db 초기화/ 설정
	//======================================
	// 	if( !m_dbMgr->Init( _T("GameAccount.mdb")) )
	// 		return FALSE;
	if( !m_dbMgr->Init() )
		return FALSE;

	if( !m_dbMgr->ConnectToDBSrv( m_document->DBSrvIp, m_document->DBSrvPortNum ) )
		return FALSE;

	return TRUE;
}

void LobbyMain::Release()
{
	//m_dbMgr->Release();
	m_dbMgr->DisConnect();
}
