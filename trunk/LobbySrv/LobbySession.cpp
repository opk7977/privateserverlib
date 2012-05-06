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
	//sessionMgr에 저장하고
	SSession::OnCreate();

	//연결확인 패킷을 보내준다.
	SendPacket( SC_LOBBY_CONNECT_OK );
}

void LobbySession::OnDestroy()
{
	if( m_myCharInfo == NULL )
	{
		//캐릭터 정보가 없다면 서버 혹은 클라에서 자신의 정보를 보내기 전에 끈경우
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::OnDestroy()\n 캐릭터 정보가 없는 session의 연결을 끊습니다.\n\n") );
	}
	else
	{
		if( m_myRoom == NULL )
		{
			//로비에 있는 user
			//그냥 로비에서 삭제
			GetLobbyMgr.MinusUser( m_myCharInfo );

			//DB에 있는 user정보에 Logout체크를 해 준다
			GetDB.UpdateLogin( m_myCharInfo->GetSessionID(), FALSE );

			//모두에게 알린다
			SendPlayerDisconnect();
		}
		else 
		{
			//방에 있던 user
			if( !m_myCharInfo->GetIsPlay() )
			{
				//게임중이 아님
				//DB에 있는 user정보에 Logout체크를 해 준다
				GetDB.UpdateLogin( m_myCharInfo->GetSessionID(), FALSE );

				//방에서 지워준다
				if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
				{
					//방에 남아있는 인원이 없음
					//방 닫고
					int room = m_myRoom->GetRoomNum();
					GetRoomMgr.CloseRoom( room );

					//방이 닫혔다는 정보를 로비의 사람들에게 보낸다.
					SendCloseRoom( room );
				}

				//내가 나갔다는 것을 방 안의 user들에게 알림
				SendRoomCharOut();
			}
			//게임중이면 지워주지 않아도 되니
			//따로 작업할것은 없다
		}

		//캐릭터 공간을 지워 준다
		//게임중인 캐릭터라면 지워주지 않고 그냥 return된다
		GetCharMgr.ReturnCharSpace( m_myCharInfo );

	}

	//session clear
	clear();

	//Mgr에서는 지워준다.
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

//패킷을 해석해서 지정된 함수를 호출한다.
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
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvConnectServer()\n게임서버와 연결되었습니다.\n\n") );
	GetSrvNet.SetSession( this );
}

void LobbySession::RecvGameStart( SPacket& packet )
{
	int room;
	packet >> room;

	//해당 번호에 맞는 방을 찾아 온다

	//그 방을 Play상태로 바꾼다
	
	//로비의 사람들에게 해당 방이 게임중이 되었다고 알린다

	//방의 player들에게 게임서버로 이동하라는 패킷을 보낸다.
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
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvPlayerDiconnectInGame()\n방이 존재하지 않습니다.\n\n") );
		return;
	}

	if( !tmpRoom->DelPlayerInRoom( session ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvPlayerDiconnectInGame()\n방에 해당 캐릭터가 존재하지 않습니다.\n\n") );
	}

	//로비에 있는 사람들에게 1명 나갔다는 표시만 보내준다.
	///////////////////////////////////////////////////////////////////////////

	//로비에 보내줄 packet
	SPacket	sendPacket;
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << tmpRoom->GetRoomNum();

	//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

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

	//방번호를 확인해서 제 접속인지를 보자
	if( roomNum > 0 )
	{
		SSynchronize Sync( this );

		//--------------------------------------
		// 재 접속 //방으로
		//--------------------------------------
		//캐릭터 공간을 받아 온다
		m_myCharInfo = GetCharMgr.FindCharAsSessionId( sessionId );
		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvInsertLobby()\n%d번에 해당하는 캐릭터의 정보가 존재하지 않습니다.\n\n"), sessionId );
			return;
		}
		//해당 캐릭터의 session정보를 다시 설정
		m_myCharInfo->SetSession( this );

		//방 공간도 다시 받아 온다 
		m_myRoom = GetRoomMgr.FindRoom( roomNum );
		if( m_myRoom == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvInsertLobby()\n%d번에 해당하는 방이 존재하지 않습니다.\n\n"), roomNum );
			return;
		}
	}
	else
	{
		//--------------------------------------
		// 새로 접속 //로비로
		//--------------------------------------

		//캐릭터 공간을 하나 얻어 온다
		m_myCharInfo = GetCharMgr.GetCharSPace();
		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::RecvInsertLobby()\n더이상 캐릭터를 수용할 수 있는 캐릭터 공간이 존재하지 않습니다.\n\n") );
			//////////////////////////////////////////////////////////////////////////
			// 더이상 수용할 수 없다는 패킷을 보낸다
			//
			//
			//
			//////////////////////////////////////////////////////////////////////////
			return;
		}
		// DB에서 정보를 찾는다.
		//TCHAR _id[30] = {0,};
		//GetDB.GetData( sessionId, _id );
		GetDB.GetData( sessionId, m_myCharInfo->GetID() );
		m_myCharInfo->SetSessionID( sessionId );
		//해당 캐릭터의 session정보를 다시 설정
		m_myCharInfo->SetSession( this );

		//로비list에 나를 추가
		GetLobbyMgr.AddUser( m_myCharInfo );

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
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvCreateRoom()\n방이 열기에 실패했습니다.\n\n") );
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
												_T("Room : %d번\n")
												_T("RoomTitle : %s\n\n")
												, m_myCharInfo->GetSessionID()
												, m_myCharInfo->GetID()
												, m_myRoom->GetRoomNum()
												, title );

#endif

	//방으로 들어갔으니 로비에서는 지워 준다
	GetLobbyMgr.MinusUser( m_myCharInfo );

	//방만들기 성공
	SendResultCreate( 1 );

	//모두에게 방이 만들어졌다는 패킷을 보낸다.
	SendOpenRoom( m_myRoom->GetRoomNum(), title, size );

	//모두에게 내가 방에 들어갔다는 패킷을 보낸다.
	SendInsertRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//방에 들어간다는 요청을 받음
	int room;
	packet >> room;

	{
		SSynchronize Sync( this );
		
		//우선 방번호로 방을 받아 와 본다
		m_myRoom = GetRoomMgr.FindRoom( room );
		if( m_myRoom == NULL )
		{
			SendResultInsert(-1);
			return;
		}

		//방이 존재 한다면 넣자
		m_myRoom->AddPlayerInRoom( m_myCharInfo );

		//로비리스트에서의 내 정보를 지워 준다.
		GetLobbyMgr.MinusUser( m_myCharInfo );

	}
#ifdef _DEBUG 
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvInsertRoom()\n%s(SessionId:%d번) %d번 방으로 들어감\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, room );

#endif

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
}

void LobbySession::RecvOutRoom()
{

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvOutRoom()\n%s(sessionId:%d) %d번 방을 나갑니다\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );

#endif

	if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
	{
		//방에 사람 없음 방 지워준다
		GetRoomMgr.CloseRoom( m_myRoom->GetRoomNum() );
		SendCloseRoom( m_myRoom->GetRoomNum() );
	}

	//클라들에게 누가 사라졌는지 보낸다
	SendRoomCharOut();

	{
		SSynchronize Sync( this );

		//방정보와 팀정보를 초기화
		m_myRoom = NULL;
		m_myCharInfo->SetTeam(-1);
	}
}

void LobbySession::RecvReady()
{
	m_myCharInfo->SetReady( m_myCharInfo->GetReady() ? FALSE : TRUE );

	//방에 ready숫자를 변경해 준다.
	m_myRoom->ChangReadyState( m_myCharInfo->GetReady() );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvReady()\n")
					  _T("%s(sessionId:%d) Ready상태가 %s으로 변경되었습니다.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myCharInfo->GetReady() ? _T("시작") : _T("준비중") );

#endif

	SendRoomCharReady();

}

void LobbySession::RecvTeamChange()
{
	//공격팀이면 수비로
	//수비팀이면 공격으로
	int team = ( m_myCharInfo->GetTeam() == 0 ) ? 1 : 0;
	/*packet >> team;*/

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO
					, _T("LobbySession::RecvTeamChange()\n")
					  _T("%s(sessionId:%d) 팀을 %s으로 변경합니다.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == 0 ) ? _T("공격팀") : _T("수비팀") );
#endif

	//팀을 바꾸고
	m_myCharInfo->SetTeam( team );
	//방에 팀카운트 수정
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
	//우선 애가 방장이 맞는지 확인
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		//실패 패킷을 보낸다
		//SC_ROOM_START_RESULT
		return;
	}
	
	//방이 시작 가능 상태인지 확인
	if( !m_myRoom->PossiblePlay() )
	{
		//실패 패킷을 보낸다
		//SC_ROOM_START_RESULT
		return;
	}

	//게임서버로 정보를 보낸다
	SendCreateGameProc();

	//로비에 방이 게임중이라는 표시를 보낸다
	//SC_ROOM_GAME_START
	
	//게임을 시작해야 하는 사람들에게 게임시작을 알림
	//SC_ROOM_GAME_START

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
	//sendPacket.PacketClear();
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
												, m_myCharInfo->GetID());

#endif

	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//내 정보를 담는다
	sendPacket << 1;

	m_myCharInfo->PackageMyInfo( sendPacket );

	//모두에게 전송( 나는 빼고 보낸다! )
	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

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
	GetRoomMgr.PackageOpenRoomInfo( sendPacket );

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
												, m_myCharInfo->GetID());

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
			sendPacket << m_myCharInfo->GetTeam();
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
												, m_myCharInfo->GetID());

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

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

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

	GetLobbyMgr.SendPacketAllInLobby( sendPacket, m_myCharInfo );

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

	//몇명이 있는지 넣는다
	sendPacket << m_myRoom->GetPlayerCount();

	m_myRoom->PackagePlayerInRoom( sendPacket );

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
												, m_myCharInfo->GetID());

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

	//내 정보를 담는다
	m_myCharInfo->PackageMyInfoForRoom( sendPacket );

	//모두에게 보내되 나에겐 보내지 않는다.
	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
												_T("[All In No.%d Room] 전송되었습니다.\n\n")
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
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeader()\n")
												_T("[To. %s] 전송되었습니다.\n\n")
												, m_myCharInfo->GetID() );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll( int leader )
{
	//방장이 바뀌어 모두에게 세션 번호를 보내준다
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_LEADER );
	sendPacket << leader;

	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
												_T("[All In No.%d Room] 전송되었습니다.\n\n")
												, m_myRoom->GetRoomNum() );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
	//////////////////////////////////////////////////////////////////////////
	//방에 보내줄 packet 
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_CHAR_OUT );
	sendPacket << m_myCharInfo->GetSessionID();

	//방에는 내가 나갔다는 신호를 보내주고
	m_myRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In No.%d Room] 전송되었습니다.\n\n")
												, m_myRoom->GetRoomNum() );

#endif

	//--------------------------------------------------------------
	// 내가 나가고 방의 leader가 바뀌었다면 바뀐 정보를 보내줘야 한다.
	//이 녀석이 leader인지 확인한다.
	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		//leader를 바꾸고 패킷을 보내준다.
		SendRoomLeaderToAll( m_myRoom->ChangeLeader()->GetSessionID() );
	}
	//--------------------------------------------------------------

	//////////////////////////////////////////////////////////////////////////
	//로비에 보내줄 packet
	sendPacket.PacketClear();
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << m_myRoom->GetRoomNum();

	//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
	GetLobbyMgr.SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomCharOut()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

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
	//ID와 채팅 문장을 넣는다.
	TCHAR tmpText[100]={0,};
	wsprintf( tmpText, _T("[%s] %s\n"), m_myCharInfo->GetID(), chat );

	//패킷을 만들고
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CHAT );
	if( m_myRoom == NULL )
	{
		//로비에 있으면 0을 넣어 준다
		sendPacket << 0;
	}
	else
	{
		//방에 있으면 방번호를 넣어 준다
		sendPacket << m_myRoom->GetRoomNum();
	}

	int size = _tcslen( tmpText )*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( tmpText, size );
	//////////////////////////////////////////////////////////////////////////

	if( m_myRoom == NULL )
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

		//나한테까지 보내야 한다.
		m_myRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In No.%d Room] 전송되었습니다.\n\n")
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
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

	return TRUE;
}
