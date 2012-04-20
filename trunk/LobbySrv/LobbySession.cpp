#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "LobbyMgr.h"
#include "SLogger.h"

#include "Room.h"

/*#include "SSynchronize.h"*/


SIMPLEMENT_DYNAMIC(LobbySession)
SIMPLEMENT_DYNCREATE(LobbySession)

LobbySession::LobbySession(void) : IsPlayNow(FALSE)
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
	if( m_roomNo == 0 )
	{
		//�κ� �ִ� �ָ� �׳� �κ񿡼� �����ָ� ��
		GetLobbyMgr.MinusUser( this );

		SendPlayerDisconnect();
	}
	else
	{
		//�濡 �ִ� ���̰�
		if( !IsPlayNow )
		{
			//�������� �ƴϸ�
			Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
			if( tmpRoom == NULL )
			{
				GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::OnDestroy()\n���� �������� �ʽ��ϴ�.\n\n") );
				return;
			}
			if( !tmpRoom->DelPlayerInRoom( m_SessionId ) )
			{
				GetRoomMgr.CloseRoom( m_roomNo );

				//���� �����ٴ� ������ �κ��� ����鿡�� ������.
				SendCloseRoom( m_roomNo );
			}

			SendRoomCharOut();
		}
		else
		{
			//���ӽ������� ���� ���� �����̸�
			//�濡 �ִ� list�� ����� �ȵȴ�.
			//==============================================================
			//==============================================================
		}
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

		IsPlayNow = FALSE;
		m_SessionId = 0;
		ZeroMemory( m_tstrId, 30 );
		m_roomNo = 0;
		m_team = -1;
	}
}


void LobbySession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	//==============================================================> GameSrv

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
		RecvReady( packet );
		break;

	case CS_ROOM_TEAM_CHANGE:
		RecvTeamChange( packet );
		break;
	case CS_LOBBY_CHAT:
		RecvChat( packet );
		break;
	}
}

//�� ������ �Ѱ� ���� ��Ŷ�� �ִ´�.
//==============================================================
void LobbySession::PackageMyInfo( SPacket& packet, BOOL isTeam /*= FALSE*/ )
{
	packet << m_SessionId;
	int size = _tcslen( m_tstrId )*sizeof(TCHAR);
	packet << size;
	packet.PutData( m_tstrId, size );

	if( isTeam )
		packet << m_team;
}

//���� ��Ŷ ó�� �Լ�
//==============================================================

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	//�ڽ��� ������ ���°���...
	//���� �ؼ�
	int size;
	{
		SSynchronize Sync( this );

		packet >> m_SessionId;
		packet >> size;
		packet.GetData( m_tstrId, size );
		packet >> m_roomNo;
	}

#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvInsertLobby()\n")
													_T("sessionID : %d\n")
													_T("ID : %s\n")
													_T("Room : %d\n\n")
													, m_SessionId
													, m_tstrId
													, m_roomNo );

#endif

	//���ȣ�� Ȯ���ؼ� �� ���������� ����
	if( m_roomNo > 0 )
	{
		//--------------------------------------
		// �� ���� //������
		//--------------------------------------
		//���������� �޴´�.

		//�ش� ���� �޾� �ͼ� 
		//�� �濡 �̳��� ���� ��ȣ�� iocp�ڵ鰪�� �ٲ� �ش�.
		
		//==============================================================
		//==============================================================
	}
	else
	{
		//--------------------------------------
		// ���� ���� //�κ��
		//--------------------------------------
		//�κ�list�� ���� �߰�
		GetLobbyMgr.AddUser( this );


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
	int size, room;
	TCHAR title[50];

	{
		SSynchronize Sync( this );
		packet >> room;	
		packet >> size;
		packet.GetData( title, size );

		m_roomNo = room;
		m_team = GetRoomMgr.OpenRoom( m_roomNo, m_SessionId, m_iIocpKey, title );
	}

	if( m_team < 0 )
	{
		//�� ���� ����
		SendResultCreate( -1 );
		return;
	}

	//������ ������ �κ񿡼��� ���� �ش�
	GetLobbyMgr.MinusUser( this );

	//�游��� ����
	SendResultCreate( 1 );

	//��ο��� ���� ��������ٴ� ��Ŷ�� ������.
	SendOpenRoom( room, title, size );

	//��ο��� ���� �濡 ���ٴ� ��Ŷ�� ������.
	SendInsertRoom();

#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvCreateRoom()\n")
												_T("sessionID : %d\n")
												_T("ID : %s\n")
												_T("Room : %d")
												_T("RoomTitle : %s\n\n")
												, m_SessionId
												, m_tstrId
												, m_roomNo
												, title );

#endif
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//�濡 ���ٴ� ��û�� ����
	int room;
	packet >> room;

	
	//�켱 ���ȣ�� ���� �޾� �� ����
	Room *tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
	{
		SendResultInsert(-1);
		return;
	}

	//���� ���� �Ѵٸ� �־� ����
	m_team = tmpRoom->AddPlayerInRoom( m_SessionId, m_iIocpKey );
	if( m_team < 0 )
	{
		SendResultInsert(-1);
		return;
	}

	{
		SSynchronize Sync( this );
		//�� ���ȣ�� �ٲ� �ش�.
		m_roomNo = room;
	}

	//�κ񸮽�Ʈ������ �� ������ ���� �ش�.
	GetLobbyMgr.MinusUser( this );

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

#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvInsertRoom()\n")
												_T("sessionID : %d\n")
												_T("ID : %s\n")
												_T("InRoom : %d�� ��\n\n")
												, m_SessionId
												, m_tstrId
												, m_roomNo );

#endif
}

void LobbySession::RecvOutRoom()
{
//////////////////////////////////////////////////////////////////////////
	//�濡�� ĳ���͸� ������ �ְ�
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvOutRoom()\n�� ������ �������� �ʽ��ϴ�.\n\n") );
		return;
	}

	if( !tmpRoom->DelPlayerInRoom( m_SessionId ) )
	{
		//�濡 ��� ���� �� �����ش�
		GetRoomMgr.CloseRoom( m_roomNo );
		SendCloseRoom( m_roomNo );
	}

	//Ŭ��鿡�� ���� ��������� ������
	SendRoomCharOut();

	{
		SSynchronize Sync( this );
		//�������� �������� �ʱ�ȭ
		m_roomNo = 0;
		m_team = -1;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvOutRoom()\n")
												_T("sessionID : %d\n")
												_T("ID : %s\n\n")
												, m_SessionId
												, m_tstrId );

#endif
}

void LobbySession::RecvReady( SPacket& packet )
{
	int ready;
	packet >> ready;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvReady()\n�� ������ �������� �ʽ��ϴ�.\n\n") );
		return;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvReady()\n")
												_T("sessionID : %d\n")
												_T("ID : %s")
												_T("Ready���°� %s���� �ٲ�ϴ�.\n\n")
												, m_SessionId
												, m_tstrId
												, ( ready == 0 ) ? _T("�غ���") : _T("����") );

#endif

	//���� ready���¸� �ٲٰ�
	tmpRoom->ChangeReadyState( (BOOL)ready );

	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_READY );
	sendPacket << m_SessionId;
	sendPacket << ready;

	tmpRoom->SendPacketAllInRoom( sendPacket );
}

void LobbySession::RecvTeamChange( SPacket& packet )
{
	int team;
	packet >> team;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvTeamChange()\n�� ������ �������� �ʽ��ϴ�.\n\n") );
		return;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvTeamChange()\n")
												_T("sessionID : %d\n")
												_T("ID : %s")
												_T("���� %s���� �ٲ�ϴ�.\n\n")
												, m_SessionId
												, m_tstrId
												, ( team == 0 ) ? _T("������") : _T("������") );

#endif


	//���� �ٲٰ�
	tmpRoom->TeamChange( m_team );

	m_team = team;

	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_TEAM_CHANGE );
	sendPacket << m_SessionId;
	sendPacket << m_team;

	tmpRoom->SendPacketAllInRoom( sendPacket );
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
												, m_tstrId
												, chatText );

#endif

	SendChat( chatText );
}

//������ �Լ�
//==============================================================

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	sendPacket.PacketClear();
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
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//�� ������ ��´�.
	int size = 1;

	{
		SSynchronize Sync( this );
		sendPacket << size;		//�� ���� �ϳ�
		sendPacket << m_SessionId;
		size = _tcslen(m_tstrId) * sizeof(TCHAR);
		sendPacket << size;
		sendPacket.PutData( m_tstrId, size );
	}
	//��ο��� ����( ���� ���� ������! )
	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

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
	GetRoomMgr.PackageRoomInfoAll( sendPacket );

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
												, m_tstrId);

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
			sendPacket << m_team;
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
												, m_tstrId);

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

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

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

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

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
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendInsertRoom()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_INSERT_ROOM );
	{
		SSynchronize Sync( this );
		sendPacket << m_SessionId;
		sendPacket << m_roomNo;
	}

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendInsertRoom()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

// BOOL LobbySession::SendCharInsert()
// {
// 	return TRUE;
// }

BOOL LobbySession::SendRoomOtherCharInfo()
{
	//������ �濡 �ִ� ��� ����� ������ ������( �� ��������...)
	//������ ��Ŷ�� �����.
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomMyInfoToOtherChar()\n�޾ƿ� room�� NULL���Դϴ�.\n\n") );
		return FALSE;
	}

	tmpRoom->PackageAllPlayerInRoom( sendPacket );

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
												, m_tstrId);

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

	{
		SSynchronize Sync( this );
		sendPacket << m_SessionId;	//sessionID
		int size = _tcslen( m_tstrId )*sizeof(TCHAR);
		sendPacket << size;			//idũ��
		sendPacket.PutData( m_tstrId, size );
		sendPacket << m_team;
	}

	//�켱 ���� �޾� �´�
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomMyInfoToOtherChar()\n�޾ƿ� room�� NULL���Դϴ�.\n\n") );
		return FALSE;
	}

	//��ο��� ������ ������ ������ �ʴ´�.
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_roomNo );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeader()
{
	//������ ���� ��ȣ�� ������.
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n�� ������ �޾� ���� ���߽��ϴ�.\n\n") );
		return FALSE;
	}

	int leader = tmpRoom->GetLeader();

	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << leader;

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
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll( int leader )
{
	//������ �ٲ�� ��ο��� ���� ��ȣ�� �����ش�

	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << leader;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeaderToAll()\n�� ������ �޾� ���� ���߽��ϴ�.\n\n") );
		return FALSE;
	}

	if( tmpRoom == NULL )
		return FALSE;

	tmpRoom->SendPacketAllInRoom( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_roomNo );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
	//////////////////////////////////////////////////////////////////////////
	//�濡 ������ packet 
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_OUT );
	sendPacket << m_SessionId;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
		return FALSE;
	//�濡�� ���� �����ٴ� ��ȣ�� �����ְ�
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
												, m_roomNo );

#endif

	//--------------------------------------------------------------
	// ���� leader�� �ٲ���ٸ� �ٲ� ������ ������� �Ѵ�.
	//�� �༮�� leader���� Ȯ���Ѵ�.
	if( tmpRoom->GetLeader() == m_SessionId )
	{
		//leader�� �ٲٰ� ��Ŷ�� �����ش�.
		SendRoomLeaderToAll( tmpRoom->ChangeLeader() );
	}
	//--------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	//�κ� ������ packet
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << m_roomNo;

	//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif
	//////////////////////////////////////////////////////////////////////////
	//������ �ʱ�ȭ
	IsPlayNow = FALSE;
	m_roomNo = 0;
	m_team = -1;

	return TRUE;
}

BOOL LobbySession::SendRoomCharReady()
{
	return TRUE;
}

BOOL LobbySession::SendRoomTeamChange()
{
	return TRUE;
}

BOOL LobbySession::SendChat( TCHAR* chat )
{
	TCHAR tmpText[100]={0,};
	wsprintf( tmpText, _T("[%s] %s\n"), m_tstrId, chat );

	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CHAT );

	sendPacket << m_roomNo;
	int size = _tcslen( tmpText )*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( tmpText, size );

	if( m_roomNo <= 0 )
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
		//���� ���� �濡 ������
		Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
		if( tmpRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendChat()\n���� ã�� �� �����ϴ�.\n\n") );
			return FALSE;
		}
		//�����ױ��� ������ �Ѵ�.
		tmpRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In No.%d Room] ���۵Ǿ����ϴ�.\n\n")
													, m_roomNo );

#endif

	}

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << m_SessionId;

	GetLobbyMgr.SendPacketAllInLobby(sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendPlayerDisconnect()\n")
												_T("[All In Lobby] ���۵Ǿ����ϴ�.\n\n") );

#endif

	return TRUE;
}

