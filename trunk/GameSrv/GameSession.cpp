#include "GameSession.h"
#include "GameProtocol.h"
#include "SLogger.h"

#include "CharMgr.h"
#include "GameMgr.h"
#include "GameProc.h"


SIMPLEMENT_DYNAMIC(GameSession)
SIMPLEMENT_DYNCREATE(GameSession)

GameSession::GameSession(void)
{
	Init();
}

GameSession::~GameSession(void)
{
}

void GameSession::Init()
{
	m_myCharInfo = NULL;
	m_myGameProc = NULL;
	isEndGame	= TRUE;
}

void GameSession::OnCreate()
{
	SSession::OnCreate();

	SendPacket( SC_GAME_CONNECT_OK );
}

void GameSession::OnDestroy()
{
	if( isEndGame )
	{
		//������ ������ ����
		//user�� ���� ���� player�� ���� ������ �������� �˸���.
		if( m_myGameProc != NULL )
		{
			//�濡�� ���� �����ϰ�
			m_myGameProc->DelPlayer( this );

			//�� ����鿡�� ��Ŷ ����
			SendCharDisconnect();
		}
		
	}
	else
	{
		//�κ�� �̵�
	}

	//ĳ���� ������ ������ ���� �ش�.
	if( m_myCharInfo == NULL )
		GetCharMgr.ReturnCharSpace( m_myCharInfo );

	//���� �ʱ�ȭ
	Init();

	SSession::OnDestroy();
}

void GameSession::PackageMyInfo( SPacket& packet )
{
	{
		SSynchronize sync( this );

		packet << m_myCharInfo->GetIndexId();
		packet << m_myCharInfo->GetState();
		packet << m_myCharInfo->GetPosX();
		packet << m_myCharInfo->GetPosY();
		packet << m_myCharInfo->GetPosZ();
		packet << m_myCharInfo->GetDirX();
		packet << m_myCharInfo->GetDirY();
		packet << m_myCharInfo->GetDirZ();
	}
}

void GameSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	//==============================================================> Client
	default:
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::PacketParsing()\n���� ��Ŷ�� ���̵� ��ȿ���� �ʽ��ϴ�.\n\n") );
	}
}

//======================================
// ���� ��Ŷ ó��
//======================================
//--------------------------------------
// Lobby Srv���� Ŀ�´����̼�
//--------------------------------------

//--------------------------------------
// Client���� Ŀ�´����̼�
//--------------------------------------
void GameSession::RecvInGame( SPacket &packet )
{
	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	//test////////////////////////////////////////////////////////////////////
	//������ ������ 1�� ��....1���� ���� ����
	roomNum = 1;
	//////////////////////////////////////////////////////////////////////////

	//ĳ���͸� �˻�
	m_myCharInfo = GetCharMgr.FindCharAsSessionId( sessionId );
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvInGame()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}
	
	m_myGameProc = GetGameMgr.FindGame( roomNum );
	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvInGame()\n�ش� ���� ���μ����� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	//�濡 ���� ���
	m_myGameProc->AddPlayer( this );

	//���� ������� ����(�� ���� ����)�� �����Ժ���
	SendOtherCharInfoToMe();

	//���� ����鿡�� �� ������ ����
	SendMyCharInfoToInGamePlayer();
}


//======================================
// ������ ��Ŷ ó��
//======================================
//--------------------------------------
// Lobby Srv���� Ŀ�´����̼�
//--------------------------------------

//--------------------------------------
// Client���� Ŀ�´����̼�
//--------------------------------------

BOOL GameSession::SendOtherCharInfoToMe()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//���ӳ��� ����� ������ ��´�.
	m_myGameProc->PackageAllPlayerInGame( sendPacket );

	int retval = SendPacket( sendPacket );
	if( retval != sendPacket.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMyCharInfoToInGamePlayer()\n���۵� ��Ŷũ��� ��Ŷ�� ũ�� ��ġ ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL GameSession::SendMyCharInfoToInGamePlayer()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//�� ������ ������
	sendPacket << (int)1;
	PackageMyInfo( sendPacket );

	//�� ���� �� ������
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendCharDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHAR_DISCONNECT );

	//�ڽ��� ���� ID�� �־� �ش�
	sendPacket << m_myCharInfo->GetIndexId();

	if( m_myGameProc == NULL )
		return FALSE;

	//���� �̹� ���� ������ ��ο��� ������ �ȴ�.
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
