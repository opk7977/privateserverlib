#include "GameSession.h"
#include "GameProtocol.h"

#include "SrvNet.h"
#include "SLogger.h"
#include "CharMgr.h"
#include "ItemMgr.h"
#include "GameMgr.h"
#include "DataLeader.h"

#include "DBSrvMgr.h"

#include "GameProc.h"
#include "MineItem.h"

SIMPLEMENT_DYNAMIC(GameSession)
SIMPLEMENT_DYNCREATE(GameSession)

SrvNet*		GameSession::m_srvNet	= &GetSrvNet;
SLogger*	GameSession::m_logger	= &GetLogger;
GameMgr*	GameSession::m_gameMgr	= &GetGameMgr;
CharMgr*	GameSession::m_charMgr	= &GetCharMgr;
ItemMgr*	GameSession::m_itemMgr	= &GetItemMgr;
DataLeader*	GameSession::m_document	= &GetDocument;

DBSrvMgr*	GameSession::m_dbMgr	= &GetDBSrv;

GameSession::GameSession(void)
: m_myCharInfo(NULL)
, m_myGameProc(NULL)
, isEndGame(TRUE)
{
}

GameSession::~GameSession(void)
{
}

void GameSession::Clear()
{
	SSynchronize sync( this );

	m_myCharInfo	= NULL;
	m_myGameProc	= NULL;
	isEndGame		= TRUE;
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
	//SSynchronize sync( this );

	packet << m_myCharInfo->GetSessionID();

	int size = strlen( m_IPAddr );
	packet << size;
	packet.PutData( m_IPAddr, size );
	packet << m_myGameProc->GetPort();
}

void GameSession::PacketParsing( SPacket& packet )
{
	SSynchronize Sync( this );

	switch( packet.GetID() )
	{
	case DB_TO_OTHER_CONNECT_OK:
		RecvDBConnectOK();
		break;
	case DB_TO_OTHER_DROP_PLAYER:
		RecvCharDrop( packet );
		break;
	//==============================================================> DBSrv
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case LG_START_GAME:
		RecvLobbyStartGame( packet );
		break;
	case LG_END_GAME_READY_OK:
		RecvLobbyEndReadyOK( packet );
		break;
	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	case CS_GAME_GAME_READY_OK:
		RecvGameReadyOK();
		break;
	case CS_GAME_CHARACTER_SYNC:
		RecvGameCharacterSync( packet );
		break;
	case CS_GAME_CHARACTER_JUMP:
		RecvGameCharacterJump();
		break;
	case CS_GAME_CHARACTER_LAND:
		RecvGameCharacterLand( packet );
		break;
	case CS_GAME_CHAR_CHANGE_OBJ:
		RecvGameCharChangeObj( packet );
		break;
	case CS_GAME_ATTACK:
		RecvGameAttack( packet );
		break;
	case CS_GAME_TRY_ATTACK:
		RecvGameTryAttack( packet );
		break;
	case CS_GAME_JUST_SHOOT:
		RecvGameJustShoot();
		break;
	case CS_GAME_LAY_MINE:
		RecvGameLayMine( packet );
		break;
	case CS_GAME_MINE_HIT:
		RecvGameMineHit( packet );
		break;
	case CS_GAME_WEAPON_CHANGE:
		RecvGameChangeWeapon( packet );
		break;
	case CS_GAME_VISIBLE_HIDE:
		RecvGameVisibleHide();
		break;
	case CS_GAME_INVISIBLE_HIDE:
		RecvGameInvisibleHide();
		break;
	case CS_GAME_SKILL_SCAN_ON:
		RecvGameSkillScanOn();
		break;
	case CS_GAME_SKILL_SCAN_OFF:
		RecvGameSkillScanOff();
		break;
	case CS_GAME_KILL_MYSELF:
		RecvGameKillMyself();
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
	case CS_GAME_RADIO_PLAY:
		RecvGameRadioPlay( packet );
		break;
	case CS_GAME_GOTO_LOBBY:
		RecvGameGotoLobby();
		break;
// 	case CS_GAME_END_GUNSELECT:
// 		RecvGameEndGunSelect();
// 		break;
// 	case CS_GAME_START_TIME_COUNTDOWN:
// 		RecvGameStartCountDown();
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
// DB �������� Ŀ�´����̼�
//--------------------------------------
void GameSession::RecvDBConnectOK()
{
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvDBConnectOK()\nDB������ ���ῡ �����Ͽ����ϴ�\n\n") );

	m_dbMgr->SetSession( this );
}

void GameSession::RecvCharDrop( SPacket &packet )
{
	int sessionId;
	packet >> sessionId;

	CharObj* tmpChar = m_charMgr->FindCharAsSessionId( sessionId );
	//ĳ���͸� ã�� �� ���ٸ� �װ� �κ� �ִ� ĳ���ʹϱ� �Ű澲�� ����
	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvCharDrop()\n") );
		return;
	}

	//�ƴϸ� ������ ������� ����� ������.
	tmpChar->GetSession()->SendGameSelfDisconnect();
}

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
	//SSynchronize Sync( this );

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

	{
		SSynchronize sync( tmpGame );
		
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
		int rankId;
		TCHAR stringID[30];
		//�ο����� �°� ĳ���͸� ������ �ش�
		for( int i=0; i<count; ++i )
		{
			ZeroMemory( stringID, 30 );
			packet >> sessionId;
			packet >> size;
			packet.GetData( stringID, size );
			packet >> team;

			packet >> rankId;

			//ĳ���� ����
			CharObj* tmpChar = m_charMgr->GetCharSpace();
			if( tmpChar == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("GameSession::RecvLobbyStartGame()\nĳ���� ������ �����ϴ�.\n\n") );
				SendStartFaild( roomNum );
				return;
			}
			//ĳ���� ����
			tmpChar->Init();
			tmpChar->SetSessionID( sessionId );
			tmpChar->SetID( stringID );
			tmpChar->SetTeam( team );
			tmpChar->SetRankID( rankId );
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
	}

	//������ �����ص� �ȴٴ� ��Ŷ�� Lobby�� ������
	SendStartOK( roomNum );
}

void GameSession::RecvLobbyEndReadyOK( SPacket &packet )
{
	int roomNum;
	packet >> roomNum;

	GameProc* tmpGame = m_gameMgr->FindGame( roomNum );
	if( tmpGame == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvLobbyEndReadyOK()\n%d�� ���� �����ϴ�.\n\n"), roomNum );
		return;
	}

	tmpGame->RecvLobbyEndReadyOK();
}

//--------------------------------------
// Client���� Ŀ�´����̼�
//--------------------------------------
void GameSession::RecvInGame( SPacket &packet )
{
	//SSynchronize Sync( this );

	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	if( roomNum == 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvInGame()\n0�� ���� �������� �ʽ��ϴ�.\n\n") );
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
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameReadyOK()\n�ش� ���� ���μ����� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	//ready count�� �ϳ� �÷� �ش�.
	m_myGameProc->AddReadyCount();
}

void GameSession::RecvGameCharacterSync( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("GameSession::RecvGameCharacterSync()\n�ش� ĳ���� ������ �����ϴ�.\n\n") );
		return;
	}

	//ĳ���� ������ ����
	float posX, posY, posZ;

	packet >> posX >> posY >> posZ;

	//��ġ���� ����
	m_myCharInfo->SetPos( posX, posY, posZ );
}

void GameSession::RecvGameCharacterJump()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterJump()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterJump()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	SendGameCharacterJump();
}

void GameSession::RecvGameCharacterLand( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	int objIndex, charState;

	packet >> objIndex;
	packet >> charState;

	SendGameCharacterLand( objIndex, charState );
}

void GameSession::RecvGameCharChangeObj( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\n�ش� ĳ���͸� ã�� ���߽��ϴ�.\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameCharacterLand()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	int objIndex;
	packet >> objIndex;

	SendGameCharChangeObj( objIndex );
}

void GameSession::RecvGameAttack( SPacket &packet )
{
	//SSynchronize Sync( this );

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

	int attectedSessionID, shoutData;
	int dataHead, dataWeapon, dataDamage;
	float posX, posY, posZ, normalX, normalY, normalZ;

	packet >> attectedSessionID;
	packet >> shoutData;
	dataHead	= ( ( shoutData & 0xff000000 ) >> 24 );
	dataWeapon	= ( ( shoutData & 0x00ff0000 ) >> 16 );
	dataDamage	= ( shoutData & 0x0000ffff );

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

	//���� �ְ� ���������� Ȯ���ϱ�
	if( tmpChar->IsDie() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �̹� �׾����ϴ�.\n\n"), tmpChar->GetID() );
		return;
	}

	//���� �ְ� �������� Ȯ���ϱ�
	if( tmpChar->IsInvincible() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttec()\nĳ����%s�� �������� �Դϴ�.\n\n"), tmpChar->GetID() );
		return;
	}

// 	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("���� : %d\n\n"), GetTickCount() );

	//�������� �ް� �Ѵ�
	//������ death�� ������
	tmpChar->DownHP( dataDamage );

	//attect��Ŷ
	//��ο���
	SendGameAttack( tmpChar, posX, posY, posZ, normalX, normalY, normalZ );

	//���� �𿡰�
	SendGameYouAttack( tmpChar );

	//�ǰݴ���� �Ǹ� Ȯ���Ѵ�
	if( tmpChar->IsDie() )
	{
		//�� �׾����� ���� killcount�� ����
		m_myGameProc->AddKillCount( tmpChar->GetTeam() );
		//ĳ���� killcount����
		m_myCharInfo->KillCountUp();

		//ĳ���Ͱ� ��ġ�� ���ڰ� ������ �����ְ� ���� ��ġ�� �� �ְ� �Ѵ�
		m_myGameProc->MineResetTarget( tmpChar->GetSessionID() );

		//die��Ŷ
		SendGameDie( dataHead, dataWeapon, tmpChar );
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

void GameSession::RecvGameLayMine( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameLayMine()\nĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameLayMine()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	float posX, posY, posZ;
	float dirX, dirY, dirZ;

	packet >> posX >> posY >> posZ;
	packet >> dirX >> dirY >> dirZ;

	m_myGameProc->SettingMine( this, posX, posY, posZ, dirX, dirY, dirZ );
}

void GameSession::RecvGameMineHit( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameMineHit()\nĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameMineHit()\nĳ���� %s�� ���� ���μ����� ã�� ���߽��ϴ�.\n\n"), m_myCharInfo->GetID() );
		return;
	}

	//������ id�� �ް�
	int mineId;
	packet >> mineId;

	MineItem* tmpMine = m_myGameProc->FindMine( mineId );
	if( tmpMine == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameMineHit()\n�� ���ڰ� �������� �ʽ��ϴ�.\n\n") );
		return;
	}

	//��ġ�� �������� üũ
	if( !tmpMine->IsInstall() )
		return;

	//��������
	tmpMine->Boom();
}

void GameSession::RecvGameChangeWeapon( SPacket &packet )
{
	int weapon;
	
	packet >> weapon;

	//����

	SendGameChangeWeapon( weapon );
}

void GameSession::RecvGameVisibleHide()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameVisibleHide()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameVisibleHide()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("%s���� ���� ���\n\n"), m_myCharInfo->GetID() );
	
// 	//�̳��� ���� NONE���°� �ƴϸ� �ȵ�
// 	if( m_myCharInfo->GetSkillState() != SKILL_NONE )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("GameSession::RecvGameVisibleHide()\n")
// 			_T("�� ���� �⺻ ���°� �ƴ�\n\n") );
// 		return;
// 	}

	//�� ���� ����
	m_myCharInfo->SetSkillHide();

	GameCharVisibleHide();
}

void GameSession::RecvGameInvisibleHide()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameInvisibleHide()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameInvisibleHide()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("%s���� ���� ���\n\n"), m_myCharInfo->GetID() );

// 	//���� ���°� �ƴϸ� ������
// 	if( m_myCharInfo->GetSkillState() != SKILL_HIDE )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("GameSession::RecvGameVisibleHide()\n")
// 			_T("�� ���� ���� ���°� �ƴ�\n\n") );
// 		return;
// 	}

	//�� ���� ���� ����
	m_myCharInfo->SetSkillNone();

	GameCharInvisibleHide();
}

void GameSession::RecvGameSkillScanOn()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameSkillScanOn()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	//�� ���� ����
	m_myCharInfo->SetSkillScan();
}

void GameSession::RecvGameSkillScanOff()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameSkillScanOff()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	//�� ���� ����
	m_myCharInfo->SetSkillNone();
}

void GameSession::RecvGameKillMyself()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameKillMyself()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameKillMyself()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_myCharInfo->DownHP( 100 );

	SendGameDie();
}

void GameSession::RecvGameChangeState( SPacket &packet )
{
	//SSynchronize Sync( this );

	int state, objIndex;
	BOOL isJump;
	packet >> state;
	packet >> isJump;
	packet >> objIndex;

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameChangeDir()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	SendGameChangeState( state, isJump, objIndex );
}

void GameSession::RecvGameAskRevival( SPacket &packet )
{
	//SSynchronize sync( this );

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
	//SSynchronize Sync( this );

	int target, size;
	TCHAR tmpChatting[256] = {0,};
	//TCHAR Chatting[300] = {0,};

	packet >> target;
	packet >> size;
	//256�̻��� �߶� ����
	if( size > 256 )
		size = 256;

	packet.GetData( tmpChatting, size );

	
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\nĳ������ ������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return;
	}

	switch( target )
	{
	case CHATTING_ALL:
		//swprintf_s( Chatting, _T("[All][%s] %s"), m_myCharInfo->GetID(), tmpChatting );
		SendGameChatting( tmpChatting );
		break;
	case CHATTING_TEAM:
		//swprintf_s( Chatting, _T("[Team][%s] %s"), m_myCharInfo->GetID(), tmpChatting );
		SendGameTeamChat( tmpChatting );
		break;
	default:
		//�̻��� �����ͳ�?
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\ntarget�� ��ȿ���� �ʽ��ϴ�.\n\n") );
		return;
	}

// 	size = _tcslen( Chatting )*sizeof( TCHAR );
// 
// 	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
// 		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );
// 
// 	SendGameChatting( Chatting, size );
}

void GameSession::RecvGameRadioPlay( SPacket &packet )
{
	int index;
	packet >> index;
	//packet.SetID( SC_GAME_RADIO_PLAY );

	//SendGameRadioPlay( packet );
	SendGameRadioPlay( index );
}

void GameSession::RecvGameAttackTower( SPacket &packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttackTower()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameAttackTower()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	int damege;
	packet >> damege;

	m_myGameProc->TowerDameged( m_myCharInfo->GetTeam(), damege );
}

void GameSession::RecvGameGotoLobby()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameGotoLobby()\n")
			_T("ĳ���� ������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameGotoLobby()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	//�κ�� �˸�
	SendGameToLobbyCharGotoLobby();

	//�κ�� �ű�ٴ� ǥ�ø� ��
	SetGotoLobby();

	//�κ�� ����� �˸�
	SendGameGotoLobby();
}

//test
void GameSession::RecvGameEndGunSelect()
{
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameEndGunSelect()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_myGameProc->SetIsSelectGun();
}

//test
void GameSession::RecvGameStartCountDown()
{
	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::RecvGameStartCountDown()\n")
			_T("���� proc������ ��ȿ���� �ʽ��ϴ�\n\n") );
		return;
	}

	m_myGameProc->SetIsStartGame();
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

	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendStartOK( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_OK );
	sendPacket << roomNum;

	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameEnd( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_GAME_END );
	sendPacket << roomNum;

	m_srvNet->SendToLobbyServer( sendPacket );

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

BOOL GameSession::SendGameCharacterJump()
{
	SPacket sendPacket( SC_GAME_CHARACTER_JUMP );

	sendPacket << m_myCharInfo->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameCharacterLand( int objIndex, int charState )
{
	SPacket sendPacket( SC_GAME_CHARACTER_LAND );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << objIndex;
	sendPacket << charState;

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameCharChangeObj( int objIndex )
{
	SPacket sendPacket( SC_GAME_CHAR_CHANGE_OBJ );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << objIndex;

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameAttack( CharObj* attactedChar, float posX, float posY, float posZ, float normalX, float normalY, float normalZ )
{
	SPacket sendPacket( SC_GAME_ATTACK );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << attactedChar->GetSessionID();

	sendPacket << posX << posY << posZ;
	sendPacket << normalX << normalY << normalZ;

	m_myGameProc->SendAllPlayerInGame( sendPacket, attactedChar->GetSession() );

// 	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("���� : %d\n\n"), GetTickCount() );


	return TRUE;
}

BOOL GameSession::SendGameYouAttack( CharObj* attactedChar )
{
	SPacket sendPacket( SC_GAME_YOU_ATTACKED );
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

BOOL GameSession::SendGameDie( BOOL isHead, int weaponType, CharObj* dieChar )
{
	int shoutData = 0;
	shoutData |= ( isHead << 16 );
	shoutData |= weaponType;
	//======================================
	// ���鿡�� ����
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << dieChar->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, dieChar->GetSession() );

	//======================================
	// ������ ����
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	dieChar->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameDie()
{
	//��弦�� �ƴϰ� ����� 8��
	int shoutData = 0;
	shoutData |= 8;

	//======================================
	// ���鿡�� ����
	//======================================
	SPacket sendPacket( SC_GAME_CHAR_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetSessionID();

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	//======================================
	// ������ ����
	//======================================
	sendPacket.PacketClear();
	sendPacket.SetID( SC_GAME_YOU_DIE );

	sendPacket << shoutData;
	sendPacket << m_myCharInfo->GetSessionID();
	SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameLayMine( float posX, float posY, float posZ, float dirX, float dirY, float dirZ )
{
	SPacket sendPacket( SC_GAME_LAY_MINE );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetTeam();
	sendPacket << posX << posY << posZ;
	sendPacket << dirX << dirY << dirZ;

	//======================================
	// �Բ� �����ϴ� ��ο��� ������
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket );
	return TRUE;
}

BOOL GameSession::SendGameChangeWeapon( int weapon )
{
	SPacket sendPacket( SC_GAME_WEAPON_CHANGE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << weapon;

	//======================================
	// �Բ� �����ϴ� ��ο��� ������( ���� ���� )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::GameCharVisibleHide()
{
	SPacket sendPacket( SC_GAME_CHAR_VISIBLE_HIDE );

	sendPacket << m_myCharInfo->GetSessionID();

	//======================================
	// �Բ� �����ϴ� ��ο��� ������( ���� ���� )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::GameCharInvisibleHide()
{
	SPacket sendPacket( SC_GAME_CHAR_INVISIBLE_HIDE );

	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetHidePoint();

	//======================================
	// �Բ� �����ϴ� ��ο��� ������( ���� ���� )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChangeState( int state, BOOL isJump, int objIndex )
{
	SPacket sendPacket( SC_GAME_CHANGE_STATE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << state;
	sendPacket << isJump;
	sendPacket << objIndex;

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChangeDir()\n")
						_T("����proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	//======================================
	// �Բ� �����ϴ� ��ο��� ������( ���� ���� )
	//======================================
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

	//======================================
	// �Բ� �����ϴ� ��ο��� ������( ���� ���� )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChatting( TCHAR* chatting )
{
	//ä�� ���� �����
	TCHAR ChattingMsg[300] = {0,};
	swprintf_s( ChattingMsg, _T("[All][%s] %s"), m_myCharInfo->GetID(), chatting );
	int size = _tcslen( ChattingMsg )*sizeof( TCHAR );

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << (int)CHATTING_ALL;
	sendPacket << size;
	sendPacket.PutData( ChattingMsg, size );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n�� ���� proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	//======================================
	// �Բ� �����ϴ� ��ο��� ������
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameTeamChat( TCHAR* chatting )
{
	//ä�� ���� �����
	TCHAR ChattingMsg[300] = {0,};
	swprintf_s( ChattingMsg, _T("[Team][%s] %s"), m_myCharInfo->GetID(), chatting );
	int size = _tcslen( ChattingMsg )*sizeof( TCHAR );

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << (int)CHATTING_TEAM;
	sendPacket << size;
	sendPacket.PutData( ChattingMsg, size );

	if( m_myGameProc == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("GameSession::SendGameChatting()\n�� ���� proc������ ��ȿ���� �ʽ��ϴ�.\n\n") );
		return FALSE;
	}

	//======================================
	// �� �����Ը� ������
	//======================================
	m_myGameProc->SendPacketToMyTeam( m_myCharInfo->GetTeam(), sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameRadioPlay( int index )
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
	SPacket sendPacket( SC_GAME_RADIO_PLAY );
	sendPacket   << m_myCharInfo->GetSessionID();
	sendPacket << index;

	//======================================
	// �� �����Ը� ������
	//======================================
	m_myGameProc->SendPacketToMyTeam( m_myCharInfo->GetTeam(), sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameToLobbyCharGotoLobby()
{
	SPacket sendPacket( GL_GAME_GOTO_LOBBY );

	sendPacket << m_myGameProc->GetGameID();
	sendPacket << m_myCharInfo->GetSessionID();

	m_srvNet->SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameGotoLobby()
{
	SPacket sendPacket( SC_GAME_GOTO_LOBBY );

	//�κ��� ip�� port��ȣ�� �ִ´�.
	int size = strlen( m_document->LobbySrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->LobbySrvIP, size );
	sendPacket << m_document->LobbySrvPortNum;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameSelfDisconnect()
{
	SSynchronize sync( this );

	SPacket sendPacket( SC_LOBBY_GAME_SELF_DISCONNECT );

	//======================================
	// �����Ը� ������
	//======================================
	SendPacket( sendPacket );

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

	//======================================
	// �Բ� �����ϴ� ��ο��� ������( ���� ������ ���������� ��ο��� ������ �� )
	//======================================
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}
