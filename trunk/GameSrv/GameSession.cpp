#include "GameSession.h"
#include "GameProtocol.h"
#include "SLogger.h"

#include "CharMgr.h"
#include "GameMgr.h"
#include "GameProc.h"

#include "SrvNet.h"


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
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	case CS_GAME_MOVE_CHAR:
		RecvMoveChar( packet );
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
void GameSession::RecvLobbyConnectOK()
{
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvLobbyConnectOK()\n�κ񼭹��� ���ῡ �����Ͽ����ϴ�\n\n") );

	GetSrvNet.SetSession( this );

	SPacket sendPacket;
	sendPacket.SetID( GL_CONNECT_SERVER );

	GetSrvNet.SendToLobbyServer( sendPacket );
}


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

void GameSession::RecvMoveChar( SPacket &packet )
{
	int state;
	POINT3 pos, dir;
	packet >> state;
	//��ġ
	packet >> pos.m_X;
	packet >> pos.m_Y;
	packet >> pos.m_Z;
	//����
	packet >> dir.m_X;
	packet >> dir.m_Y;
	packet >> dir.m_Z;

	{
		SSynchronize sync( m_myCharInfo );

		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvMoveChar()\nĳ���� ������ ������ �ֽ��ϴ�.\n\n") );
			return;
		}

		m_myCharInfo->SetState( state );
		m_myCharInfo->SetPosition( pos );
		m_myCharInfo->SetDirection( dir );
	}

	SendMoveChar();
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

BOOL GameSession::SendMoveChar()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_MOVE_CHAR );

	{
		SSynchronize sync( m_myCharInfo );

		sendPacket << m_myCharInfo->GetIndexId();
		sendPacket << m_myCharInfo->GetState();
		sendPacket << m_myCharInfo->GetPosX();
		sendPacket << m_myCharInfo->GetPosY();
		sendPacket << m_myCharInfo->GetPosZ();
		sendPacket << m_myCharInfo->GetDirX();
		sendPacket << m_myCharInfo->GetDirY();
		sendPacket << m_myCharInfo->GetDirZ();
	}

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMoveChar()\n�������� ������ �ֽ��ϴ�.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}
