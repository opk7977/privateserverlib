#include "LobbyMain.h"
#include "CheckDB.h"
#include "Network.h"

#include "Room.h"
#include "LobbyChar.h"

#include "DataLeader.h"

LobbyMain::LobbyMain(void)
{
	m_network	= &GetNetwork;
	m_roomMgr	= &GetRoomMgr;
	m_charMgr	= &GetCharMgr;
	m_document	= &GetDocument;
	m_dbMgr		= &GetDB;
}

LobbyMain::~LobbyMain(void)
{
	Release();
}

BOOL LobbyMain::Init()
{
	//======================================
	// db �ʱ�ȭ/ ����
	//======================================
	if( !m_dbMgr->Init( _T("GameAccount.mdb")) )
		return FALSE;

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

	return TRUE;
}

void LobbyMain::Release()
{
	m_dbMgr->Release();
}
