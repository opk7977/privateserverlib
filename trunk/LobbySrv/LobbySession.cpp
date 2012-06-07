#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "SSessionMgr.h"
#include "DataLeader.h"

#include "LobbyMgr.h"
#include "Room.h"
#include "LobbyChar.h"
#include "CheckDB.h"

#include "SrvNet.h"

#include "SLogger.h"


SIMPLEMENT_DYNAMIC(LobbySession)
SIMPLEMENT_DYNCREATE(LobbySession)

LobbySession::LobbySession(void)
: m_myCharInfo(NULL)
, m_myRoom(NULL)
{
	m_sessionMgr	= &GetSessionMgr;
	m_document		= &GetDocument;
	m_lobbyMgr		= &GetLobbyMgr;
	m_roomMgr		= &GetRoomMgr;
	m_charMgr		= &GetCharMgr;
	m_DBMgr			= &GetDB;
	m_srvNet		= &GetSrvNet;
	m_logger		= &GetLogger;
}

LobbySession::~LobbySession(void)
{
}

void LobbySession::OnCreate()
{
	SSynchronize Sync( this );

	//sessionMgr�� �����ϰ�
	SSession::OnCreate();

	//����Ȯ�� ��Ŷ�� �����ش�.
	SSession::SendPacket( SC_LOBBY_CONNECT_OK );
}

void LobbySession::OnDestroy()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		//ĳ���� ������ ���ٸ� ���� Ȥ�� Ŭ�󿡼� �ڽ��� ������ ������ ���� �����
		if( m_srvNet->GetSession() == this )
		{
			m_srvNet->SrvNetClear();

			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::OnDestroy()\n �������� ������ ������ϴ�.\n\n") );
		}
		else
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::OnDestroy()\n ĳ���� ������ ���� session�� ������ �����ϴ�.\n\n") );
		}
	}
	else
	{
		if( !m_myCharInfo->GetIsPlay() )
		{
			//�������� �ƴ�
			if( m_myRoom == NULL )
			{
				//�κ� ����
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::OnDestroy()\nĳ���� %s���� �κ񿡼� ������ �����ϴ�.\n\n"),
								m_myCharInfo->GetID() );

				//�κ� �ִ� user
				//�׳� �κ񿡼� ����
				m_lobbyMgr->MinusUser( m_myCharInfo );
			}
			else
			{
				//�濡 ����
				int room = m_myRoom->GetRoomNum();

				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::OnDestroy()\nĳ���� %s���� %d�� �濡�� ������ �����ϴ�.\n\n"),
								m_myCharInfo->GetID(), room );

				//�濡�� �����ش�
				if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
				{
					//�濡 �����ִ� �ο��� ����
					//�� �ݰ�
					m_roomMgr->CloseRoom( room );

					m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
									_T("LobbySession::OnDestroy()\n%d�� ���� �����ϴ�.\n\n"),
									room );

					//���� �����ٴ� ������ �κ��� ����鿡�� ������.
					SendCloseRoom( room );
				}
				else
				{
					//�濡 ����� ����
					//���� �����ٴ� ���� �� ���� user�鿡�� �˸�
					SendRoomCharOut();
				}
			}
			//�κ��� ��ο��� �˸���
			SendPlayerDisconnect();

			//DB�� �ִ� user������ Logoutüũ�� �� �ش�
			m_DBMgr->UpdateLogin( m_myCharInfo->GetSessionID(), FALSE );

			//ĳ���� ������ ���� �ش�
			m_charMgr->ReturnCharSpace( m_myCharInfo );
			
		}
		else
		{
			//���� ��
			//�� �۾��� ����
		}
	}

	//session clear
	clear();

	//Mgr������ �����ش�.
	SSession::OnDestroy();
}

void LobbySession::clear()
{
	SSynchronize Sync( this );

	m_myCharInfo = NULL;
	m_myRoom = NULL;
}

//��Ŷ�� �ؼ��ؼ� ������ �Լ��� ȣ���Ѵ�.
//==============================================================
void LobbySession::PacketParsing( SPacket& packet )
{
	SSynchronize Sync( this );

	switch( packet.GetID() )
	{
	//==============================================================> GameSrv
	case SC_GAME_CONNECT_OK:
		RecvConnectServer();
		break;
// 	case GL_CONNECT_SERVER:
// 		RecvConnectServer();
// 		break;
	case GL_START_FAILD:
		RecvStartFaild( packet );
		break;
	case GL_START_OK:
		RecvGameStart( packet );
		break;
	case GL_GAME_END:
		RecvGameEnd( packet );
		break;
	case GL_PLAYER_DISCONNECT:
		RecvPlayerDiconnectInGame( packet );
		break;

	//==============================================================> Client
	case CS_LOBBY_INSERT_LOBBY:
		RecvInsertLobby( packet );
		break;
	case CS_LOBBY_CREATE_ROOM:
		RecvCreateRoom( packet );
		break;
	case CS_LOBBY_INSERT_ROOM:
		RecvInsertRoom( packet );
		break;
	case CS_ROOM_OUT_ROOM:
		RecvOutRoom();
		break;
	case CS_ROOM_CHAR_READY:
		RecvReady();
		break;
	case CS_ROOM_MAP_CHANGE:
		RecvMapChange( packet );
		break;
	case CS_ROOM_MODE_CHANGE:
		RecvChangeMode( packet );
		break;
	case CS_ROOM_TEAM_CHANGE:
		RecvTeamChange();
		break;
	case CS_LOBBYSRV_ALL_CHAT:
		RecvAllChat( packet );
		break;
	case CS_LOBBYSRV_TARGET_CHAT:
		RecvTargetChat( packet );
		break;
	case CS_ROOM_START:
		RecvRoomStartGame();
		break;

	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::PacketParsing()\n%d�� ��Ŷ, ��Ŷ ID�� ��ȿ���� �ʽ��ϴ�.\n\n"), packet.GetID() );
	}
}

//��Ŷ������ �Լ� �� ����
//==============================================================
// int LobbySession::SendPacket( SPacket& packet, BOOL tudp /*= FALSE */ )
// {
// 	SSynchronize Sync( this );
// 
// 	return SSession::SendPacket( packet, tudp );
// }

//���� ��Ŷ ó�� �Լ�
//==============================================================

void LobbySession::RecvConnectServer()
{
	SSynchronize Sync( this );

	//�̹� ����Ǿ� �ִ����� Ȯ���ϰ�
	if( m_srvNet->GetSession() != NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvConnectServer()\n���������� �̹� �������ϴ�. �� ������...\n\n") );
		return;
	}
	//���� ������!!
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvConnectServer()\n���Ӽ����� ����Ǿ����ϴ�.\n\n") );
	m_srvNet->SetSession( this );
}

void LobbySession::RecvStartFaild( SPacket& packet )
{
	SSynchronize Sync( this );

	int room;
	packet >> room;

	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, 
						_T("LobbySession::RecvStartFaild()\n�ش� ���� ������ �����ϴ�.\n\n") );
		return;
	}

	{
		SSynchronize sync( tmpRoom );

		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, 
						_T("LobbySession::RecvStartFaild()\n%d�� ���� ���� ���ۿ� �����߽��ϴ�.\n\n"), room );

		//�Ѱ� ���� ���� ���忡�� ���� ���� ���� ��Ŷ�� ������
		tmpRoom->GetLeader()->GetSession()->SendStartGameResult();
		//���� ���¸� �ٲ�����
		tmpRoom->SetNormal();
	}

}

void LobbySession::RecvGameStart( SPacket& packet )
{
	//SSynchronize Sync( this );

	int room;
	packet >> room;

	//�ش� ��ȣ�� �´� ���� ã�� �´�
	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvGameStart()\n�ش� ���� ������ �����ϴ�.\n\n") );
		return;
	}

	{
		SSynchronize sync( tmpRoom );

		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, 
						_T("LobbySession::RecvGameStart()\n%d�� ���� ������ �����մϴ�.\n\n"), room );

		//�� ���� Play���·� �ٲ۴�
		tmpRoom->SetPlay();

		//�κ��� ����鿡�� �ش� ���� �������� �Ǿ��ٰ� �˸���
		SendLobbyGameStart( room );

		//���� player�鿡�� ���Ӽ����� �̵��϶�� ��Ŷ�� ������.
		SendStartGameInRoom( room );

		//�濡 �ִ� list�� ����ְ�, �ο����� �ʱ�ȭ �� �ش�
		//������ ��� ���� �ٽ� ĳ���� ������ �����ؾ� �ϴ� ��� �ش�
		tmpRoom->ListReset();
	}
}

void LobbySession::RecvGameEnd( SPacket& packet )
{
	//���� �ȿ� player�� ��� �������� �޴� ��Ŷ�̴�
	//������ ���� �ǰ� player�� ��� ������. �׷��� ���� �ٽ� ���� �ش�.
	//SSynchronize Sync( this );

	int room;
	packet >> room;

	//�ش� ��ȣ�� �´� ���� ã�� �´�
	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvGameEnd()\n�ش� ���� ������ �����ϴ�.\n\n") );
		return;
	}


	{
		SSynchronize sync( this );

		if( !tmpRoom->IsOpen() )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::RecvGameEnd()\n%d�� ���� �̹� �������ϴ�.\n\n"), room );
			return;
		}


		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("LobbySession::RecvGameEnd()\n%d�� ���� ������ �����߽��ϴ�.\n\n"), room );

		//�� ���� play���¸� Ǯ�� �ش�
		tmpRoom->SetNormal();

	}		
	//�κ� ����鿡�� ���� �ٽ� ���ȴٰ� �˷� �ش�.
	SendLobbyGameEnd( room );
}

void LobbySession::RecvPlayerDiconnectInGame( SPacket& packet )
{
	//================================================
	// ���� ���μ������� player�� ������ ���� �Ѱ��
	//================================================
	//SSynchronize Sync( this );

	int room, session, team;
	packet >> room;
	packet >> session;
	packet >> team;

	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvPlayerDiconnectInGame()\n���� �������� %d�� ���� sessionID %d�� ĳ���Ͱ� ������ �����߽��ϴ�.\n\n"), 
					room, 
					session );

	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvPlayerDiconnectInGame()\n%d�� ���� ������ �����ϴ�.\n\n"), 
					room );
	}
	else
	{
		SSynchronize sync( tmpRoom );

		//���ο��� ���δ�.
		if( !tmpRoom->DelPlayerInRoomAtPlaying( team ) )
		{
			//�濡 �����ִ� �ο��� ����
			//�� �ݰ�
			m_roomMgr->CloseRoom( room );

			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::RecvPlayerDiconnectInGame()\n%d�� ���� �����ϴ�.\n\n"),
								room );

			//���� �����ٴ� ������ �κ��� ����鿡�� ������.
			SendCloseRoom( room );
		}
	}

	//�κ� �ִ� ����鿡�� 1�� ���ӿ��� �����ٰ� �˸���
	SendPlayerDisconnect( session );

	//ĳ���� �α��� ������ ������ �ش�
	m_DBMgr->UpdateLogin( session, FALSE );

	//ĳ���� ������ ���� �� �ش�.
	m_charMgr->ReturnCharSpace( session );
}

//--------------------------------------------------------------

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	int sessionId, roomNum;
	packet >> sessionId;
	packet >> roomNum;

//#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvInsertLobby()\n")
					_T("sessionID : %d/ Room : %d\n\n")
					, sessionId
					, roomNum );
//#endif

	//SSynchronize Sync( this );

	//���ȣ�� Ȯ���ؼ� �� ���������� ����
	if( roomNum > 0 )
	{
		{
			SSynchronize sync( this );

			//======================================
			// ������ �� ����
			//======================================
			//--------------------------------------
			// ĳ���Ϳ� ���� ������ �ٽ� ����
			//--------------------------------------
			//ĳ���� ������ �޾� �´�
			m_myCharInfo = m_charMgr->FindCharAsSessionId( sessionId );
			if( m_myCharInfo == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::RecvInsertLobby()\n%d���� �ش��ϴ� ĳ������ ������ �������� �ʽ��ϴ�.\n\n"),
								sessionId );
				return;
			}
			//�� ������ �޾� �´� 
			m_myRoom = m_roomMgr->FindRoom( roomNum );
			if( m_myRoom == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvInsertLobby()\n%d���� �ش��ϴ� ���� �������� �ʽ��ϴ�.\n\n"),
					roomNum );
				return;
			}
			//--------------------------------------

			//�ش� ĳ������ session������ �ٽ� ����
			SetSessionID( sessionId );
		}

		m_myCharInfo->SetSession( this );

		//ĳ������ ���� ���� ���¿� ready���¸� �ʱ�ȭ ������
		m_myCharInfo->SetIsPlay( FALSE );
		m_myCharInfo->SetReady( FALSE );
	

		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("LobbySession::RecvInsertLobby()\nĳ���� %s, %d�� ������ ����.\n\n"),
						m_myCharInfo->GetID(), roomNum );

		//�濡 ���� �߰�
		m_myRoom->AddPlayerInRoom( m_myCharInfo );

		//�� ������ �� �� ����鿡�� ����
		SendRoomMyInfoToOtherChar();

		//�������� ������ ������ ����
		SendRoomOtherCharInfo();
	}
	else
	{
		if( m_myCharInfo != NULL )
		{
			//--------------------------------------
			// �濡�� �κ��
			//--------------------------------------
			//�κ�list�� ���� �ٽ� �־� �ش�
			m_lobbyMgr->AddUser( m_myCharInfo );
		}
		else
		{
			//--------------------------------------
			// ���� ����
			//--------------------------------------
			//ĳ���� ������ �ϳ� ��� �´�
			m_myCharInfo = m_charMgr->GetCharSPace();
			if( m_myCharInfo == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::RecvInsertLobby()\n���̻� ĳ���͸� ������ �� �ִ� ĳ���� ������ �������� �ʽ��ϴ�.\n\n") );
				//////////////////////////////////////////////////////////////////////////
				// ���̻� ������ �� ���ٴ� ��Ŷ�� ������
				//
				//
				//
				//////////////////////////////////////////////////////////////////////////
				return;
			}

			// DB���� ������ ã�´�.
			m_DBMgr->GetData( sessionId, m_myCharInfo->GetID() );

			//======================================
			// �� ���� ����
			//======================================
			//������ ���ǿ� �� ID�� �־� �ش�
			SetSessionID( sessionId );
			//ĳ���� ������ �� ID�� �־� �ش�
			m_myCharInfo->SetSessionID( sessionId );
			//�ش� ĳ������ session������ ����
			m_myCharInfo->SetSession( this );
			//======================================

			//�κ�list�� ���� �߰�
			m_lobbyMgr->AddUser( m_myCharInfo );

			//�κ��� �����鿡�� �� ������ ������.
			//��ο��� ����( ���� ���� ������! )
			SendMyCharInfo();
		}
		//������ �� ������ ������.
		SendRoomInfo();

		//�׸��� ��� ���� ������ ������.( �� ���� ���� )
		SendOtherCharInfo();
	}
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{
	int size;
	TCHAR title[50]={0,};

	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	packet >> size;
	packet.GetData( title, size );

	//�� ��� ������
	int stageMap, gameMode, playTime, playCount;
	packet >> stageMap >> gameMode >> playTime >> playCount;

	m_myRoom = m_roomMgr->OpenRoom( title );
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n���� ���⿡ �����߽��ϴ�.\n\n") );
		SendResultCreate( -10 );
		return;
	}
	//���� ��� ����
	m_myRoom->SetStageMap( stageMap );
	m_myRoom->SetGameMode( gameMode );
	m_myRoom->SetPlayTime( playTime );
	m_myRoom->SetPlayCount( playCount );
	
	//������ �켱 ���� ������ �д�
	m_myRoom->SetLeader( m_myCharInfo );

//#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::RecvCreateRoom()\n")
					_T("sessionID : %d\n")
					_T("ID : %s\n")
					_T("Room : %d��\n")
					_T("RoomTitle : %s\n\n")
					, m_myCharInfo->GetSessionID()
					, m_myCharInfo->GetID()
					, m_myRoom->GetRoomNum()
					, title );
//#endif

	//�游��� ����
	SendResultCreate( m_myRoom->GetRoomNum() );

	//��ο��� ���� ��������ٴ� ��Ŷ�� ������.
	SendOpenRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//�濡 ���ٴ� ��û�� ����
	int room;
	packet >> room;

	//SSynchronize Sync( this );
	
	//�켱 ���ȣ�� ���� �޾� �� ����
	m_myRoom = m_roomMgr->FindRoom( room );
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n[ĳ���� %s] %d�� �� ���忡 �����߽��ϴ�.\n\n"),
						m_myCharInfo->GetID(), room );
		SendResultInsert(-10);
		return;
	}

	//ĳ���Ϳ� ���� �־� �ش�.
	m_myCharInfo->SetRoom( m_myRoom );

	//���� ���� ���� �غ����̰ų� �ο��� �� á���� �� �� ����
	if( !m_myRoom->CanInsert() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n[ĳ���� %s] %d�� ���� ���� �� �� ���� ���� �Դϴ�.\n\n"),
						m_myCharInfo->GetID(), room );
		m_myRoom = NULL;
		SendResultInsert(-10);
		return;
	}
	//������ readyCount�� �޾� ����
	//int oldReadyCount = m_myRoom->GetReadyCount();

	//���� �������� ���� �Ѵٸ� ����
	m_myRoom->AddPlayerInRoom( m_myCharInfo );

	//�κ񸮽�Ʈ������ �� ������ ���� �ش�.
	m_lobbyMgr->MinusUser( m_myCharInfo );

//#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvInsertRoom()\n%s(SessionId:%d��) %d�� ������ ��\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, room );
//#endif

	//�켱 ���⿡ �����ߴٴ� ��Ŷ�� ������
	SendResultInsert( room );

	//�κ�� �濡 ����� ���ٴ� ���� �˸���.
	SendInsertRoom();

	//�׸��� �濡 �ִ� ����鿡�� �� ������ ������.
	SendRoomMyInfoToOtherChar();

	//������ �濡 �ִ� ��� ������� ������ ������.
	SendRoomOtherCharInfo();

	//������ ������ ������ �˸���.
	SendRoomLeader();

	//���忡�� start��ư���� packet�� ������ �Լ�
	//SendStartBtnForVisible( oldReadyCount );
	SendStartBtnForVisible();
}

void LobbySession::RecvOutRoom()
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	//���� ���°� �븻�� �ƴϸ� ����
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d������ �̹� �Ϲ� ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

//#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvOutRoom()\n%s(sessionId:%d) %d�� ���� �����ϴ�\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );
//#endif

	if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
	{
		//�濡 ��� ���� ���� �����ش�
		m_roomMgr->CloseRoom( m_myRoom->GetRoomNum() );
		SendCloseRoom( m_myRoom->GetRoomNum() );
	}

	//��/ ready���� �ʱ�ȭ
	m_myCharInfo->SetRoom(NULL);
	m_myCharInfo->SetTeam(-1);
	m_myCharInfo->SetReady(FALSE);

	SendRoomOutResult();
	
	//���� Ŭ��鿡�� ���� ��������� ������
	SendRoomCharOut();

	//�κ��� Ŭ��鿡�� ���� ��濡�� ���Դ����� �˸���
	SendLobbyRoomCharOut();

	//�������� �������� �ʱ�ȭ
	m_myRoom = NULL;
}

void LobbySession::RecvReady()
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	//���� ���°� �븻�� �ƴϸ� ����
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n%d������ �̹� �Ϲ� ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n���������� ready������.\n\n") );
		return;
	}

	m_myCharInfo->SetReady( m_myCharInfo->GetReady() == ROOM_READY_NON ? ROOM_READY_OK : ROOM_READY_NON );


	//������ readyCount�� �޾� ����
	//int oldReadyCount = m_myRoom->GetReadyCount();

	//�濡 ready���ڸ� ������ �ش�.
	m_myRoom->ChangReadyCount( m_myCharInfo->GetReady() );

//#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvReady()\n")
					  _T("%s(sessionId:%d) Ready���°� %s���� ����Ǿ����ϴ�.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myCharInfo->GetReady() ? _T("����") : _T("�غ���") );
	//#endif

	SendRoomCharReady();

	//���� ���డ�� ���¸� �������� ��������
// 	if( m_myRoom->PossiblePlay() )
// 		SendRoomStartVisible();
// 	else if( oldReadyCount == m_myRoom->GetPlayerCount()-1 )
// 	{
// 		//ready�ο��� �پ��ٸ� ��Ȱ���� �����ش�
// 		if( m_myRoom->GetReadyCount() < oldReadyCount )
// 			SendRoomStartInvisible();
// 	}
//	SendStartBtnForVisible( oldReadyCount );
	SendStartBtnForVisible();
}

void LobbySession::RecvMapChange( SPacket& packet )
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvMapChange()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
			GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvMapChange()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	//�켱 �ְ� ������ �´��� Ȯ��
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvMapChange()\n[ĳ���� %s] ĳ���Ͱ� ������ �ƴմϴ�.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	int stageMap;
	packet >> stageMap;

	//���� �ٲ�� �ƴϸ� �׳� �Ѿ� ����
	if( m_myRoom->GetStageMap() == stageMap )
		return;

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvMapChange()\n%d������ ���� %d������ �ٲ�ϴ�.\n\n"),
					m_myRoom->GetRoomNum(), stageMap );

	//�ƴϸ� ������ �ְ�
	m_myRoom->SetStageMap( stageMap );

	//�� ����鿡�� ���� �ְ�
	SendRoomMapChange();

	//�κ��� ����鿡�� ���� �ش�
	SendLobbyMapChange();
}

void LobbySession::RecvChangeMode( SPacket& packet )
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
			GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	//�̳��� �������� Ȯ���Ѵ�
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n[ĳ���� %s] %d�� ���� ������ �ƴմϴ�.\n\n"),
			m_myCharInfo->GetID(), m_myRoom->GetRoomNum() );
		return;
	}

	//���� �ʺ����� �������� Ȯ���Ѵ�
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n%d�� ���� ���� �����͸� ������ �� �ִ� ���°� �ƴմϴ�.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	int gameMode, playTime, playCount;

	packet >> gameMode;
	packet >> playTime;
	packet >> playCount;

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvChangeMode()\n%d����/ ���: %d, playTime: %d, Count: %d�� ����˴ϴ�.\n\n"),
					m_myRoom->GetRoomNum(), gameMode, playTime, playCount );

	m_myRoom->SetGameMode( gameMode );
	m_myRoom->SetPlayTime( playTime );
	m_myRoom->SetPlayCount( playCount );

	//���� Mode���� ���� ��ο��� �˸���.
	SendRoomModeChange();

	//�κ��� ����鿡�Ե� �˸���
	SendLobbyModeChange();
}

void LobbySession::RecvTeamChange()
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	//ready���¸� �� �ٲ� �� ����..
	if( m_myCharInfo->GetReady() == ROOM_READY_OK )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[ĳ���� %s] �� �Ӹ� ready����..\n\n"),
						m_myCharInfo->GetID() );
		return;
	}
	
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}


	//���� ���°� �븻�� �ƴϸ� ����
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d������ �̹� �Ϲ� ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	//�������̸� ����� or �������̸� ��������
	int team = ( m_myCharInfo->GetTeam() == ROOM_TEAM_ATT ) ? ROOM_TEAM_DEF : ROOM_TEAM_ATT;

//#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvTeamChange()\n")
					  _T("%s(sessionId:%d) ���� %s���� �����մϴ�.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("������") : _T("������") );
//#endif

	//�濡 ��ī��Ʈ ����
	if( !m_myRoom->ChangeTeam( team ) )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvTeamChange()\n")
					_T("%s(sessionId:%d) %s�� �ο��� ������ ������ �� �����ϴ�.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("������") : _T("������") );

		return;
	}


	//���� �ٲٰ�
	m_myCharInfo->SetTeam( team );
	

	SendRoomTeamChange();
}

void LobbySession::RecvAllChat( SPacket& packet )
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChat()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
			GetSocket() );
		return;
	}
	int size;
	TCHAR chatText[200] = {0,};

	packet >> size;
	packet.GetData( chatText, size );

//#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvChat()\n")
					_T("[%s] %s\n\n")
					, m_myCharInfo->GetID()
					, chatText );

//#endif

	//��ü ü��
	SendAllChat( chatText );	
}

void LobbySession::RecvTargetChat( SPacket& packet )
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvTargetChat()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
			GetSocket() );
		return;
	}

	int size;
	TCHAR targetID[20] = {0,};
	TCHAR chatText[200] = {0,};

	packet >> size;
	packet.GetData( targetID, size );
	packet >> size;
	packet.GetData( chatText, size );

	int sessionId = m_DBMgr->GetSessionId( targetID );
	if( sessionId == 0 )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvTargetChat()\n[ĳ����%s] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
			GetSocket() );
		return;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvChat()\n")
					_T("[%s] %s\n\n")
					, m_myCharInfo->GetID()
					, chatText );
// #endif

	//�Ӹ�
	SendTargetChatToTarget( sessionId, chatText );
	SendTargetChatToMe( sessionId, chatText );
}

void LobbySession::RecvRoomStartGame()
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[���Ϲ�ȣ%d] ĳ���� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	//������ ����Ǿ� �ִ� �������� Ȯ��
	if( !m_srvNet->IsConnect() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvRoomStartGame()\n���� ������ ����Ǿ� ���� �ʽ��ϴ�.\n\n") );
		SendStartGameResult();
		return;
	}

	//���� ���°� �븻�� �ƴϸ� ����
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d������ �̹� �Ϲ� ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
		SendStartGameResult();
		return;
	}

	//�켱 �ְ� ������ �´��� Ȯ��
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[ĳ���� %s] ĳ���Ͱ� ������ �ƴմϴ�.\n\n"),
						m_myCharInfo->GetID() );
		//���� ��Ŷ�� ������
		SendStartGameResult();
		return;
	}
	
	//���� ���� ���� �������� Ȯ��
	if( !m_myRoom->PossiblePlay() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d�� ���� ���� ���� ������ ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
		//���� ��Ŷ�� ������
		SendStartGameResult();
		return;
	}

	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvRoomStartGame()\n%d���� ���� ������ �õ�(?)�մϴ�.\n\n"),
					m_myRoom->GetRoomNum() );

	//���� �����غ����� ���·� �ٲ۴�
	m_myRoom->SetReady();

	//���Ӽ����� ������ ������
	SendCreateGameProc();
}


//������ �Լ�
//==============================================================

BOOL LobbySession::SendCreateGameProc()
{
	SPacket sendPacket;
	sendPacket.SetID( LG_START_GAME );
	//���Ӽ����� �غ� �϶�� ����

	//int count = m_myRoom->GetPlayerCount();

	sendPacket << m_myRoom->GetRoomNum();		//���ȣ
	sendPacket << m_myRoom->GetStageMap();		//���� ��
	sendPacket << m_myRoom->GetGameMode();		//���� ���
	sendPacket << m_myRoom->GetPlayTime();		//�Ǵ� playTime
	sendPacket << m_myRoom->GetPlayCount();		//���� �Ǽ�

	//�濡 �ִ� ĳ���͵��� ������ ��´�
	m_myRoom->PackagePlayerInRoomForGame( sendPacket );

	//���� ������ ����
	m_srvNet->SendToGameServer( sendPacket );
	
	return TRUE;
}

//--------------------------------------------------------------

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//��� ĳ������ ������ ����
	//m_lobbyMgr->PackageDataAllInLobby( sendPacket );
	m_charMgr->PackageAllCharacter( sendPacket );

	//������ ��Ŷ�� ������!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendOtherCharInfo()\n")
														_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

// #endif
		return FALSE;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOtherCharInfo()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());
// #endif

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//�� ������ ��´�
	sendPacket << 1;

	m_myCharInfo->PackageMyInfoForLobby( sendPacket );

	//��ο��� ����( ���� ���� ������! )
	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendMyCharInfo()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomInfo()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_ROOMINFO );
	//�� ������ 0�̸� �Ⱥ����� �ȴ�
	if( m_roomMgr->OpenRoomCount() <= 0 )
		return TRUE;	

	//��� �� ������ ��´�
	m_roomMgr->PackageOpenRoomInfo( sendPacket );

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomInfo()\n")
			_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

// #endif
		return FALSE;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomInfo()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

// #endif

	return TRUE;
}

BOOL LobbySession::SendResultCreate( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_CREATE );
	sendPacket << result;

	if( result > 0 )
	{
		int size = _tcslen( m_myRoom->GetTitle() ) * sizeof( TCHAR );
		sendPacket << size;
		sendPacket.PutData( m_myRoom->GetTitle(), size );

		m_myRoom->PackageRoomModeInfo( sendPacket );
	}

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultCreate()\n")
														_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

// #endif
		return FALSE;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultCreate()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

// #endif
	return TRUE;
}

BOOL LobbySession::SendOpenRoom()
{
	//���� ������ ����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OPEN_ROOM );
	sendPacket << m_myRoom->GetRoomNum();
	int size = _tcslen( m_myRoom->GetTitle() ) * sizeof( TCHAR );
	sendPacket << size;
	sendPacket.PutData( m_myRoom->GetTitle(), size );
	//���� ���� ������
	m_myRoom->PackageRoomModeInfo( sendPacket );

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOpenRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

// #endif

	return TRUE;
}

BOOL LobbySession::SendCloseRoom( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CLOSE_ROOM );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendCloseRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

// #endif

	return TRUE;
}

BOOL LobbySession::SendResultInsert( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_INSERT );
	sendPacket << result;

// 	if( result > 0 )
// 	{
// 		SSynchronize Sync( this );
// 
// 		int size = _tcslen( m_myRoom->GetTitle() ) * sizeof( TCHAR );
// 		sendPacket << size;
// 		sendPacket.PutData( m_myRoom->GetTitle(), size );
// // 		sendPacket << m_myRoom->GetStageMap();
// // 		sendPacket << m_myRoom->GetGameMode();
// // 		sendPacket << m_myRoom->GetPlayTime();
// // 		sendPacket << m_myRoom->GetPlayCount();
// 		m_myRoom->PackageRoomModeInfo( sendPacket );
// 	}

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultInsert()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

// #endif
		return FALSE;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultInsert()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

// #endif

	return TRUE;
}

BOOL LobbySession::SendInsertRoom()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_INSERT_ROOM );
	
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myRoom->GetRoomNum();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendInsertRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomOtherCharInfo()
{
	//������ �濡 �ִ� ��� ����� ������ ������( �� ��������...)
	//������ ��Ŷ�� �����.
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	//ĳ���� ������ ��� ��´�
	m_myRoom->PackagePlayerInRoom( sendPacket );

	//������ ������!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomOtherCharInfo()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

// #endif
		return FALSE;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomOtherCharInfo()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomMyInfoToOtherChar()
{
	//�� ������ �濡 �ִ� ��� ����鿡�� ������
	//��������Ŷ�� �����
	SPacket	sendPacket;

	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	sendPacket << 1;			//count

	//�� ������ ��´�
	m_myCharInfo->PackageMyInfoForRoom( sendPacket );

	//��ο��� ������ ������ ������ �ʴ´�.
	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_myRoom->GetRoomNum() );

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeader()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << m_myRoom->GetLeader()->GetSessionID();

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

// #endif
		return FALSE;
	}

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeader()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID() );

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll( int leader )
{
	//������ �ٲ�� ��ο��� ���� ��ȣ�� �����ش�
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << leader;

	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_myRoom->GetRoomNum() );

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomOutResult()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_OUT_RESULT );

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
	//////////////////////////////////////////////////////////////////////////
	//�濡 ������ packet 
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_OUT );
	sendPacket << m_myCharInfo->GetSessionID();

	//�濡�� ���� �����ٴ� ��ȣ�� �����ְ�
	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::SendRoomCharOut()\n")
					_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n"),
					m_myRoom->GetRoomNum() );

// #endif

	//--------------------------------------------------------------
	// ���� ������ ���� leader�� �ٲ���ٸ� �ٲ� ������ ������� �Ѵ�.
	// �� �༮�� leader���� Ȯ���Ѵ�.
	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		//leader�� �ٲٰ� ��Ŷ�� �����ش�.
		SendRoomLeaderToAll( m_myRoom->ChangeLeader()->GetSessionID() );

		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
						_T("LobbySession::SendRoomCharOut()\n")
						_T("%d�� ���� %s���� ������ �Ǽ̽��ϴ�.\n\n"),
						m_myRoom->GetRoomNum(), m_myRoom->GetLeader()->GetID() );

// 		//����°� ���� ���ɻ��¶�� ���忡�� �˸���
// 		if( m_myRoom->PossiblePlay() )
// 			SendRoomStartVisible();
	}
	//--------------------------------------------------------------

	SendStartBtnForVisible();

	return TRUE;
}

BOOL LobbySession::SendLobbyRoomCharOut()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_ROOM_PLAYER_OUT );
	sendPacket << m_myRoom->GetRoomNum();
	sendPacket << m_myCharInfo->GetSessionID();

	//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::SendRoomCharOut()\n")
					_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

// #endif

	return TRUE;
}

BOOL LobbySession::SendRoomCharReady()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_READY );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetReady();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomMapChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_MAP_CHANGE );

	sendPacket << m_myRoom->GetStageMap();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyMapChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_MAP_CHANGE );

	sendPacket << m_myRoom->GetRoomNum();
	sendPacket << m_myRoom->GetStageMap();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomModeChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_MODE_CHANGE );

	m_myRoom->PackageRoomModeInfo( sendPacket );

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyModeChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_MODE_CHANGE );
	sendPacket << m_myRoom->GetRoomNum();

	m_myRoom->PackageRoomModeInfo( sendPacket );

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomTeamChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_TEAM_CHANGE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetTeam();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendAllChat( TCHAR* chat )
{
	//ID�� ä�� ������ �ִ´�.
	TCHAR tmpText[100]={0,};
	wsprintf( tmpText, _T("[%s] %s\n"), m_myCharInfo->GetID(), chat );

	//��Ŷ�� �����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBYSRV_ALL_CHAT );
	if( m_myRoom == NULL )
	{
		//�κ� ������ 0�� �־� �ش�
		sendPacket << 0;
	}
	else
	{
		//�濡 ������ ���ȣ�� �־� �ش�
		sendPacket << m_myRoom->GetRoomNum();
	}

	int size = _tcslen( tmpText )*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( tmpText, size );
	//////////////////////////////////////////////////////////////////////////

	if( m_myRoom == NULL )
	{
		//�κ�κ�����
		//////////////////////////////////////////////////////////////////////////
		m_lobbyMgr->SendPacketAllInLobby( sendPacket );

// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
			_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );
// #endif
	}
	else
	{

		//�����ױ��� ������ �Ѵ�.
		m_myRoom->SendPacketAllInRoom( sendPacket );

// #ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
			_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
			, m_myRoom->GetRoomNum() );
// #endif

	}

	return TRUE;
}

BOOL LobbySession::SendTargetChatToTarget( int target, TCHAR* chat )
{
	LobbySession* tmpSession = (LobbySession*)m_sessionMgr->FindSession( target );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::SendChatForTarget()\n")
			_T("%d�� ������ ĳ���Ͱ� �������� �ʽ��ϴ�.\n\n")
			, target );
		return FALSE;
	}

	//ID�� ä�� ������ �ִ´�.
	TCHAR tmpText[255]={0,};
	wsprintf( tmpText, _T("[From %s] %s\n"), m_myCharInfo->GetID(), chat );

	//��Ŷ�� �����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBYSRV_TARGET_CHAT );

	Room* tmpRoom = tmpSession->GetMyRoom();
	if( tmpRoom == NULL )
	{
		//�κ� ������ 0�� �־� �ش�
		sendPacket << 0;
	}
	else
	{
		//�濡 ������ ���ȣ�� �־� �ش�
		sendPacket << tmpRoom->GetRoomNum();
	}

	//ä�ù����� �־� �ش�
	int size = _tcslen( tmpText )*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( tmpText, size );

	tmpSession->SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendTargetChatToMe( int target, TCHAR* chat )
{
	LobbySession* tmpSession = (LobbySession*)m_sessionMgr->FindSession( target );
	if( tmpSession == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::SendChatForTarget()\n")
			_T("%d�� ������ ĳ���Ͱ� �������� �ʽ��ϴ�.\n\n")
			, target );
		return FALSE;
	}

	//ID�� ä�� ������ �ִ´�.
	TCHAR tmpText[255]={0,};
	wsprintf( tmpText, _T("[To %s] %s\n"), tmpSession->GetMyInfo()->GetID(), chat );

	//��Ŷ�� �����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBYSRV_TARGET_CHAT );

	if( m_myRoom == NULL )
	{
		//�κ� ������ 0�� �־� �ش�
		sendPacket << 0;
	}
	else
	{
		//�濡 ������ ���ȣ�� �־� �ش�
		sendPacket << m_myRoom->GetRoomNum();
	}

	//ä�ù����� �־� �ش�
	int size = _tcslen( tmpText )*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( tmpText, size );

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendStartBtnForVisible( int oldReadyCount )
{
	//���� ���డ�� ���¸� �������� ��������
	if( m_myRoom->PossiblePlay() )
		SendRoomStartVisible();

	else if( oldReadyCount == m_myRoom->GetPlayerCount()-1 )
	{
		//ready�ο��� �پ��ٸ� ��Ȱ���� �����ش�
		if( m_myRoom->GetReadyCount() < oldReadyCount )
			SendRoomStartInvisible();
	}

	return TRUE;
}

BOOL LobbySession::SendStartBtnForVisible()
{
	if( m_myRoom->PossiblePlay() )
		SendRoomStartVisible();
	else
		SendRoomStartInvisible();

	return TRUE;
}

BOOL LobbySession::SendRoomStartVisible()
{
	SPacket sendPacket( SC_ROOM_START_VISIBLE );

	m_myRoom->GetLeader()->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomStartInvisible()
{
	SPacket sendPacket( SC_ROOM_START_INVISIBLE );

	if( m_myRoom->GetPlayerCount() > 0 )
		m_myRoom->GetLeader()->GetSession()->SendPacket( sendPacket );
		
	return TRUE;
}

BOOL LobbySession::SendStartGameResult()
{
	//���� ��ŸƮ�� ����!
	SPacket sendPacket;

	sendPacket.SetID( SC_ROOM_START_RESULT );
	sendPacket << -1;

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendStartGameResult()\n���۾�� ��Ŷũ�Ⱑ �ٸ��ϴ�.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL LobbySession::SendStartGameInRoom( int roomNum )
{
	//���� �޾ƿ� ����.
	Room* tmpRoom = m_roomMgr->FindRoom( roomNum );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::SendStartGameInRoom()\n")
				_T("%d�� ���� �������� �ʽ��ϴ�.\n\n")
				, roomNum );

		return FALSE;
	}

	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_GAME_START );

	//���Ӽ����� ������ ������
	int size = strlen( m_document->GameSrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->GameSrvIP, size );
	sendPacket << m_document->GameSrvPortNum;
	//�ش���� UDP��ȣ�� �־� �ش�
	sendPacket << (m_document->GameSrvPortNum+tmpRoom->GetRoomNum()+10);

	//�濡 �ִ� player�鿡�� ����!
	tmpRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyGameStart( int roomNum )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_GAME_START );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyGameEnd( int roomNum )
{
	SPacket sendPacket;
	
	sendPacket.SetID( SC_LOBBY_GAME_END );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << m_myCharInfo->GetSessionID();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendPlayerDisconnect()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

// #endif

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect( int sessionId )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << sessionId;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

// #ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendPlayerDisconnect()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );
// #endif

	return TRUE;
}
