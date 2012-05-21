#include "LobbySession.h"
#include "LobbyProtocol.h"

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
	m_document	= &GetDocument;
	m_lobbyMgr	= &GetLobbyMgr;
	m_roomMgr	= &GetRoomMgr;
	m_charMgr	= &GetCharMgr;
	m_DBMgr		= &GetDB;
	m_srvNet	= &GetSrvNet;
	m_logger	= &GetLogger;
}

LobbySession::~LobbySession(void)
{
}

void LobbySession::OnCreate()
{
	//sessionMgr�� �����ϰ�
	SSession::OnCreate();

	//����Ȯ�� ��Ŷ�� �����ش�.
	SendPacket( SC_LOBBY_CONNECT_OK );
}

void LobbySession::OnDestroy()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		//ĳ���� ������ ���ٸ� ���� Ȥ�� Ŭ�󿡼� �ڽ��� ������ ������ ���� �����
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::OnDestroy()\n ĳ���� ������ ���� session�� ������ �����ϴ�.\n\n") );
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

				//��ο��� �˸���
				//SendPlayerDisconnect();
			}
			else
			{
				//�濡 ����
				int room = m_myRoom->GetRoomNum();

				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::OnDestroy()\nĳ���� %s���� %d�� �濡�� ������ �����ϴ�.\n\n"),
								room, m_myCharInfo->GetID() );

				//�濡�� �����ش�
				if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
				{
					//�濡 �����ִ� �ο��� ����
					//�� �ݰ�
					m_roomMgr->CloseRoom( room );

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
	switch( packet.GetID() )
	{
	//==============================================================> GameSrv
	case GL_CONNECT_SERVER:
		RecvConnectServer();
		break;
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
	case CS_ROOM_TEAM_CHANGE:
		RecvTeamChange();
		break;
	case CS_LOBBY_CHAT:
		RecvChat( packet );
		break;
	case CS_ROOM_START:
		RecvRoomStartGame();
		break;

	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::PacketParsing()\n%d�� ��Ŷ, ��Ŷ ID�� ��ȿ���� �ʽ��ϴ�.\n\n") );
	}
}


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
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
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

	//�Ѱ� ���� ���� ���忡�� ���� ���� ���� ��Ŷ�� ������
	tmpRoom->GetLeader()->GetSession()->SendStartGameResult();
}

void LobbySession::RecvGameStart( SPacket& packet )
{
	SSynchronize Sync( this );

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

void LobbySession::RecvGameEnd( SPacket& packet )
{
	//���� �ȿ� player�� ��� �������� �޴� ��Ŷ�̴�
	//������ ���� �ǰ� player�� ��� ������. �׷��� ���� �ٽ� ���� �ش�.
	SSynchronize Sync( this );

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

	//�� ���� play���¸� Ǯ�� �ش�
	tmpRoom->SetNormal();
	
	//�κ� ����鿡�� ���� ���ȴٰ� �˷� �ش�.
	SendLobbyGameEnd( room );
}

void LobbySession::RecvPlayerDiconnectInGame( SPacket& packet )
{
	//================================================
	// ���� ���μ������� player�� ������ ���� �Ѱ��
	//================================================
	SSynchronize Sync( this );

	int room, session;
	packet >> room;
	packet >> session;


	//�κ� �ִ� ����鿡�� 1�� �����ٴ� ǥ�ø� �����ش�.
	SendLobbyRoomCharOut( room );

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

#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvInsertLobby()\n")
					_T("sessionID : %d\n")
					_T("Room : %d\n\n")
					, sessionId
					, roomNum );
#endif

	SSynchronize Sync( this );

	//���ȣ�� Ȯ���ؼ� �� ���������� ����
	if( roomNum > 0 )
	{
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
		m_myCharInfo->SetSession( this );

		//ĳ������ ���� ���� ���¿� ready���¸� �ʱ�ȭ ������
		m_myCharInfo->SetIsPlay( FALSE );
		m_myCharInfo->SetReady( FALSE );

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
			m_lobbyMgr->AddUser( this );

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			///////�κ��� ��ο��� �濡�� �κ�� ���Դٴ� ��Ŷ�� �����ش�.////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
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

		//�׸��� �κ��� ���� ������ ������.( �� ���� ���� )
		SendOtherCharInfo();
	}
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{
	int size;
	TCHAR title[50];

	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[���Ϲ�ȣ%d] ĳ���͸� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	packet >> size;
	packet.GetData( title, size );

	m_myRoom = m_roomMgr->OpenRoom( title );
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n���� ���⿡ �����߽��ϴ�.\n\n") );
		SendResultCreate( -1 );
		return;
	}
	m_myRoom->AddPlayerInRoom( m_myCharInfo );
	m_myRoom->SetLeader( m_myCharInfo );

#ifdef _DEBUG 
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

#endif

	//������ ������ �κ񿡼��� ���� �ش�
	m_lobbyMgr->MinusUser( m_myCharInfo );

	//�游��� ����
	SendResultCreate( 1 );

	//��ο��� ���� ��������ٴ� ��Ŷ�� ������.
	SendOpenRoom( m_myRoom->GetRoomNum(), title, size );

	//��ο��� ���� �濡 ���ٴ� ��Ŷ�� ������.
	SendInsertRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//�濡 ���ٴ� ��û�� ����
	int room;
	packet >> room;

	{
		SSynchronize Sync( this );
		
		//�켱 ���ȣ�� ���� �޾� �� ����
		m_myRoom = m_roomMgr->FindRoom( room );
		if( m_myRoom == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::RecvCreateRoom()\n[ĳ���� %s] %d�� �� ���忡 �����߽��ϴ�.\n\n"),
							m_myCharInfo->GetID(), room );
			SendResultInsert(-1);
			return;
		}

		//���� ���� ���� �غ����̰ų� �ο��� �� á���� �� �� ����
		if( !m_myRoom->CanInsert() )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::RecvCreateRoom()\n[ĳ���� %s] %d�� ���� ���� �� �� ���� ���� �Դϴ�.\n\n"),
							m_myCharInfo->GetID(), room );
			SendResultInsert(-1);
		}

		//���� �������� ���� �Ѵٸ� ����
		m_myRoom->AddPlayerInRoom( m_myCharInfo );

		//�κ񸮽�Ʈ������ �� ������ ���� �ش�.
		m_lobbyMgr->MinusUser( m_myCharInfo );

	}
#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvInsertRoom()\n%s(SessionId:%d��) %d�� ������ ��\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, room );

#endif

	//�켱 ���⿡ �����ߴٴ� ��Ŷ�� ������
	SendResultInsert( 1 );

	//�κ�� �濡 ����� ���ٴ� ���� �˸���.
	SendInsertRoom();

	//�׸��� �濡 �ִ� ����鿡�� �� ������ ������.
	SendRoomMyInfoToOtherChar();

	//������ �濡 �ִ� ��� ������� ������ ������.
	SendRoomOtherCharInfo();

	//������ ������ ������ �˸���.
	SendRoomLeader();
}

void LobbySession::RecvOutRoom()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[���Ϲ�ȣ%d] ĳ���͸� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		SendResultInsert(-1);
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
						m_myCharInfo->GetID() );
		SendResultInsert(-1);
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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvOutRoom()\n%s(sessionId:%d) %d�� ���� �����ϴ�\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );
#endif

	if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
	{
		//�濡 ��� ���� ���� �����ش�
		m_roomMgr->CloseRoom( m_myRoom->GetRoomNum() );
		SendCloseRoom( m_myRoom->GetRoomNum() );
	}

	//Ŭ��鿡�� ���� ��������� ������
	SendRoomCharOut();

	//�������� �������� �ʱ�ȭ
	m_myRoom = NULL;
	m_myCharInfo->SetTeam(-1);
}

void LobbySession::RecvReady()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n[���Ϲ�ȣ%d] ĳ���͸� ������ �������� �ʽ��ϴ�.\n\n"),
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
						_T("LobbySession::RecvRoomStartGame()\n%d������ �̹� �Ϲ� ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	m_myCharInfo->SetReady( m_myCharInfo->GetReady() == ROOM_READY_NON ? ROOM_READY_OK : ROOM_READY_NON );

	//�濡 ready���ڸ� ������ �ش�.
	m_myRoom->ChangReadyCount( m_myCharInfo->GetReady() );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvReady()\n")
					  _T("%s(sessionId:%d) Ready���°� %s���� ����Ǿ����ϴ�.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myCharInfo->GetReady() ? _T("����") : _T("�غ���") );
#endif

	SendRoomCharReady();

}

void LobbySession::RecvTeamChange()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[���Ϲ�ȣ%d] ĳ���͸� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvTeamChange()\n")
					  _T("%s(sessionId:%d) ���� %s���� �����մϴ�.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("������") : _T("������") );
#endif

	//���� �ٲٰ�
	m_myCharInfo->SetTeam( team );
	//�濡 ��ī��Ʈ ����
	m_myRoom->ChangeTeam( team );

	SendRoomTeamChange();
}

void LobbySession::RecvChat( SPacket& packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvChat()\n[���Ϲ�ȣ%d] ĳ���͸� ������ �������� �ʽ��ϴ�.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvChat()\n[ĳ���� %s] �� ������ �������� �ʽ��ϴ�.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	int size;
	TCHAR chatText[80] = {0,};
	
	packet >> size;
	packet.GetData( chatText, size );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvChat()\n")
												_T("[%s] %s\n\n")
												, m_myCharInfo->GetID()
												, chatText );

#endif

	SendChat( chatText );
}

void LobbySession::RecvRoomStartGame()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[���Ϲ�ȣ%d] ĳ���͸� ������ �������� �ʽ��ϴ�.\n\n"),
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

	//���� ���°� �븻�� �ƴϸ� ����
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d������ �̹� �Ϲ� ���°� �ƴմϴ�.\n\n"),
						m_myRoom->GetRoomNum() );
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
		//���� ��Ŷ�� ������
		SendStartGameResult();
		return;
	}

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

	int count = m_myRoom->GetPlayerCount();

	sendPacket << m_myRoom->GetRoomNum();		//���ȣ
	sendPacket << count;						//���ο���

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

	//������ �� ���
	m_lobbyMgr->PackageDataAllInLobby( sendPacket );

	//������ ��Ŷ�� ������!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendOtherCharInfo()\n")
														_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOtherCharInfo()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

#endif

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//�� ������ ��´�
	sendPacket << 1;

	m_myCharInfo->PackageMyInfo( sendPacket );

	//��ο��� ����( ���� ���� ������! )
	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendMyCharInfo()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomInfo()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_ROOMINFO );
	//�� ����
	sendPacket << m_roomMgr->OpenRoomCount();

	//��� �� ������ ��´�
	m_roomMgr->PackageOpenRoomInfo( sendPacket );

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomInfo()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomInfo()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

#endif

	return TRUE;
}

BOOL LobbySession::SendResultCreate( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_CREATE );
	sendPacket << result;
	
	//����� �����̸� �������� �־� �ش�
	{
		SSynchronize Sync( this );
		if( result > 0 )
			sendPacket << m_myCharInfo->GetTeam();
	}

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultCreate()\n")
														_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultCreate()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

#endif
	return TRUE;
}

BOOL LobbySession::SendOpenRoom( int room, TCHAR* title, int titleSize )
{
	//���� ������ ����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OPEN_ROOM );
	sendPacket << room;
	sendPacket << titleSize;
	sendPacket.PutData( title, titleSize );

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOpenRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendCloseRoom( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CLOSE_ROOM );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendCloseRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendResultInsert( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_INSERT );
	sendPacket << result;

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultInsert()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultInsert()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

#endif

	return TRUE;
}

BOOL LobbySession::SendInsertRoom()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_INSERT_ROOM );
	{
		SSynchronize Sync( this );
		sendPacket << m_myCharInfo->GetSessionID();
		sendPacket << m_myRoom->GetRoomNum();
	}

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendInsertRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomOtherCharInfo()
{
	//������ �濡 �ִ� ��� ����� ������ ������( �� ��������...)
	//������ ��Ŷ�� �����.
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	//����� �ִ��� �ִ´�
	sendPacket << m_myRoom->GetPlayerCount();

	m_myRoom->PackagePlayerInRoom( sendPacket );

	//������ ������!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomOtherCharInfo()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomOtherCharInfo()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID());

#endif

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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_myRoom->GetRoomNum() );

#endif

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
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeader()\n")
												_T("[To. %s] ���۵Ǿ����ϴ�.\n\n")
												, m_myCharInfo->GetID() );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll( int leader )
{
	//������ �ٲ�� ��ο��� ���� ��ȣ�� �����ش�
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << leader;

	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_myRoom->GetRoomNum() );

#endif

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
	m_myRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::SendRoomCharOut()\n")
					_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n"),
					m_myRoom->GetRoomNum() );

#endif

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
	}
	//--------------------------------------------------------------

	return TRUE;
}

BOOL LobbySession::SendLobbyRoomCharOut( int room )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << room;

	//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::SendRoomCharOut()\n")
					_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

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

BOOL LobbySession::SendRoomTeamChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_TEAM_CHANGE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetTeam();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendChat( TCHAR* chat )
{
	//ID�� ä�� ������ �ִ´�.
	TCHAR tmpText[100]={0,};
	wsprintf( tmpText, _T("[%s] %s\n"), m_myCharInfo->GetID(), chat );

	//��Ŷ�� �����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CHAT );
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

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif
		return TRUE;
	}
	else
	{

		//�����ױ��� ������ �Ѵ�.
		m_myRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
													, m_myRoom->GetRoomNum() );

#endif

	}

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
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_GAME_START );

	//���Ӽ����� ������ ������
	int size = strlen( m_document->GameSrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->GameSrvIP, size );
	sendPacket << m_document->GameSrvPortNum;

	//�濡 �ִ� player�鿡�� ����!
	Room* tmpRoom = m_roomMgr->FindRoom( roomNum );
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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendPlayerDisconnect()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}
