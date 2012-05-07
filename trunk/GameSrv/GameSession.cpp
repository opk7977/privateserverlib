#include "GameSession.h"
#include "GameProtocol.h"
#include "SLogger.h"

#include "CharMgr.h"
#include "GameMgr.h"
#include "GameProc.h"

#include "SrvNet.h"


SIMPLEMENT_DYNAMIC(GameSession)
SIMPLEMENT_DYNCREATE(GameSession)

GameSession::GameSession(void)
{
	Init();
}

GameSession::~GameSession(void)
{
}

void GameSession::Init()
{
	m_myCharInfo = NULL;
	m_myGameProc = NULL;
	isEndGame	= TRUE;
}

void GameSession::OnCreate()
{
	SSession::OnCreate();

	SendPacket( SC_GAME_CONNECT_OK );
}

void GameSession::OnDestroy()
{
	SSynchronize Sync( this );

	if( m_myCharInfo != NULL )
	{
		//캐릭터 정보가 없다면 그냥 무시
		if( isEndGame )
		{
			//완전히 게임을 끔
			//lobby서버로 접속을 종료 했음을 알려 준다
			SendToSrvThatPlayerDisconnect();
		}
		//아직 남아 있는 같이 게임하고 있던 사람들에게 내가 나가는것을 알림
		SendCharDisconnect();

		//방에서 나를 제거
		if( !m_myGameProc->DelPlayer( this ) )
		{
			//현재 게임 내에 아무도 남지 않았음
			//로비로 방의 게임 proc이 닫혔음을 알림?

			//게임 proc닫음
			m_myGameProc->Init();
		}
	}

	//캐릭터 공간의 정보를 지워 준다.
	if( m_myCharInfo == NULL )
		GetCharMgr.ReturnCharSpace( m_myCharInfo );

	//세션 초기화
	Init();

	SSession::OnDestroy();
}

void GameSession::PackageMyInfo( SPacket& packet )
{
	{
		SSynchronize sync( this );

		packet << m_myCharInfo->GetIndexId();
		packet << m_myCharInfo->GetState();
		packet << m_myCharInfo->GetPosX();
		packet << m_myCharInfo->GetPosY();
		packet << m_myCharInfo->GetPosZ();
		packet << m_myCharInfo->GetDirX();
		packet << m_myCharInfo->GetDirY();
		packet << m_myCharInfo->GetDirZ();
		packet << m_myCharInfo->GetDirInt();
	}
}

void GameSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case LG_START_GAME:
		RecvLobbyStartGame( packet );
		break;
	//==============================================================> LobbySrv
	case CS_GAME_INGAME:
		RecvInGame( packet );
		break;
	case CS_GAME_MOVE_CHAR:
		RecvMoveChar( packet );
		break;
	//==============================================================> Client
	default:
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::PacketParsing()\n받은 패킷의 아이디가 유효하지 않습니다.\n\n") );
	}
}

//======================================
// 받은 패킷 처리
//======================================
//--------------------------------------
// Lobby Srv와의 커뮤니케이션
//--------------------------------------
void GameSession::RecvLobbyConnectOK()
{
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvLobbyConnectOK()\n로비서버와 연결에 성공하였습니다\n\n") );

	GetSrvNet.SetSession( this );

	SPacket sendPacket;
	sendPacket.SetID( GL_CONNECT_SERVER );

	GetSrvNet.SendToLobbyServer( sendPacket );
}

void GameSession::RecvLobbyStartGame( SPacket &packet )
{
	int roomNum, count;

	packet >> roomNum;		//방번호
	packet >> count;		//방의 인원수

	//우선 방 번호에 해당하는 게임을 연다
	GameProc* tmpGame = GetGameMgr.FindGame( roomNum );
	if( tmpGame == NULL )
	{
		//게임proc을 열수 없거나 실패했다는 패킷을 보낸다
		SendStartFaild( roomNum );
		return;
	}
	if( tmpGame->NowIsPlay() )
	{
		//이미 게임이 진행 중이면 실패했다는 패킷을 보낸다.
		SendStartFaild( roomNum );
		return;
	}

	//방을 얻어 왔고 게임이 시작중이 아니라면 게임을 열고 캐릭터를 저장해 준다

	int sessionId, size, team;
	TCHAR stringID[30];
	//인원수에 맞게 캐릭터를 생성해 준다
	for( int i=0; i<count; ++i )
	{
		ZeroMemory( stringID, 30 );
		packet >> sessionId;
		packet >> size;
		packet.GetData( stringID, size );
		packet >> team;

		//캐릭터 생성
		CharObj* tmpChar = GetCharMgr.GetCharSpace();
		tmpChar->Init();
		tmpChar->SetIndexId( sessionId );
		tmpChar->SetID( stringID );
		tmpChar->SetTeam( team );
		tmpChar->SetPosition( 10.f, 0.f, 10.f );
	}

	//게임 proc을 활성화 하고 인원을 설정해 준다
	tmpGame->StartGame( count );

	//게임을 시작해도 된다는 패킷을 Lobby로 보낸다
	SendStartOK( roomNum );
}

//--------------------------------------
// Client와의 커뮤니케이션
//--------------------------------------
void GameSession::RecvInGame( SPacket &packet )
{
	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	//test////////////////////////////////////////////////////////////////////
	//지금은 무조건 1번 방....1번만 열려 있음
	roomNum = 1;
	//////////////////////////////////////////////////////////////////////////

	//캐릭터를 검색
	m_myCharInfo = GetCharMgr.FindCharAsSessionId( sessionId );
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvInGame()\n해당 캐릭터를 찾지 못했습니다.\n\n") );
		return;
	}
	
	m_myGameProc = GetGameMgr.FindGame( roomNum );
	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvInGame()\n해당 게임 프로세스를 찾지 못했습니다.\n\n") );
		return;
	}

	//방에 나를 등록
	m_myGameProc->AddPlayer( this );

	//방의 사람들의 정보(내 정보 포함)를 나에게보냄
	SendOtherCharInfoToMe();

	//방의 사람들에게 내 정보를 전송
	SendMyCharInfoToInGamePlayer();
}

void GameSession::RecvMoveChar( SPacket &packet )
{
	int state, dirInt;
	POINT3 pos, dir;
	packet >> state;
	//위치
	packet >> pos.m_X;
	packet >> pos.m_Y;
	packet >> pos.m_Z;
	//방향
	packet >> dir.m_X;
	packet >> dir.m_Y;
	packet >> dir.m_Z;
	packet >> dirInt;

	{
		SSynchronize sync( m_myCharInfo );

		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvMoveChar()\n캐릭터 설정에 문제가 있습니다.\n\n") );
			return;
		}

		m_myCharInfo->SetState( state );
		m_myCharInfo->SetPosition( pos );
		m_myCharInfo->SetDirection( dir.m_X, dir.m_Y, dir.m_Z );
		m_myCharInfo->SetDirInt( dirInt );
	}

	SendMoveChar();
}

//======================================
// 보내는 패킷 처리
//======================================
//--------------------------------------
// Lobby Srv와의 커뮤니케이션
//--------------------------------------

BOOL GameSession::SendStartFaild( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_FAILD );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendStartOK( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_START_OK );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameEnd( int roomNum )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_GAME_END );
	sendPacket << roomNum;

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

BOOL GameSession::SendToSrvThatPlayerDisconnect( int sessionId )
{
	SPacket sendPacket;
	sendPacket.SetID( GL_PLAYER_DISCONNECT );
	sendPacket << m_myCharInfo->GetIndexId();

	GetSrvNet.SendToLobbyServer( sendPacket );

	return TRUE;
}

//--------------------------------------
// Client와의 커뮤니케이션
//--------------------------------------

BOOL GameSession::SendOtherCharInfoToMe()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//게임내의 모두의 정보를 담는다.
	m_myGameProc->PackageAllPlayerInGame( sendPacket );

	int retval = SendPacket( sendPacket );
	if( retval != sendPacket.GetPacketSize() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMyCharInfoToInGamePlayer()\n전송된 패킷크기와 패킷의 크기 일치 하지 않습니다.\n\n") );
		return FALSE;
	}
	return TRUE;
}

BOOL GameSession::SendMyCharInfoToInGamePlayer()
{
	if( m_myCharInfo == NULL || m_myGameProc == NULL )
		return FALSE;

	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHARINFO_INGAME );

	//내 정보만 넣을꺼
	sendPacket << (int)1;
	PackageMyInfo( sendPacket );

	//나 빼고 다 보낸다
	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendCharDisconnect()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHAR_DISCONNECT );

	//자신의 세션 ID를 넣어 준다
	sendPacket << m_myCharInfo->GetIndexId();

	if( m_myGameProc == NULL )
		return FALSE;

	//나는 이미 빠져 있으니 모두에게 보내도 된다.
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendMoveChar()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_MOVE_CHAR );

	{
		SSynchronize sync( m_myCharInfo );

		sendPacket << m_myCharInfo->GetIndexId();
		sendPacket << m_myCharInfo->GetState();
		sendPacket << m_myCharInfo->GetPosX();
		sendPacket << m_myCharInfo->GetPosY();
		sendPacket << m_myCharInfo->GetPosZ();
		sendPacket << m_myCharInfo->GetDirX();
		sendPacket << m_myCharInfo->GetDirY();
		sendPacket << m_myCharInfo->GetDirZ();
		sendPacket << m_myCharInfo->GetDirInt();
	}

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMoveChar()\n방정보에 문제가 있습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}
