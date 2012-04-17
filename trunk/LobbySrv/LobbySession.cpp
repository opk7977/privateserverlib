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
				GetRoomMgr.CloseRoom( m_roomNo );
		}
		else
		{
			//���ӽ������� ���� ���� �����̸�
			//�濡 �ִ� list�� ����� �ȵȴ�.
		}
	}

	//��ο��� �������� �˷���� �Ѵ�
	SendPlayerDisconnect();

	//Mgr������ �����ش�.
	SSession::OnDestroy();
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
	}
}

//�� ������ �Ѱ� ���� ��Ŷ�� �ִ´�.
//==============================================================
void LobbySession::PackageMyInfo( SPacket& packet )
{
	packet << m_SessionId;
	int size = _tcslen( m_tstrId )*sizeof(TCHAR);
	packet << size;
	packet.PutData( m_tstrId, size );
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

	//�κ�list�� ���� �߰�
	GetLobbyMgr.AddUser( this );

	//���ȣ�� Ȯ���ؼ� �� ���������� ����
	if( m_roomNo > 0 )
	{
		//--------------------------------------
		// �� ����
		//--------------------------------------
		//�ش� ���� �޾� �ͼ� 
		//�� �濡 �̳��� ���� ��ȣ�� iocp�ڵ鰪�� �ٲ� �ش�.
	}
	//�� ������ ������.
	SendRoomInfo();

	//�׸��� �κ��� ���� ������ ������.( �� ���� ���� )
	SendOtherCharInfo();

	//�κ��� �����鿡�� �� ������ ������.
	//��ο��� ����( ���� ���� ������! )
	SendMyCharInfo();
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{
	int size;
	TCHAR title[50];

	packet >> m_roomNo;
	packet >> size;
	packet.GetData( title, size );

	if( GetRoomMgr.OpenRoom( m_roomNo, m_SessionId, m_iIocpKey, title ) == NULL )
	{
		//�� ���� ����
		SendResultCreate( -1 );
		return;
	}

	//������ ������ �κ񿡼��� ���� �ش�
	GetLobbyMgr.MinusUser( this );

	//���� ã��
	Room* tmpRoom;
	tmpRoom = GetRoomMgr.FindRoom( m_roomNo );

	//�� ���������ô�//
	m_team = tmpRoom->GetTeam();

	//�游��� ����
	SendResultCreate( 1 );

	//��ο��� ���� ��������ٴ� ��Ŷ�� ������.
	//SendOpenRoom( title, size );

	//��ο��� ���� �濡 ���ٴ� ��Ŷ�� ������.
	//SendInsertRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{

}

void LobbySession::RecvOutRoom()
{

}

void LobbySession::RecvReady( SPacket& packet )
{

}

void LobbySession::RecvTeamChange( SPacket& packet )
{

}

void LobbySession::RecvPlay()
{

}

void LobbySession::RecvChat( SPacket& packet )
{

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
	sendPacket.PutData( title, titleSize );

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

	return TRUE;
}

BOOL LobbySession::SendResultInsert()
{
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

BOOL LobbySession::SendCharInsert()
{
	return TRUE;
}

BOOL LobbySession::SendRoomOtherCharInfo()
{
	return TRUE;
}

BOOL LobbySession::SendRoomLeader()
{
	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
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

BOOL LobbySession::SendRoomChat()
{
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
		Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
		if( tmpRoom == NULL )
			return FALSE;
		tmpRoom->SendPacketAllInRoom( sendPacket, this );
	}

	return TRUE;
}

