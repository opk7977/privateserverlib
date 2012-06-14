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
	// �� ����
	//======================================
	m_roomMgr->CreateRoomSpace();

	//======================================
	// ĳ���� ���� ����
	//======================================
	m_charMgr->Init();

	//======================================
	// �α׼��� ����
	//======================================

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
	// db �ʱ�ȭ/ ����
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
