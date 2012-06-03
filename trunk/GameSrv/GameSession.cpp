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

SrvNet*		GameSession::m_srvNet	= &GetSrvNet;
SLogger*	GameSession::m_logger	= &GetLogger;
GameMgr*	GameSession::m_gameMgr	= &GetGameMgr;
CharMgr*	GameSession::m_charMgr	= &GetCharMgr;
ItemMgr*	GameSession::m_itemMgr	= &GetItemMgr;

GameSession::GameSession(void)
: m_myCharInfo(NULL)
, m_myGameProc(NULL)
, isEndGame(TRUE)
{
// 	m_srvNet	= &GetSrvNet;
// 	m_logger	= &GetLogger;
// 	m_gameMgr	= &GetGameMgr;
// 	m_charMgr	= &GetCharMgr;
// 	m_itemMgr	= &GetItemMgr;
}

GameSession::~GameSession(void)
{
}

void GameSession::Clear()
{
	SSynchronize sync( this );

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
		if( m_srvNet->GetSession() == this )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
							_T("GameSession::OnDestroy()\n �������� ������ �����ϴ�.\n\n") );
		}
		else
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
							_T("GameSession::OnDestroy()\n ĳ�������� ���� ������ ������ �����մϴ�.\n\n") );
		}
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

			//���� proc����
			m_myGameProc->Init();

			//�κ�� ���� ���� proc�� �������� �˸�?
			SendGameEnd( m_myGameProc->GetGameID() );
		}
		else
		{
			//���� ���� �ִ� ���� �����ϰ� �ִ� ����鿡�� ���� �����°��� �˸�
			SendCharDisconnect();
		}

		//ĳ���� ������ ������ ���� �ش�.
		m_charMgr->ReturnCharSpace( m_myCharInfo );
	}

	//���� �ʱ�ȭ
	Clear();

	SSession::OnDestroy();
}

void GameSession::PackageMyNetInfo( SPacket& packet )
{
	SSynchronize sync( this );

	packet << m_myCharInfo->GetSessionID();

	int size = strlen( m_IPAddr );
	packet << size;
	packet.PutData( m_IPAddr, size );
	packet << m_myGameProc->GetPort();
}

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
	case CS_GAME_GAME_READY_OK:
		RecvGameReadyOK();
		break;
	case CS_GAME_ATTACK:
		RecvGameAttack( packet );
		break;
	case CS_GAME_TRY_ATTACK:
		RecvGameTryAttack( packet );
		break;
	case CS_GAME_JUST_SHOOT:
		//
		break;
	case CS_GAME_CHANGE_STATE:
		RecvGameChangeState( packet );
		break;
	case CS_GAME_ASK_REVIVAL:
		RecvGameAskRevival( packet );
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
	case CS_GAME_RADIO_PLAY:
		RecvGameRadioPlay( packet );
		break;
	case CS_GAME_INSTALL_ITEM:
		//
		break;
// 	case CS_GAME_GOTO_LOBBY:
// 		//
// 		break;

	//==============================================================> Client
	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::PacketParsing()\n(%d�� ID)���� ��Ŷ�� ���̵� ��ȿ���� �ʽ��ϴ�.\n\n"), packet.GetID() );
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
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvLobbyConnectOK()\n�κ񼭹��� ���ῡ �����Ͽ����ϴ�\n\n") );

	m_srvNet->SetSession( this );
}

void GameSession::RecvLobbyStartGame( SPacket &packet )
{
	SSynchronize Sync( this );

	int roomNum, mapNum, gameMode, playTime, playCount, count;

	packet >> roomNum;		//���ȣ
	packet >> mapNum;
	packet >> gameMode;		//���� ���
	packet >> playTime;		//�Ǵ� ���ӽð�
	packet >> playCount;	//���� �Ǽ�

	packet >> count;		//���� �ο���

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvLobbyStartGame()\n%d�� �濡 %d���� ĳ���Ͱ� ������ �����մϴ�.\n\n") 
						, roomNum, count );

	//�켱 �� ��ȣ�� �ش��ϴ� ������ ����
	GameProc* tmpGame = m_gameMgr->FindGame( roomNum );
	if( tmpGame == NULL )
	{
		//����proc�� ���� ���ų� �����ߴٴ� ��Ŷ�� ������
		SendStartFaild( roomNum );
		return;
	}
	if( tmpGame->NowIsPlay() )
	{
		//�̹� ������ ���� ���̸� �����ߴٴ� ��Ŷ�� ������.
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
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
		CharObj* tmpChar = m_charMgr->GetCharSpace();
		if( tmpChar == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("GameSession::RecvLobbyStartGame()\nĳ���� ������ �����ϴ�.\n\n") );
			SendStartFaild( roomNum );
			return;
		}
		tmpChar->Init();
		tmpChar->SetSessionID( sessionId );
		tmpChar->SetID( stringID );
		tmpChar->SetTeam( team );
	}

	//======================================
	// ���� proc�� ���� ���� �غ�
	//======================================
	//������ ������ ������ �ش�.
	tmpGame->SetGameStage( mapNum );
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

	if( roomNum == 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvInGame()\n%d�� ĳ���� �ӽ� ĳ���� ������ ����ϴ�.\n\n") 
						, sessionId );

		//�ӽ� ĳ���� ������ �����
		CharObj* tmpChar = m_charMgr->GetCharSpace();
		tmpChar->SetSessionID( sessionId );
		tmpChar->SetID( _T("Unknown") );
		tmpChar->SetTeam( (int)(sessionId % 2) );
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::RecvInGame()\n%d�� ĳ���Ͱ� %d���������� �����մϴ�.\n\n") 
					, sessionId, roomNum );

	//���� ID�Է�
	SetSessionID( sessionId );

	//ĳ���͸� �˻�
	m_myCharInfo = m_charMgr->FindCharAsSessionId( sessionId );
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}
	m_myCharInfo->SetSession( this );
	
	m_myGameProc = m_gameMgr->FindGame( roomNum );
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvInGame()\n�ش� ���� ���μ����� ã�� ���߽��ϴ�.\n\n") );
		return;
	}
	//�濡 ���� ���
	m_myGameProc->AddPlayer( this );

	//���� ������� ������ �����Ժ���( �� ������ �ʿ� ���� )
	SendOtherCharInfoToMe();

	//���� ����鿡�� �� ������ ����
	SendMyCharInfoToInGamePlayer();
}

void GameSession::RecvGameReadyOK()
{
	SSynchronize sync( this );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameReadyOK()\n�ش� ���� ���μ����� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	//ready count�� �ϳ� �÷� �ش�.
	m_myGameProc->AddReadyCount();
}

void GameSession::RecvGameAttack( SPacket &packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	if( !m_myGameProc->NowIsPlay() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\n������ Ȱ��ȭ �Ǿ� ���� �ʽ��ϴ�.\n\n") );
		return;
	}

	int attectedSessionID, damage;
	float posX, posY, posZ, normalX, normalY, normalZ;
	BOOL isHead;
	packet >> isHead;
	packet >> attectedSessionID;
	packet >> damage;

	packet >> posX >> posY >> posZ;
	packet >> normalX >> normalY >> normalZ;

	//�ǰ� ����� �޾� �´�
	CharObj* tmpChar = m_myGameProc->FindChar( attectedSessionID );
	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �ǰݴ���� ã�� �� �����ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}
	//���� ���������� Ȯ���ϱ�
	if( m_myCharInfo->IsDie() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �̹� �׾����ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//��� ������ Ȯ���ϱ�
	if( m_myCharInfo->GetTeam() == tmpChar->GetTeam() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �ǰݴ���� ���� ���Դϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//�������� �ް� �Ѵ�
	tmpChar->DownHP( damage );

	//attect��Ŷ
	//��ο���
	SendGameAttack( isHead, tmpChar, posX, posY, posZ, normalX, normalY, normalZ );

	//���� �𿡰�
	SendGameYouAttack( isHead, tmpChar );

	//�ǰݴ���� �Ǹ� Ȯ���Ѵ�
	if( tmpChar->IsDie() )
	{
		//�� �׾����� ���� killcount�� ����
		m_myGameProc->AddKillCount( tmpChar->GetTeam() );
		//ĳ���� killcount����
		m_myCharInfo->KillCountUp();
		//ĳ���� deathcount����
		tmpChar->DeathCountUp();

		//die��Ŷ
		SendGameDie( isHead, tmpChar );
	}
}

void GameSession::RecvGameTryAttack( SPacket &packet )
{
	//ID�� �ٲ� �ش�
	packet.SetID( SC_GAME_TRY_ATTACK );

	SendGameTryAttact( packet );
}

void GameSession::RecvGameJustShoot()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameJustShoot()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	SPacket sendPacket( SC_GAME_JUST_SHOOT );

	sendPacket << m_myCharInfo->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );
}

void GameSession::RecvGameChangeState( SPacket &packet )
{
	SSynchronize Sync( this );

	int state;
	packet >> state;

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChangeDir()\n")
						_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	SendGameChangeState( state );
}

void GameSession::RecvGameAskRevival( SPacket &packet )
{
	SSynchronize sync( this );

	//�켱 �ָ� ��Ȱ���� �ش�.
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameAskRevival()\n")
						_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_myCharInfo->SetAlive();

	int startPoingIndex;

	packet >> startPoingIndex;

	SendGameCharRevival( startPoingIndex );
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\nĳ������ ������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return;
	}

	swprintf_s( Chatting, _T("[%s] %s"), m_myCharInfo->GetID(), tmpChatting );
	size = _tcslen( Chatting )*sizeof( TCHAR );

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );

	SendGameChatting( Chatting, size );
}

void GameSession::RecvGameRadioPlay( SPacket &packet )
{
	packet.SetID( SC_GAME_RADIO_PLAY );

	SendGameRadioPlay( packet );
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
	sendPacket << m_myCharInfo->GetSessionID();	//sessionId�� �ְ�	
	sendPacket << m_myCharInfo->GetTeam();

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

	int count = m_myGameProc->GetConnectPlayerCount();

	//���� ������ �Ⱥ����� �ȴ�.
	if( count <= 1 )
		return TRUE;

	//���ӳ��� ����� ������ ��´�.
	sendPacket << count-1;
	m_myGameProc->PackageAllPlayerInGame( sendPacket, this );

	int retval = SendPacket( sendPacket );
	if( retval != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::SendMyCharInfoToInGamePlayer()\n���۵� ��Ŷũ��� ��Ŷ�� ũ�� ��ġ ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL GameSession::SendMyCharInfoToInGamePlayer()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;
	
	//�濡 ���� ������ �Ⱥ����� �ȴ�
	if( m_myGameProc->GetConnectPlayerCount() <= 1 )
		return TRUE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//�� ������ ������
	sendPacket << (int)1;
	PackageMyNetInfo( sendPacket );

	//�� ���� �� ������
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameAttack( BOOL isHead, CharObj* attactedChar, float posX, float posY, float posZ, float normalX, float normalY, float normalZ )
{
	SPacket sendPacket( SC_GAME_ATTACK );
	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << attactedChar->GetSessionID();

	sendPacket << posX << posY << posZ;
	sendPacket << normalX << normalY << normalZ;

	m_myGameProc->SendAllPlayerInGame( sendPacket, attactedChar->GetSession() );

	return TRUE;
}

BOOL GameSession::SendGameYouAttack( BOOL isHead, CharObj* attactedChar )
{
	SPacket sendPacket( SC_GAME_YOU_ATTACKED );
	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << attactedChar->GetHP();		//���� HP

	attactedChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameTryAttact( SPacket& packet )
{
	m_myGameProc->SendAllPlayerInGame( packet, this );

	return TRUE;
}

BOOL GameSession::SendGameDie( BOOL isHead, CharObj* dieChar )
{
	//======================================
	// ���鿡�� ����
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << dieChar->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//======================================
	// ������ ����
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << isHead;
	sendPacket << m_myCharInfo->GetSessionID();
	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameChangeState( int state )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHANGE_STATE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << state;

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChangeDir()\n")
						_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameCharRevival( int pointIndex )
{
	SPacket sendPacket( SC_GAME_CHAR_REVIVAL );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << pointIndex;

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameCharRevival()\n")
						_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n�� ���� proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameRadioPlay( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendGameRadioPlay()\nĳ���� ������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendGameRadioPlay()\nĳ���� %s�� ���� proc������ ��ȿ���� �ʽ��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return FALSE;
	}

	//�ڽ��� ������ ������
	m_myGameProc->SendPacketToMyTeam( m_myCharInfo->GetTeam(), packet, this );

	return TRUE;
}

BOOL GameSession::SendCharDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHAR_DISCONNECT );

	//�ڽ��� ���� ID�� �־� �ش�
	sendPacket << m_myCharInfo->GetSessionID();

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendCharDisconnect()\n")
						_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	//���� �̹� ���� ������ ��ο��� ������ �ȴ�.
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
