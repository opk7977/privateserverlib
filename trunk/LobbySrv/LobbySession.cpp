#include "LobbySession.h"
#include "LobbyProtocol.h"

#include "SSessionMgr.h"
#include "DataLeader.h"

#include "LobbyMgr.h"
#include "Room.h"
#include "LobbyChar.h"

#include "SrvNet.h"
#include "DBSrvMgr.h"

#include "LogSrvMgr.h"
#include "SLogger.h"


SIMPLEMENT_DYNAMIC(LobbySession)
SIMPLEMENT_DYNCREATE(LobbySession)

DataLeader*		LobbySession::m_document	= &GetDocument;
LobbyMgr*		LobbySession::m_lobbyMgr	= &GetLobbyMgr;
RoomMgr*		LobbySession::m_roomMgr		= &GetRoomMgr;
CharMgr*		LobbySession::m_charMgr		= &GetCharMgr;
SrvNet*			LobbySession::m_srvNet		= &GetSrvNet;
DBSrvMgr*		LobbySession::m_dbMgr		= &GetDBSrv;

#ifdef CONNECT_LOG_SERVER
	LogSrvMgr*	LobbySession::m_logSrv		= &GetLogSrvMgr;
#else
	LogSrvMgr*	LobbySession::m_logSrv		= NULL;
#endif

SLogger*	LobbySession::m_logger		= &GetLogger;


LobbySession::LobbySession(void)
: m_myCharInfo(NULL)
, m_myRoom(NULL)
{
// 	m_sessionMgr	= &GetSessionMgr;
// 	m_document		= &GetDocument;
// 	m_lobbyMgr		= &GetLobbyMgr;
// 	m_roomMgr		= &GetRoomMgr;
// 	m_charMgr		= &GetCharMgr;
// 	m_srvNet		= &GetSrvNet;
// 	m_dbMgr			= &GetDBSrv;
// #ifdef _DEBUG
// 	m_logger		= &GetLogger;
// #endif
}

LobbySession::~LobbySession(void)
{
}

void LobbySession::OnCreate()
{
	SSynchronize Sync( this );

	//sessionMgr에 저장하고
	SSession::OnCreate();

	//연결확인 패킷을 보내준다.
	SSession::SendPacket( SC_LOBBY_CONNECT_OK );
}

void LobbySession::OnDestroy()
{
	//SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		//캐릭터 정보가 없다면 서버 혹은 클라에서 자신의 정보를 보내기 전에 끈경우
		if( m_srvNet->GetSession() == this )
		{
			m_srvNet->SrvNetClear();

			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::OnDestroy()\n 게임서버와의 연결이 끊겼습니다.\n\n") );
		}
		else if( m_dbMgr->GetSession() == this )
		{
			m_dbMgr->DisConnect();

			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::OnDestroy()\n DB서버와의 연결이 끊겼습니다.\n\n") );
		}
#ifdef CONNECT_LOG_SERVER
		else if( m_logSrv->GetSession() == this )
		{
			m_logSrv->DisConnect();

			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::OnDestroy()\n Log서버와의 연결이 끊겼습니다.\n\n") );
		}
#endif
		else
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
							_T("LobbySession::OnDestroy()\n 캐릭터 정보가 없는 session의 연결을 끊습니다.\n\n") );
		}
	}
	else
	{
		if( !m_myCharInfo->GetIsPlay() )
		{
			//게임중이 아님
			if( m_myRoom == NULL )
			{
				//로비에 있음
#ifdef _DEBUG
				m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
								_T("LobbySession::OnDestroy()\n캐릭터 %s님이 로비에서 연결을 끊습니다.\n\n"),
								m_myCharInfo->GetID() );
#endif
#ifdef CONNECT_LOG_SERVER
				m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
								_T("LobbySession::OnDestroy() 캐릭터 %s님이 로비에서 연결을 끊습니다."),
								m_myCharInfo->GetID() );
#endif

				//로비에 있는 user
				//그냥 로비에서 삭제
				m_lobbyMgr->MinusUser( m_myCharInfo );
			}
			else
			{
				//방에 있음
				int room = m_myRoom->GetRoomNum();

#ifdef _DEBUG
				m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
								_T("LobbySession::OnDestroy()\n캐릭터 %s님이 %d번 방에서 연결을 끊습니다.\n\n"),
								m_myCharInfo->GetID(), room );
#endif
#ifdef CONNECT_LOG_SERVER
				m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
								_T("LobbySession::OnDestroy() 캐릭터 %s님이 %d번 방에서 연결을 끊습니다."),
								m_myCharInfo->GetID(), room );
#endif

				//방에서 지워준다
				if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
				{
					//방에 남아있는 인원이 없음
					//방 닫고
					m_roomMgr->CloseRoom( room );

#ifdef _DEBUG
					m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
									_T("LobbySession::OnDestroy()\n%d번 방이 닫힙니다.\n\n"),
									room );
#endif
#ifdef CONNECT_LOG_SERVER
					m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
									_T("LobbySession::OnDestroy() %d번 방이 닫힙니다."),
									room );
#endif

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

			//DB서버에 알림
			SendToDBCharDiscconect( m_myCharInfo->GetSessionID() );

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
	SSynchronize Sync( this );

	switch( packet.GetID() )
	{
	//==============================================================> LogSrv
	case LOG_SERVER_CONNECT_OK:
		RecvLogConnectOK();
		break;
	//==============================================================> DBSrv
	case DB_TO_OTHER_CONNECT_OK:
		RecvDBConnectOK();
		break;
	case DB_TO_LOBBY_CHARACTER_LOGIN:
		RecvCharacterLogin( packet );
		break;
	case DB_TO_OTHER_DROP_PLAYER:
		//
		break;
	//==============================================================> GameSrv
	case SC_GAME_CONNECT_OK:
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
	case CS_ROOM_MAP_CHANGE:
		RecvMapChange( packet );
		break;
	case CS_ROOM_MODE_CHANGE:
		RecvChangeMode( packet );
		break;
	case CS_ROOM_TEAM_CHANGE:
		RecvTeamChange();
		break;
	case CS_LOBBYSRV_ALL_CHAT:
		RecvAllChat( packet );
		break;
	case CS_LOBBYSRV_TARGET_CHAT:
		RecvTargetChat( packet );
		break;
	case CS_ROOM_START:
		RecvRoomStartGame();
		break;

	default:
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::PacketParsing()\n%d번 패킷, 패킷 ID가 유효하지 않습니다.\n\n"), packet.GetID() );
	}
}

//받은 패킷 처리 함수
//==============================================================

void LobbySession::RecvLogConnectOK()
{
	//이미 연결되어 있는지를 확인하고
	if( m_logSrv->IsConnect() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvDBConnectOK()\nLog서버설정이 이미 끝났습니다. 넌 누구냐...\n\n") );
		return;
	}

	//로그서버 연결
#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LobbySession::RecvDBConnectOK()\nLog서버와 연결되었습니다.\n\n") );
#endif
	m_logSrv->SetSession( this );
}

//--------------------------------------------------------------

void LobbySession::RecvDBConnectOK()
{
	//이미 연결되어 있는지를 확인하고
	if( m_dbMgr->IsConnect() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvDBConnectOK()\nDB서버설정이 이미 끝났습니다. 넌 누구냐...\n\n") );
		return;
	}
	//내가 서버다!!
#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LobbySession::RecvDBConnectOK()\nDB서버와 연결되었습니다.\n\n") );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
		_T("LobbySession::RecvDBConnectOK() DB서버와 연결되었습니다.") );
#endif
	m_dbMgr->SetSession( this );
}

void LobbySession::RecvCharacterLogin( SPacket& packet )
{
	int index, sessionId, size, rankId, rankPoint, AccumulKill, AccumulDeath;
	TCHAR uID[10]={0,};

	packet >> index >> sessionId;
	packet >> size;
	packet.GetData( uID, size );
	packet >> rankId >> rankPoint >> AccumulKill >> AccumulDeath;

	//캐릭터 공간을 하나 얻어 온다
	LobbyChar* tmpChar = m_charMgr->GetCharSPace();
	if( tmpChar == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvCharacterLogin()\n더이상 캐릭터를 수용할 수 있는 캐릭터 공간이 존재하지 않습니다.\n\n") );
		SendToDBCharInsertReadyResult( index, CHARAVER_SPACE_IS_FULL );
		return;
	}

	//캐릭터 정보를 셋팅하고
	tmpChar->SetSessionID( sessionId );
	tmpChar->SetID( uID );
	tmpChar->SetCharData( rankId, rankPoint, AccumulKill, AccumulDeath );

	//대기 공간에 추가
	m_charMgr->AddWaitChar( sessionId, tmpChar );

	//로비로 준비 완료 패킷을 보낸다
	SendToDBCharInsertReadyResult( index, sessionId );
}

//--------------------------------------------------------------

void LobbySession::RecvConnectServer()
{
	//이미 연결되어 있는지를 확인하고
#ifdef CONNECT_LOG_SERVER
	if( m_srvNet->GetSession() != NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvConnectServer()\n게임서버설정이 이미 끝났습니다. 넌 누구냐...\n\n") );
		return;
	}
	//내가 서버다!!
#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvConnectServer()\n게임서버와 연결되었습니다.\n\n") );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
					_T("LobbySession::RecvConnectServer() 게임서버와 연결되었습니다.") );
#endif
	m_srvNet->SetSession( this );

#endif
}

void LobbySession::RecvStartFaild( SPacket& packet )
{
	int room;
	packet >> room;

	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, 
						_T("LobbySession::RecvStartFaild()\n%d번 방의 정보가 없습니다.\n\n"), room );
		return;
	}

	{
		SSynchronize sync( tmpRoom );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, 
						_T("LobbySession::RecvStartFaild()\n%d번 방이 게임 시작에 실패했습니다.\n\n"), room );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("LobbySession::RecvStartFaild() %d번 방이 게임 시작에 실패했습니다."), room );

#endif

		//넘겨 받은 방의 방장에게 게임 시작 실패 패킷을 보낸다
		tmpRoom->GetLeader()->GetSession()->SendStartGameResult();
		//방의 상태를 바꿔주자
		tmpRoom->SetNormal();
	}

}

void LobbySession::RecvGameStart( SPacket& packet )
{
	int room;
	packet >> room;

	//해당 번호에 맞는 방을 찾아 온다
	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvGameStart()\n%d번 방의 정보가 없습니다.\n\n"), room );
		return;
	}

	{
		SSynchronize sync( tmpRoom );

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, 
						_T("LobbySession::RecvGameStart()\n%d번 방이 게임을 시작합니다.\n\n"), room );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
						_T("LobbySession::RecvGameStart() %d번 방이 게임을 시작합니다."), room );

#endif

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
}

void LobbySession::RecvGameEnd( SPacket& packet )
{
	//게임 안에 player가 모두 나갔을때 받는 패킷이다
	//게임이 종료 되고 player가 모두 나갔다. 그러니 방을 다시 열어 준다.
	int room;
	packet >> room;

	//해당 번호에 맞는 방을 찾아 온다
	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvGameEnd()\n%d번 방의 정보가 없습니다.\n\n"), room );
		return;
	}


	{
		SSynchronize sync( this );

		if( !tmpRoom->IsOpen() )
		{
			m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::RecvGameEnd()\n%d번 방은 이미 닫혔습니다.\n\n"), room );
			return;
		}


#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("LobbySession::RecvGameEnd()\n%d번 방이 게임을 종료했습니다.\n\n"), room );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("LobbySession::RecvGameEnd() %d번 방이 게임을 종료했습니다."), room );

#endif

		//그 방의 play상태를 풀어 준다
		tmpRoom->SetNormal();

	}		
	//로비 사람들에게 방이 다시 열렸다고 알려 준다.
	SendLobbyGameEnd( room );
}

void LobbySession::RecvPlayerDiconnectInGame( SPacket& packet )
{
	//================================================
	// 게임 프로세스에서 player가 게임을 종료 한경우
	//================================================
	int room, session, team;
	packet >> room;
	packet >> session;
	packet >> team;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvPlayerDiconnectInGame()\n게임 서버에서 %d번 방의 sessionID %d번 캐릭터가 게임을 종료했습니다.\n\n"), 
					room, session );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvPlayerDiconnectInGame() 게임 서버에서 %d번 방의 sessionID %d번 캐릭터가 게임을 종료했습니다."), 
					room, session );

#endif

	Room* tmpRoom = m_roomMgr->FindRoom( room );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvPlayerDiconnectInGame()\n%d번 방의 정보가 없습니다.\n\n"), 
					room );
	}
	else
	{
		SSynchronize sync( tmpRoom );

		//방인원을 줄인다.
		if( !tmpRoom->DelPlayerInRoomAtPlaying( team ) )
		{
			//방에 남아있는 인원이 없음
			//방 닫고
			m_roomMgr->CloseRoom( room );

#ifdef _DEBUG
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
							_T("LobbySession::RecvPlayerDiconnectInGame()\n%d번 방이 닫힙니다.\n\n"),
							room );
#endif
#ifdef CONNECT_LOG_SERVER
			m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
							_T("LobbySession::RecvPlayerDiconnectInGame() %d번 방이 닫힙니다."), 
							room );

#endif

			//방이 닫혔다는 정보를 로비의 사람들에게 보낸다.
			SendCloseRoom( room );
		}
	}

	//로비에 있는 사람들에게 1명 게임에서 나갔다고 알린다
	SendPlayerDisconnect( session );

	//DB서버에 알림
	SendToDBCharDiscconect( session );

	//캐릭터 정보를 삭제 해 준다.
	m_charMgr->ReturnCharSpace( session );
}

//--------------------------------------------------------------

void LobbySession::RecvInsertLobby( SPacket& packet )
{
	int sessionId, roomNum;
	packet >> sessionId;
	packet >> roomNum;


	//방번호를 확인해서 재 접속인지를 보자
	if( roomNum > 0 )
	{
		{
			SSynchronize sync( this );

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
		}

		m_myCharInfo->SetSession( this );

		//캐릭터의 게임 실행 상태와 ready상태를 초기화 해주자
		m_myCharInfo->SetIsPlay( FALSE );
		m_myCharInfo->SetReady( FALSE );
	

#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("LobbySession::RecvInsertLobby()\n캐릭터 %s, %d번 방으로 접속.\n\n"),
						m_myCharInfo->GetID(), roomNum );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("LobbySession::RecvInsertLobby() 캐릭터 %s, %d번 방으로 접속."),
						m_myCharInfo->GetID(), roomNum );

#endif

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
			m_lobbyMgr->AddUser( m_myCharInfo );
		}
		else
		{
			//--------------------------------------
			// 새로 접속
			//--------------------------------------
			//캐릭터를 얻어 온다
			m_myCharInfo = m_charMgr->GetWaitCharInfo( sessionId );
			if( m_myCharInfo == NULL )
			{
				m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvInsertLobby()\n%d번에 해당하는 캐릭터의 정보가 준비 되어 있지 않습니다.\n\n"),
					sessionId );
				return;
			}

#ifdef _DEBUG
			m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
							_T("LobbySession::RecvInsertLobby()\n캐릭터 %s가 로비로 접속합니다.\n\n"),
							m_myCharInfo->GetID() );
#endif
#ifdef CONNECT_LOG_SERVER
			m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
							_T("LobbySession::RecvInsertLobby() 캐릭터 %s가 로비로 접속합니다."),
							m_myCharInfo->GetID() );

#endif

			//======================================
			// 내 정보 셋팅
			//======================================
			//현재의 세션에 내 ID를 넣어 준다
			SetSessionID( sessionId );
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

		//그리고 모든 유저 정보를 보낸다.( 내 정보 포함 )
		SendOtherCharInfo();
	}
}

void LobbySession::RecvCreateRoom( SPacket& packet )
{
	int size;
	TCHAR title[50]={0,};

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	packet >> size;
	packet.GetData( title, size );

	//방 모드 변수들
	int stageMap, gameMode, playTime, playCount;
	packet >> stageMap >> gameMode >> playTime >> playCount;

	m_myRoom = m_roomMgr->OpenRoom( title );
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n방이 열기에 실패했습니다.\n\n") );
		SendResultCreate( -10 );
		return;
	}
	//게임 모드 설정
	m_myRoom->SetStageMap( stageMap );
	m_myRoom->SetGameMode( gameMode );
	m_myRoom->SetPlayTime( playTime );
	m_myRoom->SetPlayCount( playCount );
	
	//방장은 우선 나로 설정해 둔다
	m_myRoom->SetLeader( m_myCharInfo );

#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvCreateRoom()\n%s님이 %d번(%s)방을 열었습니다/n map:%d/ mode:%d/ time:%d/ count:%d\n\n"),
					m_myCharInfo->GetID(), m_myRoom->GetRoomNum(), title
					, stageMap, gameMode, playTime, playCount );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvCreateRoom() %s님이 %d번(%s)방을 열었습니다/ map:%d/ mode:%d/ time:%d/ count:%d"),
					m_myCharInfo->GetID(), m_myRoom->GetRoomNum(), title
					, stageMap, gameMode, playTime, playCount );
#endif

	//방만들기 성공
	SendResultCreate( m_myRoom->GetRoomNum() );

	//모두에게 방이 만들어졌다는 패킷을 보낸다.
	SendOpenRoom();
}

void LobbySession::RecvInsertRoom( SPacket& packet )
{
	//방에 들어간다는 요청을 받음
	int room;
	packet >> room;

	//SSynchronize Sync( this );
	
	//우선 방번호로 방을 받아 와 본다
	m_myRoom = m_roomMgr->FindRoom( room );
	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n[캐릭터 %s] %d번 방을 찾을 수 없습니다. 방 입장에 실패했습니다.\n\n"),
						m_myCharInfo->GetID(), room );
		SendResultInsert(-10);
		return;
	}

	//캐릭터에 방을 넣어 준다.
	m_myCharInfo->SetRoom( m_myRoom );

	//방이 지금 게임 준비중이거나 인원이 다 찼으면 들어갈 수 없다
	if( !m_myRoom->CanInsert() )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvCreateRoom()\n[캐릭터 %s] %d번 방은 입장 할 수 없는 상태 입니다.\n\n"),
						m_myCharInfo->GetID(), room );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
						_T("LobbySession::RecvCreateRoom() [캐릭터 %s] %d번 방은 입장 할 수 없는 상태 입니다."),
						m_myCharInfo->GetID(), room );
#endif
		m_myRoom = NULL;
		SendResultInsert(-10);
		return;
	}

	//방이 정상으로 존재 한다면 넣자
	m_myRoom->AddPlayerInRoom( m_myCharInfo );

	//로비리스트에서의 내 정보를 지워 준다.
	m_lobbyMgr->MinusUser( m_myCharInfo );

#ifdef _DEBUG 
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvInsertRoom()\n%s(SessionId:%d번) %d번 방으로 들어감\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, room );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvInsertRoom() %s(SessionId:%d번) %d번 방으로 들어감")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, room );
#endif

	//우선 들어가기에 성공했다는 패킷을 보낸다
	SendResultInsert( room );

	//로비로 방에 사람이 들어갔다는 것을 알린다.
	SendInsertRoom();

	//그리고 방에 있는 사람들에게 내 정보를 보낸다.
	SendRoomMyInfoToOtherChar();

	//나에게 방에 있는 모든 사람들의 정보를 보낸다.
	SendRoomOtherCharInfo();

	//나에게 방장의 정보를 알린다.
	SendRoomLeader();

	//방장에게 start버튼관련 packet을 보내는 함수
	SendStartBtnForVisible();
}

void LobbySession::RecvOutRoom()
{

	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvOutRoom()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
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

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvOutRoom()\n%s(sessionId:%d) %d번 방을 나갑니다\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvOutRoom() %s(sessionId:%d) %d번 방을 나갑니다.")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myRoom->GetRoomNum() );
#endif

	if( !m_myRoom->DelPlayerInRoom( m_myCharInfo ) )
	{
		//방에 사람 없음 방을 지워준다
		m_roomMgr->CloseRoom( m_myRoom->GetRoomNum() );
		SendCloseRoom( m_myRoom->GetRoomNum() );
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
			_T("LobbySession::RecvOutRoom()\n%d번 방이 닫힙니다.\n\n"),
			m_myRoom->GetRoomNum() );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
			_T("LobbySession::RecvOutRoom() %d번 방이 닫힙니다."),
			m_myRoom->GetRoomNum() );
#endif
	}

	//팀/ ready정보 초기화
	m_myCharInfo->SetRoom(NULL);
	m_myCharInfo->SetTeam(-1);
	m_myCharInfo->SetReady(FALSE);

	SendRoomOutResult();
	
	//방의 클라들에게 누가 사라졌는지 보낸다
	SendRoomCharOut();

	//로비의 클라들에게 누가 어떤방에서 나왔는지를 알린다
	SendLobbyRoomCharOut();

	//방정보와 팀정보를 초기화
	m_myRoom = NULL;
}

void LobbySession::RecvReady()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
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
						_T("LobbySession::RecvReady()\n%d번방은 이미 일반 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		return;
	}

	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvReady()\n방장주제에 ready하지마.\n\n") );
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
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvReady() %s(sessionId:%d) Ready상태가 %s으로 변경되었습니다.")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, m_myCharInfo->GetReady() ? _T("시작") : _T("준비중") );
#endif

	SendRoomCharReady();

	SendStartBtnForVisible();
}

void LobbySession::RecvMapChange( SPacket& packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvMapChange()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
			GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvMapChange()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	//우선 애가 방장이 맞는지 확인
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvMapChange()\n[캐릭터 %s] 캐릭터가 방장이 아닙니다.\n\n"),
			m_myCharInfo->GetID() );
		return;
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_WORRNING,
			_T("LobbySession::RecvMapChange() [캐릭터 %s] 캐릭터가 방장이 아닙니다."),
			m_myCharInfo->GetID() );
#endif

	}

	int stageMap;
	packet >> stageMap;

	//맵이 바뀐게 아니면 그냥 넘어 간다
	if( m_myRoom->GetStageMap() == stageMap )
		return;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
		_T("LobbySession::RecvMapChange()\n%d번방의 맵이 %d번으로 바뀝니다.\n\n"),
		m_myRoom->GetRoomNum(), stageMap );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
		_T("LobbySession::RecvMapChange() %d번방의 맵이 %d번으로 바뀝니다."),
		m_myRoom->GetRoomNum(), stageMap );
#endif

	//아니면 변경해 주고
	m_myRoom->SetStageMap( stageMap );

	//방 사람들에게 말해 주고
	SendRoomMapChange();

	//로비의 사람들에게 말해 준다
	SendLobbyMapChange();
}

void LobbySession::RecvChangeMode( SPacket& packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
			GetSocket() );
		return;
	}

	if( m_myRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n[캐릭터 %s] 방 정보가 존재하지 않습니다.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	//이놈의 방장인지 확인한다
	if( m_myRoom->GetLeader() != m_myCharInfo )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n[캐릭터 %s] %d번 방의 방장이 아닙니다.\n\n"),
			m_myCharInfo->GetID(), m_myRoom->GetRoomNum() );
		return;
	}

	//방이 맵변경이 가능한지 확인한다
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChangeMap()\n%d번 방은 현재 데이터를 변경할 수 있는 상태가 아닙니다.\n\n"),
			m_myCharInfo->GetID() );
		return;
	}

	int gameMode, playTime, playCount;

	packet >> gameMode;
	packet >> playTime;
	packet >> playCount;

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
					_T("LobbySession::RecvChangeMode()\n%d번방/ 모드: %d, playTime: %d, Count: %d로 변경됩니다.\n\n"),
					m_myRoom->GetRoomNum(), gameMode, playTime, playCount );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvChangeMode() %d번방/ 모드: %d, playTime: %d, Count: %d로 변경됩니다."),
					m_myRoom->GetRoomNum(), gameMode, playTime, playCount );
#endif

	m_myRoom->SetGameMode( gameMode );
	m_myRoom->SetPlayTime( playTime );
	m_myRoom->SetPlayCount( playCount );

	//변한 Mode들을 방의 모두에게 알린다.
	SendRoomModeChange();

	//로비의 사람들에게도 알린다
	SendLobbyModeChange();
}

void LobbySession::RecvTeamChange()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
						GetSocket() );
		return;
	}

	//ready상태면 팀 바꿀 수 업써..
	if( m_myCharInfo->GetReady() == ROOM_READY_OK )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvTeamChange()\n[캐릭터 %s] 너 임마 ready상태..\n\n"),
						m_myCharInfo->GetID() );
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
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					 _T("LobbySession::RecvTeamChange() %s(sessionId:%d) 팀을 %s으로 변경합니다.")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("공격팀") : _T("수비팀") );
#endif

	//방에 팀카운트 수정
	if( !m_myRoom->ChangeTeam( team ) )
	{
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM
					, _T("LobbySession::RecvTeamChange()\n")
					_T("%s(sessionId:%d) %s팀 인원이 꽉차서 변경할 수 없습니다.\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("공격팀") : _T("수비팀") );

#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvTeamChange() %s(sessionId:%d) %s팀 인원이 꽉차서 변경할 수 없습니다.")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetSessionID()
					, ( team == ROOM_TEAM_ATT ) ? _T("공격팀") : _T("수비팀") );
#endif
		return;
	}


	//팀을 바꾸고
	m_myCharInfo->SetTeam( team );
	

	SendRoomTeamChange();
}

void LobbySession::RecvAllChat( SPacket& packet )
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvChat()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
			GetSocket() );
		return;
	}
	int size;
	TCHAR chatText[200] = {0,};

	packet >> size;
	packet.GetData( chatText, size );

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvChat()\n")
					_T("[%s] %s\n\n")
					, m_myCharInfo->GetID()
					, chatText );

#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvChat() [%s] %s")
					, m_myCharInfo->GetID()
					, chatText );
#endif

	//전체 체팅
	SendAllChat( chatText );	
}

void LobbySession::RecvTargetChat( SPacket& packet )
{
// 	if( m_myCharInfo == NULL )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("LobbySession::RecvTargetChat()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
// 			GetSocket() );
// 		return;
// 	}
// 
// 	int size;
// 	TCHAR targetID[20] = {0,};
// 	TCHAR chatText[200] = {0,};
// 
// 	packet >> size;
// 	packet.GetData( targetID, size );
// 	packet >> size;
// 	packet.GetData( chatText, size );
// 
// 	int sessionId = m_DBMgr->GetSessionId( targetID );
// 	if( sessionId == 0 )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("LobbySession::RecvTargetChat()\n[캐릭터%s] 캐릭터 정보가 존재하지 않습니다.\n\n"),
// 			GetSocket() );
// 		return;
// 	}
// 
// // #ifdef _DEBUG
// 	m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("LobbySession::RecvChat()\n")
// 					_T("[%s] %s\n\n")
// 					, m_myCharInfo->GetID()
// 					, chatText );
// // #endif
// 
// 	//귓말
// 	SendTargetChatToTarget( sessionId, chatText );
// 	SendTargetChatToMe( sessionId, chatText );
}

void LobbySession::RecvRoomStartGame()
{
	if( m_myCharInfo == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n[소켓번호%d] 캐릭터 정보가 존재하지 않습니다.\n\n"),
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

	//서버와 연결되어 있는 상태인지 확인
	if( !m_srvNet->IsConnect() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
			_T("LobbySession::RecvRoomStartGame()\n게임 서버와 연결되어 있지 않습니다.\n\n") );
		SendStartGameResult();
		return;
	}

	//방의 상태가 노말이 아니면 무시
	if( m_myRoom->GetRoomState() != ROOM_STATE_NORMAL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d번방은 이미 일반 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		SendStartGameResult();
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
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("LobbySession::RecvRoomStartGame()\n%d번 방이 게임 시작 가능한 상태가 아닙니다.\n\n"),
						m_myRoom->GetRoomNum() );
		//실패 패킷을 보낸다
		SendStartGameResult();
		return;
	}

#ifdef _DEBUG
	m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("LobbySession::RecvRoomStartGame()\n%d번방 게임 시작을 시도(?)합니다.\n\n"),
					m_myRoom->GetRoomNum() );
#endif
#ifdef CONNECT_LOG_SERVER
	m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
					_T("LobbySession::RecvRoomStartGame() %d번방 게임 시작을 시도(?)합니다."),
					m_myRoom->GetRoomNum() );
#endif

	//방을 게임준비중인 상태로 바꾼다
	m_myRoom->SetReady();

	//게임서버로 정보를 보낸다
	SendCreateGameProc();
}


//보내는 함수
//==============================================================

BOOL LobbySession::SendToDBCharInsertReadyResult( int index, int sessionId )
{
	SPacket sendPacket( LOBBY_TO_DB_CHARINSERT_READY_RESULT );

	sendPacket << index;
	sendPacket << sessionId;

	//DB서버로 보낸다
	m_dbMgr->SendToDBServer( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendToDBCharDiscconect( int sessionID )
{
	SPacket sendPacket( OTHER_TO_DB_DISCONNECT_CHARACTER );

	sendPacket << sessionID;

	//DB서버로 보낸다
	m_dbMgr->SendToDBServer( sendPacket );

	return TRUE;
}

//--------------------------------------------------------------

BOOL LobbySession::SendCreateGameProc()
{
	SPacket sendPacket( LG_START_GAME );
	//게임서버에 준비를 하라고 전달

	//int count = m_myRoom->GetPlayerCount();

	sendPacket << m_myRoom->GetRoomNum();		//방번호
	sendPacket << m_myRoom->GetStageMap();		//게임 맵
	sendPacket << m_myRoom->GetGameMode();		//게임 모드
	sendPacket << m_myRoom->GetPlayTime();		//판당 playTime
	sendPacket << m_myRoom->GetPlayCount();		//게임 판수

	//인원수를 넣고
	sendPacket << m_myRoom->GetPlayerCount();

	//방에 있는 캐릭터들의 정보를 담는다
	m_myRoom->PackagePlayerInRoomForGame( sendPacket );

	//게임 서버로 전송
	m_srvNet->SendToGameServer( sendPacket );
	
	return TRUE;
}

//--------------------------------------------------------------

BOOL LobbySession::SendOtherCharInfo()
{
	SPacket sendPacket( SC_LOBBY_OTHER_CHARINFO );

	//모든 캐릭터의 정보를 담자
	//m_lobbyMgr->PackageDataAllInLobby( sendPacket );
	m_charMgr->PackageAllCharacter( sendPacket );

	//나에게 패킷을 보낸다!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendOtherCharInfo()\n")
														_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

		return FALSE;
	}


	return TRUE;
}

BOOL LobbySession::SendMyCharInfo()
{
	SPacket sendPacket( SC_LOBBY_OTHER_CHARINFO );

	//내 정보를 담는다
	sendPacket << 1;

	m_myCharInfo->PackageMyInfoForLobby( sendPacket );

	//모두에게 전송( 나는 빼고 보낸다! )
	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendRoomInfo()
{
	SPacket sendPacket( SC_LOBBY_ROOMINFO );
	//방 개수가 0이면 안보내도 된다
	if( m_roomMgr->OpenRoomCount() <= 0 )
		return TRUE;	

	//모든 방 정보를 담는다
	m_roomMgr->PackageOpenRoomInfo( sendPacket );

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, 
			_T("LobbySession::SendRoomInfo()\n보낸크기와 패킷의 크기가 다릅니다.\n\n") );

		return FALSE;
	}

	return TRUE;
}

BOOL LobbySession::SendResultCreate( int result )
{
	SPacket sendPacket( SC_ROOM_RESULT_CREATE );
	sendPacket << result;

	if( result > 0 )
	{
		int size = _tcslen( m_myRoom->GetTitle() ) * sizeof( TCHAR );
		sendPacket << size;
		sendPacket.PutData( m_myRoom->GetTitle(), size );

		m_myRoom->PackageRoomModeInfo( sendPacket );
	}

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendResultCreate()\n")
														_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

		return FALSE;
	}

	return TRUE;
}

BOOL LobbySession::SendOpenRoom()
{
	//방의 정보를 담자
	SPacket sendPacket( SC_LOBBY_OPEN_ROOM );
	sendPacket << m_myRoom->GetRoomNum();
	int size = _tcslen( m_myRoom->GetTitle() ) * sizeof( TCHAR );
	sendPacket << size;
	sendPacket.PutData( m_myRoom->GetTitle(), size );
	//방의 변수 정보들
	m_myRoom->PackageRoomModeInfo( sendPacket );

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendCloseRoom( int roomNum )
{
	SPacket sendPacket( SC_LOBBY_CLOSE_ROOM );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendResultInsert( int result )
{
	SPacket sendPacket( SC_ROOM_RESULT_INSERT );
	sendPacket << result;

	int retval = SendPacket( sendPacket );

	if( retval != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendResultInsert()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

		return FALSE;
	}

	return TRUE;
}

BOOL LobbySession::SendInsertRoom()
{
	SPacket sendPacket( SC_LOBBY_INSERT_ROOM );
	
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myRoom->GetRoomNum();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendRoomOtherCharInfo()
{
	//나에게 방에 있는 사람 모두의 정보를 보낸다( 내 정보까지...)
	//전송할 패킷을 만든다.
	SPacket sendPacket( SC_ROOM_OTHER_CHARINFO );

	//캐릭터 수를 담는다
	sendPacket << m_myRoom->GetPlayerCount();

	//캐릭터 정보를 모두 담는다
	m_myRoom->PackagePlayerInRoom( sendPacket );

	//나에게 보낸다!
	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendRoomOtherCharInfo()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

		return FALSE;
	}

	return TRUE;
}

BOOL LobbySession::SendRoomMyInfoToOtherChar()
{
	//내 정보를 방에 있는 모든 사람들에게 보낸다
	//전송할패킷을 만든다
	SPacket	sendPacket( SC_ROOM_OTHER_CHARINFO );

	sendPacket << 1;			//count

	//내 정보를 담는다
	m_myCharInfo->PackageMyInfoForRoom( sendPacket );

	//모두에게 보내되 나에겐 보내지 않는다.
	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendRoomLeader()
{
	SPacket sendPacket( SC_ROOM_LEADER );
	sendPacket << m_myRoom->GetLeader()->GetSessionID();

	int result = SendPacket( sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("LobbySession::SendRoomLeader()\n")
													_T("보낸크기와 패킷의 크기가 다릅니다.\n\n") );

		return FALSE;
	}

	return TRUE;
}

BOOL LobbySession::SendRoomLeaderToAll( int leader )
{
	//방장이 바뀌어 모두에게 세션 번호를 보내준다
	SPacket sendPacket( SC_ROOM_LEADER );
	sendPacket << leader;

	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendRoomOutResult()
{
	SPacket sendPacket( SC_ROOM_OUT_RESULT );

	SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomCharOut()
{
	//////////////////////////////////////////////////////////////////////////
	//방에 보내줄 packet 
	SPacket sendPacket( SC_ROOM_CHAR_OUT );
	sendPacket << m_myCharInfo->GetSessionID();

	//방에는 내가 나갔다는 신호를 보내주고
	m_myRoom->SendPacketAllInRoom( sendPacket, m_myCharInfo );

	//--------------------------------------------------------------
	// 내가 나가고 방의 leader가 바뀌었다면 바뀐 정보를 보내줘야 한다.
	// 이 녀석이 leader인지 확인한다.
	if( m_myRoom->GetLeader() == m_myCharInfo )
	{
		//leader를 바꾸고 패킷을 보내준다.
		SendRoomLeaderToAll( m_myRoom->ChangeLeader()->GetSessionID() );
#ifdef _DEBUG
		m_logger->PutLog( SLogger::LOG_LEVEL_SYSTEM,
						_T("LobbySession::SendRoomCharOut()\n")
						_T("%d번 방의 %s님이 방장이 되셨습니다.\n\n"),
						m_myRoom->GetRoomNum(), m_myRoom->GetLeader()->GetID() );
#endif
#ifdef CONNECT_LOG_SERVER
		m_logSrv->SendLog( LogSrvMgr::LOG_LEVEL_INFORMATION,
						_T("LobbySession::SendRoomCharOut() %d번 방의 %s님이 방장이 되셨습니다."),
						m_myRoom->GetRoomNum(), m_myRoom->GetLeader()->GetID() );
#endif
	}
	//--------------------------------------------------------------

	SendStartBtnForVisible();

	return TRUE;
}

BOOL LobbySession::SendLobbyRoomCharOut()
{
	SPacket sendPacket( SC_LOBBY_ROOM_PLAYER_OUT );
	sendPacket << m_myRoom->GetRoomNum();
	sendPacket << m_myCharInfo->GetSessionID();

	//로비의 사람들에게 몇번방에 사람이 줄었다는 패킷을 보낸다.
	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomCharReady()
{
	SPacket sendPacket( SC_ROOM_CHAR_READY );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetReady();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomMapChange()
{
	SPacket sendPacket( SC_ROOM_MAP_CHANGE );

	sendPacket << m_myRoom->GetStageMap();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyMapChange()
{
	SPacket sendPacket( SC_LOBBY_MAP_CHANGE );

	sendPacket << m_myRoom->GetRoomNum();
	sendPacket << m_myRoom->GetStageMap();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomModeChange()
{
	SPacket sendPacket( SC_ROOM_MODE_CHANGE );

	m_myRoom->PackageRoomModeInfo( sendPacket );

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyModeChange()
{
	SPacket sendPacket( SC_LOBBY_MODE_CHANGE );
	sendPacket << m_myRoom->GetRoomNum();

	m_myRoom->PackageRoomModeInfo( sendPacket );

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomTeamChange()
{
	SPacket sendPacket( SC_ROOM_TEAM_CHANGE );
	sendPacket << m_myCharInfo->GetSessionID();
	sendPacket << m_myCharInfo->GetTeam();

	m_myRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendAllChat( TCHAR* chat )
{
	//ID와 채팅 문장을 넣는다.
	TCHAR tmpText[100]={0,};
	wsprintf( tmpText, _T("[%s] %s\n"), m_myCharInfo->GetID(), chat );

	//패킷을 만들고
	SPacket sendPacket( SC_LOBBYSRV_ALL_CHAT );
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
	}
	else
	{
		//나한테까지 보내야 한다.
		m_myRoom->SendPacketAllInRoom( sendPacket );
	}

	return TRUE;
}

BOOL LobbySession::SendTargetChatToTarget( int target, TCHAR* chat )
{
// 	LobbySession* tmpSession = (LobbySession*)m_sessionMgr->FindSession( target );
// 	if( tmpSession == NULL )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("LobbySession::SendChatForTarget()\n")
// 			_T("%d번 세션의 캐릭터가 존재하지 않습니다.\n\n")
// 			, target );
// 		return FALSE;
// 	}
// 
// 	//ID와 채팅 문장을 넣는다.
// 	TCHAR tmpText[255]={0,};
// 	wsprintf( tmpText, _T("[From %s] %s\n"), m_myCharInfo->GetID(), chat );
// 
// 	//패킷을 만들고
// 	SPacket sendPacket( SC_LOBBYSRV_TARGET_CHAT );
// 
// 	Room* tmpRoom = tmpSession->GetMyRoom();
// 	if( tmpRoom == NULL )
// 	{
// 		//로비에 있으면 0을 넣어 준다
// 		sendPacket << 0;
// 	}
// 	else
// 	{
// 		//방에 있으면 방번호를 넣어 준다
// 		sendPacket << tmpRoom->GetRoomNum();
// 	}
// 
// 	//채팅문장을 넣어 준다
// 	int size = _tcslen( tmpText )*sizeof(TCHAR);
// 	sendPacket << size;
// 	sendPacket.PutData( tmpText, size );
// 
// 	tmpSession->SendPacket( sendPacket );
 
 	return TRUE;
}

BOOL LobbySession::SendTargetChatToMe( int target, TCHAR* chat )
{
// 	LobbySession* tmpSession = (LobbySession*)m_sessionMgr->FindSession( target );
// 	if( tmpSession == NULL )
// 	{
// 		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
// 			_T("LobbySession::SendChatForTarget()\n")
// 			_T("%d번 세션의 캐릭터가 존재하지 않습니다.\n\n")
// 			, target );
// 		return FALSE;
// 	}
// 
// 	//ID와 채팅 문장을 넣는다.
// 	TCHAR tmpText[255]={0,};
// 	wsprintf( tmpText, _T("[To %s] %s\n"), tmpSession->GetMyInfo()->GetID(), chat );
// 
// 	//패킷을 만들고
// 	SPacket sendPacket( SC_LOBBYSRV_TARGET_CHAT );
// 
// 	if( m_myRoom == NULL )
// 	{
// 		//로비에 있으면 0을 넣어 준다
// 		sendPacket << 0;
// 	}
// 	else
// 	{
// 		//방에 있으면 방번호를 넣어 준다
// 		sendPacket << m_myRoom->GetRoomNum();
// 	}
// 
// 	//채팅문장을 넣어 준다
// 	int size = _tcslen( tmpText )*sizeof(TCHAR);
// 	sendPacket << size;
// 	sendPacket.PutData( tmpText, size );
// 
// 	SendPacket( sendPacket );
 
 	return TRUE;
}

BOOL LobbySession::SendStartBtnForVisible( int oldReadyCount )
{
	//방이 실행가능 상태면 리더에게 보내주자
	if( m_myRoom->PossiblePlay() )
		SendRoomStartVisible();

	else if( oldReadyCount == m_myRoom->GetPlayerCount()-1 )
	{
		//ready인원이 줄었다면 비활성을 보내준다
		if( m_myRoom->GetReadyCount() < oldReadyCount )
			SendRoomStartInvisible();
	}

	return TRUE;
}

BOOL LobbySession::SendStartBtnForVisible()
{
	if( m_myRoom->PossiblePlay() )
		SendRoomStartVisible();
	else
		SendRoomStartInvisible();

	return TRUE;
}

BOOL LobbySession::SendRoomStartVisible()
{
	SPacket sendPacket( SC_ROOM_START_VISIBLE );

	m_myRoom->GetLeader()->GetSession()->SendPacket( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendRoomStartInvisible()
{
	SPacket sendPacket( SC_ROOM_START_INVISIBLE );

	if( m_myRoom->GetPlayerCount() > 0 )
		m_myRoom->GetLeader()->GetSession()->SendPacket( sendPacket );
		
	return TRUE;
}

BOOL LobbySession::SendStartGameResult()
{
	//게임 스타트에 실패!
	SPacket sendPacket( SC_ROOM_START_RESULT );
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
	//방을 받아와 본다.
	Room* tmpRoom = m_roomMgr->FindRoom( roomNum );
	if( tmpRoom == NULL )
	{
		m_logger->PutLog( SLogger::LOG_LEVEL_WORRNIG,
				_T("LobbySession::SendStartGameInRoom()\n")
				_T("%d번 방이 존재하지 않습니다.\n\n")
				, roomNum );
		return FALSE;
	}

	SPacket sendPacket( SC_ROOM_GAME_START );

	//게임서버의 정보를 보낸다
	int size = strlen( m_document->GameSrvIP );
	sendPacket << size;
	sendPacket.PutData( m_document->GameSrvIP, size );
	sendPacket << m_document->GameSrvPortNum;
	//해당방의 UDP번호를 넣어 준다
	sendPacket << (m_document->GameSrvPortNum+tmpRoom->GetRoomNum()+10);

	//방에 있는 player들에게 전송!
	tmpRoom->SendPacketAllInRoom( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyGameStart( int roomNum )
{
	SPacket sendPacket( SC_LOBBY_GAME_START );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendLobbyGameEnd( int roomNum )
{
	SPacket sendPacket( SC_LOBBY_GAME_END );
	sendPacket << roomNum;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect()
{
	SPacket sendPacket( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << m_myCharInfo->GetSessionID();

	m_lobbyMgr->SendPacketAllInLobby( sendPacket, m_myCharInfo );

	return TRUE;
}

BOOL LobbySession::SendPlayerDisconnect( int sessionId )
{
	SPacket sendPacket( SC_LOBBY_PLAYER_DISCONNECT );
	sendPacket << sessionId;

	m_lobbyMgr->SendPacketAllInLobby( sendPacket );

	return TRUE;
}
