#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "LobbyMgr.h"

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
	if( !IsPlayNow )
	{
		//������ IsPlayNow�� FALSE�̸� �׳� �� ���� �ش�
		if( m_roomNo == 0 )			//lobby�� �ִ� ����̸�
			GetLobbyMgr.MinusUser( this );
		//else
			//
		
	}
	//else
		//������ IsPlayNow�� TRUE�̸� �濡 �ִ� list�� ����� �ȵȴ�.

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

	//���ȣ�� Ȯ���ؼ� �� ���������� ����
	if( m_roomNo > 0 )
	{
		//--------------------------------------
		// �� ����
		//--------------------------------------
		//�ش� ���� �޾� �ͼ� 
		//�� �濡 �̳��� ���� ��ȣ�� iocp�ڵ鰪�� �ٲ� �ش�.

		//return;
	}
	//--------------------------------------
	// ���� ����
	//--------------------------------------
	//�κ�list�� ���� �߰�
	GetLobbyMgr.AddUser( this );

	//�� ������ ������.
	//SendRoomInfo();

	//�׸��� �κ��� ���� ������ ������.( �� ���� ���� )
	SendOtherCharInfo();

	//�κ��� �����鿡�� �� ������ ������.
	//��ο��� ����( ���� ���� ������! )
	SendMyCharInfo();
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{

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
	//���� ����
	return TRUE;
}
///�켱 ������...///////////////////////////////////////////////////////////////////////
BOOL LobbySession::SendResultCreate()
{
	return TRUE;
}

BOOL LobbySession::SendOpenRoom()
{
	return TRUE;
}

BOOL LobbySession::SendResultInsert()
{
	return TRUE;
}

BOOL LobbySession::SendInsertRoom()
{
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

	GetLobbyMgr.SendPacketAllInLobby(sendPacket, this );

	return TRUE;
}

