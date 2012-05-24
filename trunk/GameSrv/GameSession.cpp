#include "GameSession.h"
#include "GameProtocol.h"

#include "SrvNet.h"
#include "SLogger.h"
#include "CharMgr.h"
#include "ItemMgr.h"
#include "GameMgr.h"

#include "GameProc.h"


SIMPLEMENT_DYNAMIC(GameSession)
SIMPLEMENT_DYNCREATE(GameSession)

GameSession::GameSession(void)
: m_myCharInfo(NULL)
, m_myGameProc(NULL)
, isEndGame(TRUE)
{
	m_srvNet	= &GetSrvNet;
	m_logger	= &GetLogger;
	m_gameMgr	= &GetGameMgr;
	m_charMgr	= &GetCharMgr;
	m_itemMgr	= &GetItemMgr;
}

GameSession::~GameSession(void)
{
}

void GameSession::Clear()
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
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		//ĳ���� ������ ����...
		//������ ���ϴ�..
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("GameSession::OnDestroy()\n ĳ�������� ���� ������ ������ �����մϴ�. ������?\n\n") );
	}
	else
	{
		//ĳ���� ������ �ֽ��ϴ�.
		//���������� ���󽬴�..
		if( isEndGame )
		{
			//������ ������ ��
			//lobby������ ������ ���� ������ �˷� �ش�
			SendToSrvThatPlayerDisconnect();
		}

		//�濡�� ���� ����
		if( !m_myGameProc->DelPlayer( this ) )
		{
			//���� ���� ���� �ƹ��� ���� �ʾ���
			//�κ�� ���� ���� proc�� �������� �˸�?
			SendGameEnd( m_myGameProc->GetGameID() );

			//���� proc����
			m_myGameProc->EndGame();
		}
		else
		{
			//���� ���� �ִ� ���� �����ϰ� �ִ� ����鿡�� ���� �����°��� �˸�
			SendCharDisconnect();
		}

		//ĳ���� ������ ������ ���� �ش�.
		GetCharMgr.ReturnCharSpace( m_myCharInfo );
	}

	//���� �ʱ�ȭ
	Clear();

	SSession::OnDestroy();
}

// void GameSession::PackageMyInfo( SPacket& packet )
// {
// 	SSynchronize sync( this );
// 
// 	packet << m_myCharInfo->GetIndexId();
// 	packet << m_myCharInfo->GetState();
// 	packet << m_myCharInfo->GetPosX();
// 	packet << m_myCharInfo->GetPosY();
// 	packet << m_myCharInfo->GetPosZ();
// 	packet << m_myCharInfo->GetDirX();
// 	packet << m_myCharInfo->GetDirY();
// 	packet << m_myCharInfo->GetDirZ();
// 	packet << m_myCharInfo->GetDirInt();
// }

void GameSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case LG_START_GAME:
		RecvLobbyStartGame( packet );
		break;

	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	case CS_GAME_MOVE_CHAR:
		RecvMoveChar( packet );
		break;
	case CS_GAME_ATTECT:
		//
		break;
	case CS_GAME_SYNC:
		RecvGameSync( packet );
		break;
	case CS_GAME_CHANGE_STATE:
		RecvGameChangeState( packet );
		break;
	case CS_GAME_ROTATION:
		RecvGameRotation( packet );
		break;
	case CS_GAME_CHATTING:
		RecvGameChatting( packet );
		break;
	case CS_GAME_INSTALL_BOOM:
		//
		break;
	case CS_GAME_UNINSTALL_BOOM:
		//
		break;
	case CS_GAME_INSTALL_ITEM:
		//
		break;
	case CS_GAME_GOTO_LOBBY:
		//
		break;

	//==============================================================> Client
	default:
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::PacketParsing()\n(%d�� ID)���� ��Ŷ�� ���̵� ��ȿ���� �ʽ��ϴ�.\n\n"), packet.GetID() );
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
	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvLobbyConnectOK()\n�κ񼭹��� ���ῡ �����Ͽ����ϴ�\n\n") );

	GetSrvNet.SetSession( this );

	SPacket sendPacket;
	sendPacket.SetID( GL_CONNECT_SERVER );

	GetSrvNet.SendToLobbyServer( sendPacket );
}

void GameSession::RecvLobbyStartGame( SPacket &packet )
{
	SSynchronize Sync( this );

	int roomNum, gameMode, playTime, playCount, count;

	packet >> roomNum;		//���ȣ
	packet >> gameMode;		//���� ���
	packet >> playTime;		//�Ǵ� ���ӽð�
	packet >> playCount;	//���� �Ǽ�

	packet >> count;		//���� �ο���

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvLobbyStartGame()\n%d�� �濡 %d���� ĳ���Ͱ� ������ �����մϴ�.\n\n") 
						, roomNum, count );

	//�켱 �� ��ȣ�� �ش��ϴ� ������ ����
	GameProc* tmpGame = GetGameMgr.FindGame( roomNum );
	if( tmpGame == NULL )
	{
		//����proc�� ���� ���ų� �����ߴٴ� ��Ŷ�� ������
		SendStartFaild( roomNum );
		return;
	}
	if( tmpGame->NowIsPlay() )
	{
		//�̹� ������ ���� ���̸� �����ߴٴ� ��Ŷ�� ������.
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvLobbyStartGame()\n%d�� ���� �̹� ���� ���Դϴ�.\n\n") 
			, roomNum );

		SendStartFaild( roomNum );
		return;
	}

	//���� ��� �԰� ������ �������� �ƴ϶�� ������ ���� ĳ���͸� ������ �ش�

	int sessionId, size, team;
	TCHAR stringID[30];
	//�ο����� �°� ĳ���͸� ������ �ش�
	for( int i=0; i<count; ++i )
	{
		ZeroMemory( stringID, 30 );
		packet >> sessionId;
		packet >> size;
		packet.GetData( stringID, size );
		packet >> team;

		//ĳ���� ����
		CharObj* tmpChar = GetCharMgr.GetCharSpace();
		tmpChar->Init();
		tmpChar->SetIndexId( sessionId );
		tmpChar->SetID( stringID );
		tmpChar->SetTeam( team );
		//��ġ ����!!
		tmpChar->SetPosition( 10.f, 0.f, 10.f );
	}

	//======================================
	// ���� proc�� ���� ���� �غ�
	//======================================
	//������ ������ ������ �ش�.
	tmpGame->SetPlayerCount( count );
	tmpGame->SetGameMode( gameMode );
	tmpGame->SetGamePlayTime( playTime );
	tmpGame->SetGamePlayCount( playCount );

	//���� proc�� Ȱ��ȭ �Ѵ�
	tmpGame->StartGame();

	//������ �����ص� �ȴٴ� ��Ŷ�� Lobby�� ������
	SendStartOK( roomNum );
}

//--------------------------------------
// Client���� Ŀ�´����̼�
//--------------------------------------
void GameSession::RecvInGame( SPacket &packet )
{
	SSynchronize Sync( this );

	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	//test////////////////////////////////////////////////////////////////////
	//������ ������ 1�� ��....1���� ���� ����
	roomNum = 1;
	//////////////////////////////////////////////////////////////////////////

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::RecvInGame()\n%d�� ĳ���Ͱ� %d���������� �����մϴ�.\n\n") 
					, sessionId, roomNum );

	//���� ID�Է�
	SetSessionID( sessionId );

	//ĳ���͸� �˻�
	m_myCharInfo = GetCharMgr.FindCharAsSessionId( sessionId );
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}
	
	m_myGameProc = GetGameMgr.FindGame( roomNum );
	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n�ش� ���� ���μ����� ã�� ���߽��ϴ�.\n\n") );
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
	SSynchronize Sync( this );

	int state, dirInt;
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
	packet >> dirInt;

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvMoveChar()\nĳ���� ������ ������ �ֽ��ϴ�.\n\n") );
		return;
	}

	m_myCharInfo->SetState( state );
	m_myCharInfo->SetPosition( pos );
	m_myCharInfo->SetDirection( dir.m_X, dir.m_Y, dir.m_Z );
	m_myCharInfo->SetDirInt( dirInt );

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("GameSession::RecvMoveChar()\n")
					_T("%s(%d) ĳ���Ͱ� %d�� ����\n")
					_T("pos(%f, %f, %f)��ġ")
					_T("/ dir(%f, %f, %f, %i)�������� ��ȯ\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, state
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z, dirInt );

	SendMoveChar();
}

void GameSession::RecvGameAttec( SPacket &packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	int weapon, attectedSessionID, part;
	packet >> weapon;
	packet >> attectedSessionID;
	packet >> part;

	//�ǰ� ����� �޾� �´�
	CharObj* tmpChar = m_myGameProc->FindChar( attectedSessionID );
	if( tmpChar == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �ǰݴ���� ã�� �� �����ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}
	//���� ������ Ȯ���ϱ�
	if( m_myCharInfo->GetTeam() == tmpChar->GetTeam() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �ǰݴ���� ���� ���Դϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//����, ��Ʈ�� ���� �������� �޾� �´�

	//�������� �ް� �Ѵ�

	//�ǰݴ���� �Ǹ� Ȯ���Ѵ�

	//�ǰ���Ŷ�� �����ų� die��Ŷ�� ������

	//�׾����� �׿� ���� killcount�� ����(?)�� ó���� �� �ش�.
}

void GameSession::RecvGameSync( SPacket &packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvMoveChar()\nĳ���� ������ ������ �ֽ��ϴ�.\n\n") );
		return;
	}

	POINT3 pos, dir;
	//��ġ
	packet >> pos.m_X;
	packet >> pos.m_Y;
	packet >> pos.m_Z;
	//����
	packet >> dir.m_X;
	packet >> dir.m_Y;
	packet >> dir.m_Z;

	m_myCharInfo->SetPosition( pos );
	m_myCharInfo->SetDirection( dir );

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("GameSession::RecvGameSync()\n")
					_T("%s(%d) ĳ����\n")
					_T("pos(%f, %f, %f)��ġ")
					_T("/ dir(%f, %f, %f)�������� ��ȯ\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z );

	SendGameSync();
}

void GameSession::RecvGameChangeState( SPacket &packet )
{
	SSynchronize Sync( this );

	int state;
	packet >> state;

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChangeDir()\n")
						_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_myCharInfo->SetState( state );

	SendGameChangeState();
}

void GameSession::RecvGameRotation( SPacket &packet )
{
	SSynchronize Sync( this );

	POINT3 dir;
	packet >> dir.m_X >> dir.m_Y >> dir.m_Z;

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameRotation()\n")
						_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_myCharInfo->SetDirection( dir );

	SendGameRotation();
}

void GameSession::RecvGameChatting( SPacket &packet )
{
	SSynchronize Sync( this );

	int size;
	TCHAR tmpChatting[256] = {0,};
	TCHAR Chatting[300] = {0,};

	packet >> size;
	packet.GetData( tmpChatting, size );

	
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\nĳ������ ������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return;
	}

	swprintf_s( Chatting, _T("[%s] %s"), m_myCharInfo->GetID(), tmpChatting );
	size = _tcslen( Chatting )*sizeof( TCHAR );

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );

	SendGameChatting( Chatting, size );
}

//======================================
// ������ ��Ŷ ó��
//======================================
//--------------------------------------
// Lobby Srv���� Ŀ�´����̼�
//--------------------------------------

BOOL GameSession::SendStartFaild( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_FAILD );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendStartOK( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_OK );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameEnd( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_GAME_END );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendToSrvThatPlayerDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( GL_PLAYER_DISCONNECT );
	sendPacket << m_myGameProc->GetGameID();	//���ӹ�ȣ(���ȣ)�� �ְ�
	sendPacket << m_myCharInfo->GetIndexId();	//sessionId�� �ְ�

	//�κ񼭹��� ĳ���Ͱ� ������ �����ϰ� �����ٴ� ���� �˸���.
	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

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
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMyCharInfoToInGamePlayer()\n���۵� ��Ŷũ��� ��Ŷ�� ũ�� ��ġ ���� �ʽ��ϴ�.\n\n") );
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
	m_myCharInfo->PackageMyInfo( sendPacket );
	//PackageMyInfo( sendPacket );

	//�� ���� �� ������
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendMoveChar()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_MOVE_CHAR );

	POINT3 pos = m_myCharInfo->GetPosition();
	POINT3 dir = m_myCharInfo->GetDirection();

	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << m_myCharInfo->GetState();
	sendPacket << pos.m_X << pos.m_Y << pos.m_Z;
	sendPacket << dir.m_X << dir.m_Y << dir.m_Z;
	sendPacket << m_myCharInfo->GetDirInt();

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("GameSession::SendMoveChar()\n")
					_T("%s(%d) ĳ���Ͱ� %d�� ����\n")
					_T("pos(%f, %f, %f)��ġ")
					_T("/ dir(%f, %f, %f, %i)�������� ��ȯ\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, m_myCharInfo->GetState()
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z, m_myCharInfo->GetDirInt() );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMoveChar()\n�������� ������ �ֽ��ϴ�.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameAttec( int weapon, int attectedSessionID, int part, int damage )
{
	return TRUE;	
}

BOOL GameSession::SendGameSync()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_SYNC );

	const POINT3 pos = m_myCharInfo->GetPosition();
	const POINT3 dir = m_myCharInfo->GetDirection();
	

	sendPacket << m_myCharInfo->GetIndexId();
	//sendPacket << m_myCharInfo->GetState();
	sendPacket << pos.m_X;
	sendPacket << pos.m_Y;
	sendPacket << pos.m_Z;

	sendPacket << dir.m_X;
	sendPacket << dir.m_Y;
	sendPacket << dir.m_Z;
	//sendPacket << m_myCharInfo->GetDirInt();

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("GameSession::SendGameSync()\n")
					_T("%s(%d) ĳ����\n")
					_T("pos(%f, %f, %f)��ġ\n")
					_T("dir(%f, %f, %f)�������� ��ȯ\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMoveChar()\n�������� ������ �ֽ��ϴ�.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChatting( TCHAR* chatting, int size )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << size;
	sendPacket.PutData( chatting, size );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n�� ���� proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameChangeState()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHANGE_STATE );
	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << m_myCharInfo->GetState();

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChangeDir()\n")
						_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameRotation()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_ROTATION );

	POINT3 dir = m_myCharInfo->GetDirection();

	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << dir.m_X << dir.m_Y << dir.m_Z;

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameRotation()\n")
						_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

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
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendCharDisconnect()\n")
			_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	//���� �̹� ���� ������ ��ο��� ������ �ȴ�.
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
