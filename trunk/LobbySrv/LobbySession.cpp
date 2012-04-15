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
	//sessionMgr에 저장하고
	SSession::OnCreate();

	//연결확인 패킷을 보내준다.
	SendPacket( SC_LOBBY_CONNECT_OK );
}

void LobbySession::OnDestroy()
{
	if( !IsPlayNow )
	{
		//세션의 IsPlayNow가 FALSE이면 그냥 다 지워 준다
		if( m_roomNo == 0 )			//lobby에 있는 사람이면
			GetLobbyMgr.MinusUser( this );
		//else
			//
		
	}
	//else
		//세션의 IsPlayNow가 TRUE이면 방에 있는 list는 지우면 안된다.

	//모두에게 나간것을 알려줘야 한다
	SendPlayerDisconnect();

	//Mgr에서는 지워준다.
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

//내 정보를 넘겨 받은 패킷에 넣는다.
//==============================================================
void LobbySession::PackageMyInfo( SPacket& packet )
{
	packet << m_SessionId;
	int size = _tcslen( m_tstrId )*sizeof(TCHAR);
	packet << size;
	packet.PutData( m_tstrId, size );
}

//받은 패킷 처리 함수
//==============================================================

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	//자신의 정보를 보냈겠지...
	//정보 해석
	int size;
	
	packet >> m_SessionId;
	packet >> size;
	packet.GetData( m_tstrId, size );
	packet >> m_roomNo;

	//방번호를 확인해서 제 접속인지를 보자
	if( m_roomNo > 0 )
	{
		//--------------------------------------
		// 재 접속
		//--------------------------------------
		//해당 방을 받아 와서 
		//그 방에 이놈의 세션 번호의 iocp핸들값을 바꿔 준다.

		//return;
	}
	//--------------------------------------
	// 새로 접속
	//--------------------------------------
	//로비list에 나를 추가
	GetLobbyMgr.AddUser( this );

	//방 정보를 보낸다.
	//SendRoomInfo();

	//그리고 로비의 유저 정보를 보낸다.( 내 정보 포함 )
	SendOtherCharInfo();

	//로비의 유저들에게 내 정보를 보낸다.
	//모두에게 전송( 나는 빼고 보낸다! )
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

//보내는 함수
//==============================================================

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//현재 lobby에 있는 인원의 수를 담고
	sendPacket << (int)GetLobbyMgr.GetPlayerCount();

	//정보를 다 담고
	GetLobbyMgr.PackageDataAllInLobby( sendPacket );

	//나에게 패킷을 보낸다!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
		return FALSE;

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//내 정보를 담는다.
	int size = 1;
	sendPacket << size;		//내 정보 하나
	sendPacket << m_SessionId;
	size = _tcslen(m_tstrId) * sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( m_tstrId, size );

	//모두에게 전송( 나는 빼고 보낸다! )
	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

	return TRUE;
}

BOOL LobbySession::SendRoomInfo()
{
	//아직 없음
	return TRUE;
}
///우선 여까지...///////////////////////////////////////////////////////////////////////
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

