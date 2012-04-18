#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "LobbyMgr.h"
#include "SLogger.h"

#include "Room.h"


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
				//==============================================================
				//==============================================================
			}
		}
		else
		{
			//���ӽ������� ���� ���� �����̸�
			//�濡 �ִ� list�� ����� �ȵȴ�.
			//==============================================================
			//==============================================================
		}
	}

	//��ο��� �������� �˷���� �Ѵ�
	SendPlayerDisconnect();

	//session clear
	clear();

	//Mgr������ �����ش�.
	SSession::OnDestroy();
}

void LobbySession::clear()
{
	IsPlayNow = FALSE;
	m_SessionId = 0;
	ZeroMemory( m_tstrId, 30 );
	m_roomNo = 0;
	m_team = -1;
	
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
	
	packet >> m_SessionId;
	packet >> size;
	packet.GetData( m_tstrId, size );
	packet >> m_roomNo;

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
	int size;
	TCHAR title[50];

	packet >> m_roomNo;
	packet >> size;
	packet.GetData( title, size );

	m_team = GetRoomMgr.OpenRoom( m_roomNo, m_SessionId, m_iIocpKey, title );

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
	SendOpenRoom( title, size );

	//��ο��� ���� �濡 ���ٴ� ��Ŷ�� ������.
	SendInsertRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//�濡 ���ٴ� ��û�� ����
	int room;
	packet >> room;

	//�켱 ���ȣ�� ���� �޾� �� ����
	Room *tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
		SendResultInsert(-1);

	//���� ���� �Ѵٸ� �־� ����
	m_team = tmpRoom->AddPlayerInRoom( m_SessionId, m_iIocpKey );
	if( m_team < 0 )
		SendResultInsert(-1);

	//�� ���ȣ�� �ٲ� �ش�.
	m_roomNo = room;

	//�κ񸮽�Ʈ������ �� ������ ���� �ش�.
	GetLobbyMgr.MinusUser( this );

	//�켱 ���⿡ �����ߴٴ� ��Ŷ�� ������
	SendResultInsert( 1 );

	//�κ� �濡 ����� ���ٴ� ���� �˸���.
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
//////////////////////////////////////////////////////////////////////////
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

	//���� ready���¸� �ٲٰ�
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

	SendChat( chatText );
}

//������ �Լ�
//==============================================================

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//���� lobby�� �ִ� �ο��� ���� ���
	sendPacket << (int)GetLobbyMgr.GetPlayerCount();

	//������ �� ���
	GetLobbyMgr.PackageDataAllInLobby( sendPacket );

	//������ ��Ŷ�� ������!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
		return FALSE;

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//�� ������ ��´�.
	int size = 1;
	sendPacket << size;		//�� ���� �ϳ�
	sendPacket << m_SessionId;
	size = _tcslen(m_tstrId) * sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( m_tstrId, size );

	//��ο��� ����( ���� ���� ������! )
	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

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

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendResultCreate( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_CREATE );
	sendPacket << result;
	
	//����� �����̸� �������� �־� �ش�
	if( result > 0 )
		sendPacket << m_team;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendOpenRoom( TCHAR* title, int titleSize )
{
	//���� ������ ����
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OPEN_ROOM );
	sendPacket << m_roomNo;
	sendPacket << titleSize;
	sendPacket.PutData( title, titleSize );

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

	return TRUE;
}

BOOL LobbySession::SendResultInsert( int result )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_ROOM_RESULT_INSERT );
	sendPacket << result;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendInsertRoom()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_INSERT_ROOM );
	sendPacket << m_SessionId;
	sendPacket << m_roomNo;

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

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
	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomMyInfoToOtherChar()
{
	//�� ������ �濡 �ִ� ��� ����鿡�� ������
	//��������Ŷ�� �����
	SPacket	sendPacket;

	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	sendPacket << 1;			//count

	sendPacket << m_SessionId;	//sessionID
	int size = _tcslen( m_tstrId )*sizeof(TCHAR);
	sendPacket << size;			//idũ��
	sendPacket.PutData( m_tstrId, size );
	sendPacket << m_team;

	//�켱 ���� �޾� �´�
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomMyInfoToOtherChar()\n�޾ƿ� room�� NULL���Դϴ�.\n\n") );
		return FALSE;
	}

	//��ο��� ������ ������ ������ �ʴ´�.
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

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

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll()
{
	//������ �ٲ�� ��ο��� ���� ��ȣ�� �����ش�
// 	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
// 	if( tmpRoom == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n�� ������ �޾� ���� ���߽��ϴ�.\n\n") );
// 		return FALSE;
// 	}
// 
// 	int leader = tmpRoom->GetLeader();
// 
// 	SPacket sendPacket;
// 	sendPacket.SetID( SC_ROOM_LEADER );
// 	sendPacket << leader;
// 
// 	tmpRoom->

	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
	//�濡 ������ packet 
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_OUT );
	sendPacket << m_SessionId;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
		return FALSE;
	//�濡�� ���� �����ٴ� ��ȣ�� �����ְ�
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

	//////////////////////////////////////////////////////////////////////////
	//�κ� ������ packet
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << m_roomNo;

	//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

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
	}

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << m_SessionId;

	if( m_roomNo == 0 )
		GetLobbyMgr.SendPacketAllInLobby(sendPacket, this );
	else
	{
// 		Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
// 		if( tmpRoom == NULL )
// 			return FALSE;
// 		//�濡�� ���� �����ٴ� ��ȣ�� �����ְ�
// 		tmpRoom->SendPacketAllInRoom( sendPacket, this );
// 
// 		sendPacket.PacketClear();
// 		sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
// 		sendPacket << m_roomNo;
// 
// 		//�κ��� ����鿡�� ����濡 ����� �پ��ٴ� ��Ŷ�� ������.
// 		GetLobbyMgr.SendPacketAllInLobby( sendPacket );
		SendRoomCharOut();
	}

	return TRUE;
}

