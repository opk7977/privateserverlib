#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "LobbyMgr.h"
#include "SLogger.h"

#include "Room.h"
#include "CheckDB.h"

#include "SrvNet.h"

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

		//DB에 있는 애를 Logout 체크해준다
		GetDB.UpdateLogin( m_SessionId, FALSE );

		SendPlayerDisconnect();
	}
	else if( m_roomNo > 0 )
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

			//DB에 있는 애를 Logout 체크해준다
			GetDB.UpdateLogin( m_SessionId, FALSE );

			//방에서 지워 준다
			if( !tmpRoom->DelPlayerInRoom( m_SessionId ) )
			{
				//방에 인원이 이제 없음!
				//방 닫고
				GetRoomMgr.CloseRoom( m_roomNo );

				//방이 닫혔다는 정보를 로비의 사람들에게 보낸다.
				SendCloseRoom( m_roomNo );
			}

			SendRoomCharOut();
		}
		//게임시작으로 인한 접속 종료이면
		//방에 있는 list를 지우면 안된다.
	}
	else
	{
		//m_roomNo이 0보다 작은 경우는 Srv들인경우이다....
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::OnDestroy()\n 게임 서버와의 연결을 끊습니다.\n\n") );
	}
	//그냥 지워주면 된다.

	//session clear
	clear();

	//Mgr에서는 지워준다.
	SSession::OnDestroy();
}

void LobbySession::clear()
{
	{
		SSynchronize Sync( this );

		IsPlayNow = FALSE;
		m_SessionId = 0;
		ZeroMemory( m_tstrId, 30 );
		m_roomNo = -1;
		m_team = -1;
	}
}


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
		RecvReady( packet );
		break;
	case CS_ROOM_TEAM_CHANGE:
		RecvTeamChange( packet );
		break;
	case CS_LOBBY_CHAT:
		RecvChat( packet );
		break;
	case CS_ROOM_START:
		//
		break;
	}
}

//내 정보를 넘겨 받은 패킷에 넣는다.
//==============================================================
void LobbySession::PackageMyInfo( SPacket& packet/*, BOOL isTeam*/ /*= FALSE*/ )
{
	packet << m_SessionId;
	int size = _tcslen( m_tstrId )*sizeof(TCHAR);
	packet << size;
	packet.PutData( m_tstrId, size );
}

//받은 패킷 처리 함수
//==============================================================

void LobbySession::RecvConnectServer()
{
	if( GetSrvNet.GetSession() != NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvConnectServer()\n서버설정이 이미 끝났습니다. 넌 누구냐...\n\n") );
		return;
	}
	//내가 서버다!!
	//서버는 방번호를 -1로 통일
	m_roomNo = -1;

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvConnectServer()\n게임서버와 연결되었습니다.\n\n") );
	GetSrvNet.SetSession( this );
}

void LobbySession::RecvPlayerDiconnectInGame( SPacket& packet )
{
	int room, session;
	packet >> room;
	packet >> session;

	//방에서 사람을 지워 주고
	Room* tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::OnDestroy()\n방으 존재하지 않습니다.\n\n") );
		return;
	}
	tmpRoom->DelPlayerInRoom( session );

	//로비에 있는 사람들에게 1명 나갔다는 표시만 보내준다.

}

//--------------------------------------------------------------

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	//자신의 정보를 보냈겠지...
	{
		SSynchronize Sync( this );

		packet >> m_SessionId;
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

	//방번호를 확인해서 제 접속인지를 보자
	if( m_roomNo > 0 )
	{
		//--------------------------------------
		// 재 접속 //방으로
		//--------------------------------------
		//해당 방을 받아 와서 
		Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
		if( tmpRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvInsertLobby()\n방이 존재하지 않습니다.\n\n") );
			return;
		}
		
		//그 방에 이놈의 세션 번호의 iocp핸들값을 바꿔 준다.
		tmpRoom->SetPlayerIocp( m_SessionId, m_iIocpKey );
	}
	else
	{
		//--------------------------------------
		// 새로 접속 //로비로
		//--------------------------------------
		// DB에서 정보를 찾는다.
		ZeroMemory( m_tstrId, 30 );
		GetDB.GetData( m_SessionId, m_tstrId );
		

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

	{
		SSynchronize Sync( this );
		packet >> m_roomNo;	
		packet >> size;
		packet.GetData( title, size );

		Room* tmpRoom = GetRoomMgr.OpenRoom( m_roomNo, /*m_SessionId, m_iIocpKey,*/ title );
		if( tmpRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvCreateRoom()\n방이 열기에 실패했습니다.\n\n") );
			SendResultCreate( -1 );
			return;
		}
		m_team = tmpRoom->AddPlayerInRoom( m_SessionId, m_iIocpKey, m_tstrId );
		tmpRoom->SetLeader( m_SessionId );
	}

	//방으로 들어갔으니 로비에서는 지워 준다
	GetLobbyMgr.MinusUser( this );

	//방만들기 성공
	SendResultCreate( 1 );

	//모두에게 방이 만들어졌다는 패킷을 보낸다.
	SendOpenRoom( m_roomNo, title, size );

	//모두에게 내가 방에 들어갔다는 패킷을 보낸다.
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
	//방에 들어간다는 요청을 받음
	int room;
	packet >> room;

	
	//우선 방번호로 방을 받아 와 본다
	Room *tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
	{
		SendResultInsert(-1);
		return;
	}

	//방이 존재 한다면 넣어 보자
	m_team = tmpRoom->AddPlayerInRoom( m_SessionId, m_iIocpKey, m_tstrId );
	if( m_team < 0 )
	{
		SendResultInsert(-1);
		return;
	}

	{
		SSynchronize Sync( this );
		//내 방번호를 바꿔 준다.
		m_roomNo = room;
	}

	//로비리스트에서의 내 정보를 지워 준다.
	GetLobbyMgr.MinusUser( this );

	//우선 들어가기에 성공했다는 패킷을 보낸다
	SendResultInsert( 1 );

	//로비로 방에 사람이 들어갔다는 것을 알린다.
	SendInsertRoom();

	//그리고 방에 있는 사람들에게 내 정보를 보낸다.
	SendRoomMyInfoToOtherChar();

	//나에게 방에 있는 모든 사람들의 정보를 보낸다.
	SendRoomOtherCharInfo();

	//나에게 방장의 정보를 알린다.
	SendRoomLeader();

#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvInsertRoom()\n")
												_T("sessionID : %d\n")
												_T("ID : %s\n")
												_T("InRoom : %d번 방\n\n")
												, m_SessionId
												, m_tstrId
												, m_roomNo );

#endif
}

void LobbySession::RecvOutRoom()
{
//////////////////////////////////////////////////////////////////////////
	//방에서 캐릭터를 제거해 주고
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvOutRoom()\n방 정보가 존재하지 않습니다.\n\n") );
		return;
	}

	if( !tmpRoom->DelPlayerInRoom( m_SessionId ) )
	{
		//방에 사람 없음 방 지워준다
		GetRoomMgr.CloseRoom( m_roomNo );
		SendCloseRoom( m_roomNo );
	}

	//클라들에게 누가 사라졌는지 보낸다
	SendRoomCharOut();

	{
		SSynchronize Sync( this );
		//방정보와 팀정보를 초기화
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
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvReady()\n방 정보가 존재하지 않습니다.\n\n") );
		return;
	}

	//방의 ready상태를 바꾸고
	int ready = tmpRoom->ChangeReadyState( m_SessionId );
	if( ready < 0 )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvReady()\nready상태를 받아 올 수 없습니다.\n\n") );
		return;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvReady()\n")
													_T("sessionID : %d\n")
													_T("ID : %s")
													_T("Ready상태가 %s으로 바뀜니다.\n\n")
													, m_SessionId
													, m_tstrId
													, ( ready == 0 ) ? _T("준비중") : _T("시작") );

#endif

	SendRoomCharReady( ready );

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

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::RecvTeamChange()\n")
												_T("sessionID : %d\n")
												_T("ID : %s")
												_T("팀을 %s으로 바뀜니다.\n\n")
												, m_SessionId
												, m_tstrId
												, ( team == 0 ) ? _T("공격팀") : _T("수비팀") );

#endif


	//팀을 바꾸고
	m_team = tmpRoom->TeamChange( m_SessionId );

	if( m_team < 0 )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvTeamChange()\n팀변화 실패, 존재하지 않는 세션입니다.\n\n") );
		return;
	}

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
												, m_tstrId
												, chatText );

#endif

	SendChat( chatText );
}

//보내는 함수
//==============================================================

BOOL LobbySession::SendCreateGameProc()
{
	return TRUE;
}

//--------------------------------------------------------------

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//정보를 다 담고
	GetLobbyMgr.PackageDataAllInLobby( sendPacket );

	//나에게 패킷을 보낸다!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendOtherCharInfo()\n")
														_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOtherCharInfo()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//내 정보를 담는다.
	int size = 1;

	{
		SSynchronize Sync( this );
		sendPacket << size;		//내 정보 하나
		sendPacket << m_SessionId;
		size = _tcslen(m_tstrId) * sizeof(TCHAR);
		sendPacket << size;
		sendPacket.PutData( m_tstrId, size );
	}
	//모두에게 전송( 나는 빼고 보낸다! )
	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendMyCharInfo()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

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

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomInfo()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomInfo()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendResultCreate( int result )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_RESULT_CREATE );
	sendPacket << result;
	
	//결과가 성공이면 팀정보를 넣어 준다
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
														_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultCreate()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
												, m_tstrId);

#endif
	return TRUE;
}

BOOL LobbySession::SendOpenRoom( int room, TCHAR* title, int titleSize )
{
	//방의 정보를 담자
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OPEN_ROOM );
	sendPacket << room;
	sendPacket << titleSize;
	sendPacket.PutData( title, titleSize );

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOpenRoom()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

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
												_T("[All In Lobby] 전송되었습니다.\n\n") );

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
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultInsert()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
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
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

	return TRUE;
}

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
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomOtherCharInfo()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomOtherCharInfo()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomMyInfoToOtherChar()
{
	//내 정보를 방에 있는 모든 사람들에게 보낸다
	//전송할패킷을 만든다
	SPacket	sendPacket;

	sendPacket.SetID( SC_ROOM_OTHER_CHARINFO );

	sendPacket << 1;			//count

	{
		SSynchronize Sync( this );
		sendPacket << m_SessionId;	//sessionID
		int size = _tcslen( m_tstrId )*sizeof(TCHAR);
		sendPacket << size;			//id크기
		sendPacket.PutData( m_tstrId, size );
		sendPacket << m_team;
		sendPacket << (int)0;
	}

	//우선 방을 받아 온다
	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomMyInfoToOtherChar()\n받아온 room이 NULL값입니다.\n\n") );
		return FALSE;
	}

	//모두에게 보내되 나에겐 보내지 않는다.
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
												_T("[All In No.%d Room] 전송되었습니다.\n\n")
												, m_roomNo );

#endif

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

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeader()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
												, m_tstrId);

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll( int leader )
{
	//방장이 바뀌어 모두에게 세션 번호를 보내준다

	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << leader;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeaderToAll()\n방 정보를 받아 오지 못했습니다.\n\n") );
		return FALSE;
	}

	if( tmpRoom == NULL )
		return FALSE;

	tmpRoom->SendPacketAllInRoom( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
												_T("[All In No.%d Room] 전송되었습니다.\n\n")
												, m_roomNo );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
	//////////////////////////////////////////////////////////////////////////
	//방에 보내줄 packet 
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_OUT );
	sendPacket << m_SessionId;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
		return FALSE;
	//방에는 내가 나갔다는 신호를 보내주고
	tmpRoom->SendPacketAllInRoom( sendPacket, this );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In No.%d Room] 전송되었습니다.\n\n")
												, m_roomNo );

#endif

	//--------------------------------------------------------------
	// 방의 leader가 바뀌었다면 바뀐 정보를 보내줘야 한다.
	//이 녀석이 leader인지 확인한다.
	if( tmpRoom->GetLeader() == m_SessionId )
	{
		//leader를 바꾸고 패킷을 보내준다.
		SendRoomLeaderToAll( tmpRoom->ChangeLeader() );
	}
	//--------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	//로비에 보내줄 packet
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << m_roomNo;

	//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif
	//////////////////////////////////////////////////////////////////////////
	//정보를 초기화
	IsPlayNow = FALSE;
	m_roomNo = 0;
	m_team = -1;

	return TRUE;
}

BOOL LobbySession::SendRoomCharReady( int ready )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_READY );
	sendPacket << m_SessionId;
	sendPacket << ready;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomCharReady()\n방 정보를 받아 오지 못했습니다.\n\n") );
		return FALSE;
	}

	tmpRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomTeamChange()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_TEAM_CHANGE );
	sendPacket << m_SessionId;
	sendPacket << m_team;

	Room* tmpRoom = GetRoomMgr.FindRoom( m_roomNo );
	if( tmpRoom == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomTeamChange()\n방 정보를 받아 오지 못했습니다.\n\n") );
		return FALSE;
	}

	tmpRoom->SendPacketAllInRoom( sendPacket );

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
		//////////////////////////////////////////////////////////////////////////
		GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif
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

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In No.%d Room] 전송되었습니다.\n\n")
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
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

	return TRUE;
}
