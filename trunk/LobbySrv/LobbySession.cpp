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
			{
				GetRoomMgr.CloseRoom( m_roomNo );

				//방이 닫혔다는 정보를 로비의 사람들에게 보낸다.
				//==============================================================
				//==============================================================
			}
		}
		else
		{
			//게임시작으로 인한 접속 종료이면
			//방에 있는 list는 지우면 안된다.
			//==============================================================
			//==============================================================
		}
	}

	//모두에게 나간것을 알려줘야 한다
	SendPlayerDisconnect();

	//session clear
	clear();

	//Mgr에서는 지워준다.
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

//내 정보를 넘겨 받은 패킷에 넣는다.
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
		// 재 접속 //방으로
		//--------------------------------------
		//팀정보까지 받는다.

		//해당 방을 받아 와서 
		//그 방에 이놈의 세션 번호의 iocp핸들값을 바꿔 준다.
		
		//==============================================================
		//==============================================================
	}
	else
	{
		//--------------------------------------
		// 새로 접속 //로비로
		//--------------------------------------
		//로비list에 나를 추가
		GetLobbyMgr.AddUser( this );


		//방 정보를 보낸다.
		SendRoomInfo();

		//그리고 로비의 유저 정보를 보낸다.( 내 정보 포함 )
		SendOtherCharInfo();

		//로비의 유저들에게 내 정보를 보낸다.
		//모두에게 전송( 나는 빼고 보낸다! )
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
		//방 생성 실패
		SendResultCreate( -1 );
		return;
	}

	//방으로 들어갔으니 로비에서는 지워 준다
	GetLobbyMgr.MinusUser( this );

	//방만들기 성공
	SendResultCreate( 1 );

	//모두에게 방이 만들어졌다는 패킷을 보낸다.
	SendOpenRoom( title, size );

	//모두에게 내가 방에 들어갔다는 패킷을 보낸다.
	SendInsertRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//방에 들어간다는 요청을 받음
	int room;
	packet >> room;

	//우선 방번호로 방을 받아 와 본다
	Room *tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
		SendResultInsert(-1);

	//방이 존재 한다면 넣어 보자
	m_team = tmpRoom->AddPlayerInRoom( m_SessionId, m_iIocpKey );
	if( m_team < 0 )
		SendResultInsert(-1);

	//내 방번호를 바꿔 준다.
	m_roomNo = room;

	//로비리스트에서의 내 정보를 지워 준다.
	GetLobbyMgr.MinusUser( this );

	//우선 들어가기에 성공했다는 패킷을 보낸다
	SendResultInsert( 1 );

	//로비에 방에 사람이 들어갔다는 것을 알린다.
	SendInsertRoom();

	//그리고 방에 있는 사람들에게 내 정보를 보낸다.
	SendRoomMyInfoToOtherChar();

	//나에게 방에 있는 모든 사람들의 정보를 보낸다.
	SendRoomOtherCharInfo();

	//나에게 방장의 정보를 알린다.
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvReady()\n방 정보가 존재하지 않습니다.\n\n") );
		return;
	}

	//방의 ready상태를 바꾸고
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvTeamChange()\n방 정보가 존재하지 않습니다.\n\n") );
		return;
	}

	//방의 ready상태를 바꾸고
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
	//나에게 방에 있는 사람 모두의 정보를 보낸다( 내 정보까지...)
	//전송할 패킷을 만든다.
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomMyInfoToOtherChar()\n받아온 room이 NULL값입니다.\n\n") );
		return FALSE;
	}

	tmpRoom->PackageAllPlayerInRoom( sendPacket );

	//나에게 보낸다!
	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomMyInfoToOtherChar()
{
	//내 정보를 방에 있는 모든 사람들에게 보낸다
	//전송할패킷을 만든다
	SPacket	sendPacket;

	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	sendPacket << 1;			//count

	sendPacket << m_SessionId;	//sessionID
	int size = _tcslen( m_tstrId )*sizeof(TCHAR);
	sendPacket << size;			//id크기
	sendPacket.PutData( m_tstrId, size );
	sendPacket << m_team;

	//우선 방을 받아 온다
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomMyInfoToOtherChar()\n받아온 room이 NULL값입니다.\n\n") );
		return FALSE;
	}

	//모두에게 보내되 나에겐 보내지 않는다.
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

	return TRUE;
}

BOOL LobbySession::SendRoomLeader()
{
	//방장의 세션 번호를 보낸다.
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n방 정보를 받아 오지 못했습니다.\n\n") );
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
	//방장이 바뀌어 모두에게 세션 번호를 보내준다
// 	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
// 	if( tmpRoom == NULL )
// 	{
// 		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n방 정보를 받아 오지 못했습니다.\n\n") );
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
	//방에 보내줄 packet 
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_OUT );
	sendPacket << m_SessionId;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
		return FALSE;
	//방에는 내가 나갔다는 신호를 보내주고
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

	//////////////////////////////////////////////////////////////////////////
	//로비에 보내줄 packet
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << m_roomNo;

	//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

	//////////////////////////////////////////////////////////////////////////
	//정보를 초기화
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
		//로비로보낸다
		return TRUE;
	}
	else
	{
		//내가 속한 방에 보낸다
		Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
		if( tmpRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendChat()\n방을 찾을 수 없습니다.\n\n") );
			return FALSE;
		}
		//나한테까지 보내야 한다.
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
// 		//방에는 내가 나갔다는 신호를 보내주고
// 		tmpRoom->SendPacketAllInRoom( sendPacket, this );
// 
// 		sendPacket.PacketClear();
// 		sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
// 		sendPacket << m_roomNo;
// 
// 		//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
// 		GetLobbyMgr.SendPacketAllInLobby( sendPacket );
		SendRoomCharOut();
	}

	return TRUE;
}

