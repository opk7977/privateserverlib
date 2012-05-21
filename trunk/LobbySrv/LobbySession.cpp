#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "DataLeader.h"

#include "LobbyMgr.h"
#include "Room.h"
#include "LobbyChar.h"
#include "CheckDB.h"

#include "SrvNet.h"

#include "SLogger.h"


SIMPLEMENT_DYNAMIC(LobbySession)
SIMPLEMENT_DYNCREATE(LobbySession)

LobbySession::LobbySession(void)
: m_myCharInfo(NULL)
, m_myRoom(NULL)
{
	m_document	= &GetDocument;
	m_lobbyMgr	= &GetLobbyMgr;
	m_roomMgr	= &GetRoomMgr;
	m_charMgr	= &GetCharMgr;
	m_DBMgr		= &GetDB;
	m_srvNet	= &GetSrvNet;
	m_logger	= &GetLogger;
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
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		//캐릭터 정보가 없다면 서버 혹은 클라에서 자신의 정보를 보내기 전에 끈경우
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::OnDestroy()\n 캐릭터 정보가 없는 session의 연결을 끊습니다.\n\n") );
	}
	else
	{
		if( !m_myCharInfo->GetIsPlay() )
		{
			//게임중이 아님
			if( m_myRoom == NULL )
			{
				//로비에 있음
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::OnDestroy()\n캐릭터 %s님이 로비에서 연결을 끊습니다.\n\n"),
								m_myCharInfo->GetID() );

				//로비에 있는 user
				//그냥 로비에서 삭제
				m_lobbyMgr->MinusUser( m_myCharInfo );

				//모두에게 알린다
				//SendPlayerDisconnect();
			}
			else
			{
				//방에 있음
				int room = m_myRoom->GetRoomNum();

				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::OnDestroy()\n캐릭터 %s님이 %d번 방에서 연결을 끊습니다.\n\n"),
								room, m_myCharInfo->GetID() );

				//방에서 지워준다
				if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
				{
					//방에 남아있는 인원이 없음
					//방 닫고
					m_roomMgr->CloseRoom( room );

					//방이 닫혔다는 정보를 로비의 사람들에게 보낸다.
					SendCloseRoom( room );
				}
				else
				{
					//방에 사람이 있음
					//내가 나갔다는 것을 방 안의 user들에게 알림
					SendRoomCharOut();
				}
			}
			//로비의 모두에게 알린다
			SendPlayerDisconnect();

			//DB에 있는 user정보에 Logout체크를 해 준다
			m_DBMgr->UpdateLogin( m_myCharInfo->GetSessionID(), FALSE );

			//캐릭터 공간을 지워 준다
			m_charMgr->ReturnCharSpace( m_myCharInfo );
			
		}
		else
		{
			//게임 중
			//별 작업이 없음
		}
	}

	//session clear
	clear();

	//Mgr에서는 지워준다.
	SSession::OnDestroy();
}

void LobbySession::clear()
{
	SSynchronize Sync( this );

	m_myCharInfo = NULL;
	m_myRoom = NULL;
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
	case GL_START_FAILD:
		RecvStartFaild( packet );
		break;
	case GL_START_OK:
		RecvGameStart( packet );
		break;
	case GL_GAME_END:
		RecvGameEnd( packet );
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

	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::PacketParsing()\n%d번 패킷, 패킷 ID가 유효하지 않습니다.\n\n") );
	}
}


//받은 패킷 처리 함수
//==============================================================

void LobbySession::RecvConnectServer()
{
	SSynchronize Sync( this );

	//이미 연결되어 있는지를 확인하고
	if( m_srvNet->GetSession() != NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvConnectServer()\n서버설정이 이미 끝났습니다. 넌 누구냐...\n\n") );
		return;
	}
	//내가 서버다!!
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvConnectServer()\n게임서버와 연결되었습니다.\n\n") );
	m_srvNet->SetSession( this );
}

void LobbySession::RecvStartFaild( SPacket& packet )
{
	SSynchronize Sync( this );

	int room;
	packet >> room;

	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, 
						_T("LobbySession::RecvStartFaild()\n해당 방의 정보가 없습니다.\n\n") );
		return;
	}

	//넘겨 받은 방의 방장에게 게임 시작 실패 패킷을 보낸다
	tmpRoom->GetLeader()->GetSession()->SendStartGameResult();
}

void LobbySession::RecvGameStart( SPacket& packet )
{
	SSynchronize Sync( this );

	int room;
	packet >> room;

	//해당 번호에 맞는 방을 찾아 온다
	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvGameStart()\n해당 방의 정보가 없습니다.\n\n") );
		return;
	}

	//그 방을 Play상태로 바꾼다
	tmpRoom->SetPlay();

	//로비의 사람들에게 해당 방이 게임중이 되었다고 알린다
	SendLobbyGameStart( room );

	//방의 player들에게 게임서버로 이동하라는 패킷을 보낸다.
	SendStartGameInRoom( room );

	//방에 있는 list를 비워주고, 인원수를 초기화 해 준다
	//방으로 들어 오면 다시 캐릭터 정보를 갱신해야 하니 비워 준다
	tmpRoom->ListReset();
}

void LobbySession::RecvGameEnd( SPacket& packet )
{
	//게임 안에 player가 모두 나갔을때 받는 패킷이다
	//게임이 종료 되고 player가 모두 나갔다. 그러니 방을 다시 열어 준다.
	SSynchronize Sync( this );

	int room;
	packet >> room;

	//해당 번호에 맞는 방을 찾아 온다
	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvGameStart()\n해당 방의 정보가 없습니다.\n\n") );
		return;
	}

	//그 방의 play상태를 풀어 준다
	tmpRoom->SetNormal();
	
	//로비 사람들에게 방이 열렸다고 알려 준다.
	SendLobbyGameEnd( room );
}

void LobbySession::RecvPlayerDiconnectInGame( SPacket& packet )
{
	//================================================
	// 게임 프로세스에서 player가 게임을 종료 한경우
	//================================================
	SSynchronize Sync( this );

	int room, session;
	packet >> room;
	packet >> session;


	//로비에 있는 사람들에게 1명 나갔다는 표시만 보내준다.
	SendLobbyRoomCharOut( room );

	//캐릭터 로그인 정보를 갱신해 준다
	m_DBMgr->UpdateLogin( session, FALSE );

	//캐릭터 정보를 삭제 해 준다.
	m_charMgr->ReturnCharSpace( session );
}

//--------------------------------------------------------------

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	int sessionId, roomNum;
	packet >> sessionId;
	packet >> roomNum;

#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvInsertLobby()\n")
					_T("sessionID : %d\n")
					_T("Room : %d\n\n")
					, sessionId
					, roomNum );
#endif

	SSynchronize Sync( this );

	//방번호를 확인해서 제 접속인지를 보자
	if( roomNum > 0 )
	{
		//======================================
		// 방으로 재 접속
		//======================================
		//--------------------------------------
		// 캐릭터와 방의 정보를 다시 갱신
		//--------------------------------------
		//캐릭터 공간을 받아 온다
		m_myCharInfo = m_charMgr->FindCharAsSessionId( sessionId );
		if( m_myCharInfo == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::RecvInsertLobby()\n%d번에 해당하는 캐릭터의 정보가 존재하지 않습니다.\n\n"),
							sessionId );
			return;
		}
		//방 공간도 받아 온다 
		m_myRoom = m_roomMgr->FindRoom( roomNum );
		if( m_myRoom == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::RecvInsertLobby()\n%d번에 해당하는 방이 존재하지 않습니다.\n\n"),
				roomNum );
			return;
		}
		//--------------------------------------


		//해당 캐릭터의 session정보를 다시 설정
		SetSessionID( sessionId );
		m_myCharInfo->SetSession( this );

		//캐릭터의 게임 실행 상태와 ready상태를 초기화 해주자
		m_myCharInfo->SetIsPlay( FALSE );
		m_myCharInfo->SetReady( FALSE );

		//방에 나를 추가
		m_myRoom->AddPlayerInRoom( m_myCharInfo );

		//내 정보를 내 방 사람들에게 전송
		SendRoomMyInfoToOtherChar();

		//방사람들의 정보를 나에게 전송
		SendRoomOtherCharInfo();
	}
	else
	{
		if( m_myCharInfo != NULL )
		{
			//--------------------------------------
			// 방에서 로비로
			//--------------------------------------
			//로비list로 나를 다시 넣어 준다
			m_lobbyMgr->AddUser( this );

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			///////로비의 모두에게 방에서 로비로 나왔다는 패킷을 보내준다.////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			//--------------------------------------
			// 새로 접속
			//--------------------------------------
			//캐릭터 공간을 하나 얻어 온다
			m_myCharInfo = m_charMgr->GetCharSPace();
			if( m_myCharInfo == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
								_T("LobbySession::RecvInsertLobby()\n더이상 캐릭터를 수용할 수 있는 캐릭터 공간이 존재하지 않습니다.\n\n") );
				//////////////////////////////////////////////////////////////////////////
				// 더이상 수용할 수 없다는 패킷을 보낸다
				//
				//
				//
				//////////////////////////////////////////////////////////////////////////
				return;
			}

			// DB에서 정보를 찾는다.
			m_DBMgr->GetData( sessionId, m_myCharInfo->GetID() );

			//======================================
			// 내 정보 셋팅
			//======================================
			//현재의 세션에 내 ID를 넣어 준다
			SetSessionID( sessionId );
			//캐릭터 정보에 내 ID를 넣어 준다
			m_myCharInfo->SetSessionID( sessionId );
			//해당 캐릭터의 session정보를 설정
			m_myCharInfo->SetSession( this );
			//======================================

			//로비list에 나를 추가
			m_lobbyMgr->AddUser( m_myCharInfo );

			//로비의 유저들에게 내 정보를 보낸다.
			//모두에게 전송( 나는 빼고 보낸다! )
			SendMyCharInfo();
		}
		//나에게 방 정보를 보낸다.
		SendRoomInfo();

		//그리고 로비의 유저 정보를 보낸다.( 내 정보 포함 )
		SendOtherCharInfo();
	}
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{
	int size;
	TCHAR title[50];

	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[소켓번호%d] 캐릭터를 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	packet >> size;
	packet.GetData( title, size );

	m_myRoom = m_roomMgr->OpenRoom( title );
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n방이 열기에 실패했습니다.\n\n") );
		SendResultCreate( -1 );
		return;
	}
	m_myRoom->AddPlayerInRoom( m_myCharInfo );
	m_myRoom->SetLeader( m_myCharInfo );

#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::RecvCreateRoom()\n")
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
	m_lobbyMgr->MinusUser( m_myCharInfo );

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
		m_myRoom = m_roomMgr->FindRoom( room );
		if( m_myRoom == NULL )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::RecvCreateRoom()\n[캐릭터 %s] %d번 방 입장에 실패했습니다.\n\n"),
							m_myCharInfo->GetID(), room );
			SendResultInsert(-1);
			return;
		}

		//방이 지금 게임 준비중이거나 인원이 다 찼으면 들어갈 수 없다
		if( !m_myRoom->CanInsert() )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::RecvCreateRoom()\n[캐릭터 %s] %d번 방은 입장 할 수 없는 상태 입니다.\n\n"),
							m_myCharInfo->GetID(), room );
			SendResultInsert(-1);
		}

		//방이 정상으로 존재 한다면 넣자
		m_myRoom->AddPlayerInRoom( m_myCharInfo );

		//로비리스트에서의 내 정보를 지워 준다.
		m_lobbyMgr->MinusUser( m_myCharInfo );

	}
#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO
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
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[소켓번호%d] 캐릭터를 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		SendResultInsert(-1);
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
						m_myCharInfo->GetID() );
		SendResultInsert(-1);
		return;
	}

	//방의 상태가 노말이 아니면 무시
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d번방은 이미 일반 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvOutRoom()\n%s(sessionId:%d) %d번 방을 나갑니다\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );
#endif

	if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
	{
		//방에 사람 없음 방을 지워준다
		m_roomMgr->CloseRoom( m_myRoom->GetRoomNum() );
		SendCloseRoom( m_myRoom->GetRoomNum() );
	}

	//클라들에게 누가 사라졌는지 보낸다
	SendRoomCharOut();

	//방정보와 팀정보를 초기화
	m_myRoom = NULL;
	m_myCharInfo->SetTeam(-1);
}

void LobbySession::RecvReady()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n[소켓번호%d] 캐릭터를 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	//방의 상태가 노말이 아니면 무시
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d번방은 이미 일반 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	m_myCharInfo->SetReady( m_myCharInfo->GetReady() == ROOM_READY_NON ? ROOM_READY_OK : ROOM_READY_NON );

	//방에 ready숫자를 변경해 준다.
	m_myRoom->ChangReadyCount( m_myCharInfo->GetReady() );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
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
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[소켓번호%d] 캐릭터를 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	//방의 상태가 노말이 아니면 무시
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d번방은 이미 일반 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	//공격팀이면 수비로 or 수비팀이면 공격으로
	int team = ( m_myCharInfo->GetTeam() == ROOM_TEAM_ATT ) ? ROOM_TEAM_DEF : ROOM_TEAM_ATT;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvTeamChange()\n")
					  _T("%s(sessionId:%d) 팀을 %s으로 변경합니다.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("공격팀") : _T("수비팀") );
#endif

	//팀을 바꾸고
	m_myCharInfo->SetTeam( team );
	//방에 팀카운트 수정
	m_myRoom->ChangeTeam( team );

	SendRoomTeamChange();
}

void LobbySession::RecvChat( SPacket& packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvChat()\n[소켓번호%d] 캐릭터를 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvChat()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	int size;
	TCHAR chatText[80] = {0,};
	
	packet >> size;
	packet.GetData( chatText, size );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvChat()\n")
												_T("[%s] %s\n\n")
												, m_myCharInfo->GetID()
												, chatText );

#endif

	SendChat( chatText );
}

void LobbySession::RecvRoomStartGame()
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[소켓번호%d] 캐릭터를 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
						m_myCharInfo->GetID() );
		return;
	}

	//방의 상태가 노말이 아니면 무시
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d번방은 이미 일반 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	//우선 애가 방장이 맞는지 확인
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[캐릭터 %s] 캐릭터가 방장이 아닙니다.\n\n"),
						m_myCharInfo->GetID() );
		//실패 패킷을 보낸다
		SendStartGameResult();
		return;
	}
	
	//방이 시작 가능 상태인지 확인
	if( !m_myRoom->PossiblePlay() )
	{
		//실패 패킷을 보낸다
		SendStartGameResult();
		return;
	}

	//방을 게임준비중인 상태로 바꾼다
	m_myRoom->SetReady();

	//게임서버로 정보를 보낸다
	SendCreateGameProc();
}


//보내는 함수
//==============================================================

BOOL LobbySession::SendCreateGameProc()
{
	SPacket sendPacket;
	sendPacket.SetID( LG_START_GAME );
	//게임서버에 준비를 하라고 전달

	int count = m_myRoom->GetPlayerCount();

	sendPacket << m_myRoom->GetRoomNum();		//방번호
	sendPacket << count;						//방인원수

	//방에 있는 캐릭터들의 정보를 담는다
	m_myRoom->PackagePlayerInRoomForGame( sendPacket );

	//게임 서버로 전송
	m_srvNet->SendToGameServer( sendPacket );
	
	return TRUE;
}

//--------------------------------------------------------------

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_OTHER_CHARINFO );

	//정보를 다 담고
	m_lobbyMgr->PackageDataAllInLobby( sendPacket );

	//나에게 패킷을 보낸다!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendOtherCharInfo()\n")
														_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOtherCharInfo()\n")
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
	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendMyCharInfo()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendRoomInfo()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_ROOMINFO );
	//방 개수
	sendPacket << m_roomMgr->OpenRoomCount();

	//모든 방 정보를 담는다
	m_roomMgr->PackageOpenRoomInfo( sendPacket );

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomInfo()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomInfo()\n")
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
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultCreate()\n")
														_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultCreate()\n")
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

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendOpenRoom()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

	return TRUE;
}

BOOL LobbySession::SendCloseRoom( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_CLOSE_ROOM );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendCloseRoom()\n")
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
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendResultInsert()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendResultInsert()\n")
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

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendInsertRoom()\n")
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
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomOtherCharInfo()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomOtherCharInfo()\n")
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
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomMyInfoToOtherChar()\n")
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
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::SendRoomLeader()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

#endif
		return FALSE;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeader()\n")
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
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendRoomLeaderToAll()\n")
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
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::SendRoomCharOut()\n")
					_T("[All In No.%d Room] 전송되었습니다.\n\n"),
					m_myRoom->GetRoomNum() );

#endif

	//--------------------------------------------------------------
	// 내가 나가고 방의 leader가 바뀌었다면 바뀐 정보를 보내줘야 한다.
	// 이 녀석이 leader인지 확인한다.
	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		//leader를 바꾸고 패킷을 보내준다.
		SendRoomLeaderToAll( m_myRoom->ChangeLeader()->GetSessionID() );

		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
						_T("LobbySession::SendRoomCharOut()\n")
						_T("%d번 방의 %s님이 방장이 되셨습니다.\n\n"),
						m_myRoom->GetRoomNum(), m_myRoom->GetLeader()->GetID() );
	}
	//--------------------------------------------------------------

	return TRUE;
}

BOOL LobbySession::SendLobbyRoomCharOut( int room )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_LOBBY_ROOMPLAYER_COUNTDOWN );
	sendPacket << room;

	//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO,
					_T("LobbySession::SendRoomCharOut()\n")
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
		m_lobbyMgr->SendPacketAllInLobby( sendPacket );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif
		return TRUE;
	}
	else
	{

		//나한테까지 보내야 한다.
		m_myRoom->SendPacketAllInRoom( sendPacket );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendChat()\n")
													_T("[All In No.%d Room] 전송되었습니다.\n\n")
													, m_myRoom->GetRoomNum() );

#endif

	}

	return TRUE;
}

BOOL LobbySession::SendStartGameResult()
{
	//게임 스타트에 실패!
	SPacket sendPacket;

	sendPacket.SetID( SC_ROOM_START_RESULT );
	sendPacket << -1;

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendStartGameResult()\n전송양과 패킷크기가 다릅니다.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL LobbySession::SendStartGameInRoom( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_ROOM_GAME_START );

	//게임서버의 정보를 보낸다
	int size = strlen( m_document->GameSrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->GameSrvIP, size );
	sendPacket << m_document->GameSrvPortNum;

	//방에 있는 player들에게 전송!
	Room* tmpRoom = m_roomMgr->FindRoom( roomNum );
	tmpRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyGameStart( int roomNum )
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_GAME_START );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyGameEnd( int roomNum )
{
	SPacket sendPacket;
	
	sendPacket.SetID( SC_LOBBY_GAME_END );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect()
{
	SPacket sendPacket;

	sendPacket.SetID( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << m_myCharInfo->GetSessionID();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("LobbySession::SendPlayerDisconnect()\n")
												_T("[All In Lobby] 전송되었습니다.\n\n") );

#endif

	return TRUE;
}
