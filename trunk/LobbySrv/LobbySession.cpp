#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "LobbyMgr.h"
#include "SLogger.h"

#include "Room.h"
#include "LobbyChar.h"

#include "CheckDB.h"

#include "SrvNet.h"

SIMPLEMENT_DYNAMIC(LobbySession)
SIMPLEMENT_DYNCREATE(LobbySession)

LobbySession::LobbySession(void)// : IsPlayNow(FALSE)
: m_myCharInfo(NULL)
, m_myRoom(NULL)
{
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
	if( m_myCharInfo == NULL )
	{
		//ĳ���� ������ ���ٸ� ���� Ȥ�� Ŭ�󿡼� �ڽ��� ������ ������ ���� �����
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::OnDestroy()\n ĳ���� ������ ���� session�� ������ �����ϴ�.\n\n") );
	}
	else
	{
		if( m_myRoom == NULL )
		{
			//�κ� �ִ� user
			//�׳� �κ񿡼� ����
			GetLobbyMgr.MinusUser( m_myCharInfo );

			//DB�� �ִ� user������ Logoutüũ�� �� �ش�
			GetDB.UpdateLogin( m_myCharInfo->GetSessionID(), FALSE );

			//��ο��� �˸���
			SendPlayerDisconnect();
		}
		else 
		{
			//�濡 �ִ� user
			if( !m_myCharInfo->GetIsPlay() )
			{
				//�������� �ƴ�
				//DB�� �ִ� user������ Logoutüũ�� �� �ش�
				GetDB.UpdateLogin( m_myCharInfo->GetSessionID(), FALSE );

				//�濡�� �����ش�
				if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
				{
					//�濡 �����ִ� �ο��� ����
					//�� �ݰ�
					int room = m_myRoom->GetRoomNum();
					GetRoomMgr.CloseRoom( room );

					//���� �����ٴ� ������ �κ��� ����鿡�� ������.
					SendCloseRoom( room );
				}

				//���� �����ٴ� ���� �� ���� user�鿡�� �˸�
				SendRoomCharOut();
			}
			//�������̸� �������� �ʾƵ� �Ǵ�
			//���� �۾��Ұ��� ����
		}

		//ĳ���� ������ ���� �ش�
		//�������� ĳ���Ͷ�� �������� �ʰ� �׳� return�ȴ�
		GetCharMgr.ReturnCharSpace( m_myCharInfo );

	}

	//session clear
	clear();

	//Mgr������ �����ش�.
	SSession::OnDestroy();
}

void LobbySession::clear()
{
	{
		SSynchronize Sync( this );

		m_myCharInfo = NULL;
		m_myRoom = NULL;
	}
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
	}
}


//���� ��Ŷ ó�� �Լ�
//==============================================================

void LobbySession::RecvConnectServer()
{
	if( GetSrvNet.GetSession() != NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvConnectServer()\n���������� �̹� �������ϴ�. �� ������...\n\n") );
		return;
	}
	//���� ������!!
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvConnectServer()\n���Ӽ����� ����Ǿ����ϴ�.\n\n") );
	GetSrvNet.SetSession( this );
}

void LobbySession::RecvGameStart( SPacket& packet )
{
	int room;
	packet >> room;

	//�ش� ��ȣ�� �´� ���� ã�� �´�

	//�� ���� Play���·� �ٲ۴�
	
	//�κ��� ����鿡�� �ش� ���� �������� �Ǿ��ٰ� �˸���

	//���� player�鿡�� ���Ӽ����� �̵��϶�� ��Ŷ�� ������.
}

void LobbySession::RecvPlayerDiconnectInGame( SPacket& packet )
{
	int room, session;
	packet >> room;
	packet >> session;

	//�濡�� ����� ���� �ְ�
	Room* tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvPlayerDiconnectInGame()\n���� �������� �ʽ��ϴ�.\n\n") );
		return;
	}

	if( !tmpRoom->DelPlayerInRoom( session ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvPlayerDiconnectInGame()\n�濡 �ش� ĳ���Ͱ� �������� �ʽ��ϴ�.\n\n") );
	}

	//�κ� �ִ� ����鿡�� 1�� �����ٴ� ǥ�ø� �����ش�.
	///////////////////////////////////////////////////////////////////////////

	//�κ� ������ packet
	SPacket	sendPacket;
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << tmpRoom->GetRoomNum();

	//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

}

//--------------------------------------------------------------

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	int sessionId, roomNum;
	packet >> sessionId;
	packet >> roomNum;

#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvInsertLobby()\n")
													_T("sessionID : %d\n")
													_T("Room : %d\n\n")
													, sessionId
													, roomNum );
#endif

	//���ȣ�� Ȯ���ؼ� �� ���������� ����
	if( roomNum > 0 )
	{
		SSynchronize Sync( this );

		//--------------------------------------
		// �� ���� //������
		//--------------------------------------
		//ĳ���� ������ �޾� �´�
		m_myCharInfo = GetCharMgr.FindCharAsSessionId( sessionId );
		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvInsertLobby()\n%d���� �ش��ϴ� ĳ������ ������ �������� �ʽ��ϴ�.\n\n"), sessionId );
			return;
		}
		//�ش� ĳ������ session������ �ٽ� ����
		m_myCharInfo->SetSession( this );

		//�� ������ �ٽ� �޾� �´� 
		m_myRoom = GetRoomMgr.FindRoom( roomNum );
		if( m_myRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvInsertLobby()\n%d���� �ش��ϴ� ���� �������� �ʽ��ϴ�.\n\n"), roomNum );
			return;
		}
	}
	else
	{
		//--------------------------------------
		// ���� ���� //�κ��
		//--------------------------------------

		//ĳ���� ������ �ϳ� ��� �´�
		m_myCharInfo = GetCharMgr.GetCharSPace();
		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvInsertLobby()\n���̻� ĳ���͸� ������ �� �ִ� ĳ���� ������ �������� �ʽ��ϴ�.\n\n") );
			//////////////////////////////////////////////////////////////////////////
			// ���̻� ������ �� ���ٴ� ��Ŷ�� ������
			//
			//
			//
			//////////////////////////////////////////////////////////////////////////
			return;
		}
		// DB���� ������ ã�´�.
		//TCHAR _id[30] = {0,};
		//GetDB.GetData( sessionId, _id );
		GetDB.GetData( sessionId, m_myCharInfo->GetID() );
		m_myCharInfo->SetSessionID( sessionId );
		//�ش� ĳ������ session������ �ٽ� ����
		m_myCharInfo->SetSession( this );

		//�κ�list�� ���� �߰�
		GetLobbyMgr.AddUser( m_myCharInfo );

		//�� ������ ������.
		SendRoomInfo();

		//�׸��� �κ��� ���� ������ ������.( �� ���� ���� )
		SendOtherCharInfo();

		//�κ��� �����鿡�� �� ������ ������.
		//��ο��� ����( ���� ���� ������! )
		SendMyCharInfo();
	}
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{
	//int room, size;
	int size;
	TCHAR title[50];

	{
		SSynchronize Sync( this );
		//packet >> room;	
		packet >> size;
		packet.GetData( title, size );

		m_myRoom = GetRoomMgr.OpenRoom( title );
		if( m_myRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvCreateRoom()\n���� ���⿡ �����߽��ϴ�.\n\n") );
			SendResultCreate( -1 );
			return;
		}
		m_myRoom->AddPlayerInRoom( m_myCharInfo );
		m_myRoom->SetLeader( m_myCharInfo );
	}

#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvCreateRoom()\n")
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
	GetLobbyMgr.MinusUser( m_myCharInfo );

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
		m_myRoom = GetRoomMgr.FindRoom( room );
		if( m_myRoom == NULL )
		{
			SendResultInsert(-1);
			return;
		}

		//���� ���� �Ѵٸ� ����
		m_myRoom->AddPlayerInRoom( m_myCharInfo );

		//�κ񸮽�Ʈ������ �� ������ ���� �ش�.
		GetLobbyMgr.MinusUser( m_myCharInfo );

	}
#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
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

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvOutRoom()\n%s(sessionId:%d) %d�� ���� �����ϴ�\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );

#endif

	if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
	{
		//�濡 ��� ���� �� �����ش�
		GetRoomMgr.CloseRoom( m_myRoom->GetRoomNum() );
		SendCloseRoom( m_myRoom->GetRoomNum() );
	}

	//Ŭ��鿡�� ���� ��������� ������
	SendRoomCharOut();

	{
		SSynchronize Sync( this );

		//�������� �������� �ʱ�ȭ
		m_myRoom = NULL;
		m_myCharInfo->SetTeam(-1);
	}
}

void LobbySession::RecvReady()
{
	m_myCharInfo->SetReady( m_myCharInfo->GetReady() ? FALSE : TRUE );

	//�濡 ready���ڸ� ������ �ش�.
	m_myRoom->ChangReadyState( m_myCharInfo->GetReady() );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
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
	//�������̸� �����
	//�������̸� ��������
	int team = ( m_myCharInfo->GetTeam() == 0 ) ? 1 : 0;
	/*packet >> team;*/

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvTeamChange()\n")
					  _T("%s(sessionId:%d) ���� %s���� �����մϴ�.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == 0 ) ? _T("������") : _T("������") );
#endif

	//���� �ٲٰ�
	m_myCharInfo->SetTeam( team );
	//�濡 ��ī��Ʈ ����
	m_myRoom->ChangeTeam( team );

	SendRoomTeamChange();
}

void LobbySession::RecvPlay()
{

}

void LobbySession::RecvChat( SPacket& packet )
{
	int size;
	TCHAR chatText[80] = {0,};
	
	packet >> size;
	packet.GetData( chatText, size );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvChat()\n")
												_T("[%s] %s\n\n")
												, m_myCharInfo->GetID()
												, chatText );

#endif

	SendChat( chatText );
}

void LobbySession::RecvRoomStartGame()
{
	//�켱 �ְ� ������ �´��� Ȯ��
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		//���� ��Ŷ�� ������
		//SC_ROOM_START_RESULT
		return;
	}
	
	//���� ���� ���� �������� Ȯ��
	if( !m_myRoom->PossiblePlay() )
	{
		//���� ��Ŷ�� ������
		//SC_ROOM_START_RESULT
		return;
	}

	//���Ӽ����� ������ ������
	SendCreateGameProc();

	//�κ� ���� �������̶�� ǥ�ø� ������
	//SC_ROOM_GAME_START
	
	//������ �����ؾ� �ϴ� ����鿡�� ���ӽ����� �˸�
	//SC_ROOM_GAME_START

}


//������ �Լ�
//==============================================================

BOOL LobbySession::SendCreateGameProc()
{
	return TRUE;
}

//--------------------------------------------------------------

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	//sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//������ �� ���
	GetLobbyMgr.PackageDataAllInLobby( sendPacket );

	//������ ��Ŷ�� ������!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendOtherCharInfo()\n")
														_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOtherCharInfo()\n")
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
	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendMyCharInfo()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomInfo()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_ROOMINFO );
	//�� ����
	sendPacket << ROOMCOUNT;

	//��� �� ������ ��´�
	GetRoomMgr.PackageOpenRoomInfo( sendPacket );

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomInfo()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomInfo()\n")
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultCreate()\n")
														_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultCreate()\n")
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

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOpenRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendCloseRoom( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CLOSE_ROOM );
	sendPacket << roomNum;

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendCloseRoom()\n")
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultInsert()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultInsert()\n")
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

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendInsertRoom()\n")
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomOtherCharInfo()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomOtherCharInfo()\n")
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
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n")
													_T("����ũ��� ��Ŷ�� ũ�Ⱑ �ٸ��ϴ�.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeader()\n")
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
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
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
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_myRoom->GetRoomNum() );

#endif

	//--------------------------------------------------------------
	// ���� ������ ���� leader�� �ٲ���ٸ� �ٲ� ������ ������� �Ѵ�.
	//�� �༮�� leader���� Ȯ���Ѵ�.
	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		//leader�� �ٲٰ� ��Ŷ�� �����ش�.
		SendRoomLeaderToAll( m_myRoom->ChangeLeader()->GetSessionID() );
	}
	//--------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	//�κ� ������ packet
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << m_myRoom->GetRoomNum();

	//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
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
		GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif
		return TRUE;
	}
	else
	{

		//�����ױ��� ������ �Ѵ�.
		m_myRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
													, m_myRoom->GetRoomNum() );

#endif

	}

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << m_myCharInfo->GetSessionID();

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendPlayerDisconnect()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}
