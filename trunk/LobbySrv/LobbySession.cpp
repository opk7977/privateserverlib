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
	//sessionMgr에 저장하고
	SSession::OnCreate();

	//연결확인 패킷을 보내준다.
	SendPacket( SC_LOBBY_CONNECT_OK );
}

void LobbySession::OnDestroy()
{
	if( m_roomNo == 0 )
	{
		//로비에 있던 애면 그냥 로비에서 지워주면 됨
		GetLobbyMgr.MinusUser( this );
	}
	else
	{
		//방에 있던 애이고
		if( !IsPlayNow )
		{
			//게임중이 아니면
			Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
			if( tmpRoom == NULL )
			{
				GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::OnDestroy()\n방으 존재하지 않습니다.\n\n") );
				return;
			}
			if( !tmpRoom->DelPlayerInRoom( m_SessionId ) )
				GetRoomMgr.CloseRoom( m_roomNo );
		}
		else
		{
			//게임시작으로 인한 접속 종료이면
			//방에 있는 list는 지우면 안된다.
		}
	}

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
	case CS_LOBBY_CREATE_ROOM:
		RecvCreateRoom( packet );
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

	//로비list에 나를 추가
	GetLobbyMgr.AddUser( this );

	//방번호를 확인해서 제 접속인지를 보자
	if( m_roomNo > 0 )
	{
		//--------------------------------------
		// 재 접속
		//--------------------------------------
		//해당 방을 받아 와서 
		//그 방에 이놈의 세션 번호의 iocp핸들값을 바꿔 준다.
	}
	//방 정보를 보낸다.
	SendRoomInfo();

	//그리고 로비의 유저 정보를 보낸다.( 내 정보 포함 )
	SendOtherCharInfo();

	//로비의 유저들에게 내 정보를 보낸다.
	//모두에게 전송( 나는 빼고 보낸다! )
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
		//방 생성 실패
		SendResultCreate( -1 );
		return;
	}

	//방으로 들어갔으니 로비에서는 지워 준다
	GetLobbyMgr.MinusUser( this );

	//방을 찾고
	Room* tmpRoom;
	tmpRoom = GetRoomMgr.FindRoom( m_roomNo );

	//팀 배정받읍시다//
	m_team = tmpRoom->GetTeam();

	//방만들기 성공
	SendResultCreate( 1 );

	//모두에게 방이 만들어졌다는 패킷을 보낸다.
	//SendOpenRoom( title, size );

	//모두에게 내가 방에 들어갔다는 패킷을 보낸다.
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
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_ROOMINFO );
	//방 개수
	sendPacket << ROOMCOUNT;

	//모든 방 정보를 담는다
	GetRoomMgr.PackageRoomInfoAll( sendPacket );

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendResultCreate( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_CREATE );
	sendPacket << result;
	
	//결과가 성공이면 팀정보를 넣어 준다
	if( result > 0 )
		sendPacket << m_team;

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendOpenRoom( TCHAR* title, int titleSize )
{
	//방의 정보를 담자
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

