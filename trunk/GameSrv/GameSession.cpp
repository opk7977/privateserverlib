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

		//캐릭터 공간의 정보를 지워 준다.
		GetCharMgr.ReturnCharSpace( m_myCharInfo );
	}

	//세션 초기화
	Init();

	SSession::OnDestroy();
}

void GameSession::PackageMyInfo( SPacket& packet )
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
	case CS_GAME_SYNC:
		RecvGameSync( packet );
		break;
	case CS_GAME_CHANGE_STATE:
		RecvGameChangeState( packet );
		break;
	case CS_GAME_ROTATION:
		RecvGameRotation( packet );
		break;
	case CS_GAME_CHATTING:
		RecvGameChatting( packet );
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
	SSynchronize Sync( this );

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
	SSynchronize Sync( this );

	int sessionId, roomNum;

	packet >> sessionId;
	packet >> roomNum;

	//test////////////////////////////////////////////////////////////////////
	//지금은 무조건 1번 방....1번만 열려 있음
	roomNum = 1;
	//////////////////////////////////////////////////////////////////////////

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::RecvInGame()\n%d번 캐릭터가 %d번게임으로 입장합니다.\n\n") 
					, sessionId, roomNum );

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
	SSynchronize Sync( this );

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

// 	{
// 		SSynchronize sync( m_myCharInfo );

		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvMoveChar()\n캐릭터 설정에 문제가 있습니다.\n\n") );
			return;
		}

		m_myCharInfo->SetState( state );
		m_myCharInfo->SetPosition( pos );
		m_myCharInfo->SetDirection( dir.m_X, dir.m_Y, dir.m_Z );
		m_myCharInfo->SetDirInt( dirInt );

		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvMoveChar()\n")
						_T("%s(%d) 캐릭터가 %d번 상태\n")
						_T("pos(%f, %f, %f)위치")
						_T("/ dir(%f, %f, %f, %i)방향으로 전환\n\n")
						, m_myCharInfo->GetID()
						, m_myCharInfo->GetIndexId()
						, state
						, pos.m_X, pos.m_Y, pos.m_Z
						, dir.m_X, dir.m_Y, dir.m_Z, dirInt );
//	}

	SendMoveChar();
}

void GameSession::RecvGameSync( SPacket &packet )
{
	SSynchronize Sync( this );

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvMoveChar()\n캐릭터 설정에 문제가 있습니다.\n\n") );
		return;
	}

	POINT3 pos, dir;
	//위치
	packet >> pos.m_X;
	packet >> pos.m_Y;
	packet >> pos.m_Z;
	//방향
	packet >> dir.m_X;
	packet >> dir.m_Y;
	packet >> dir.m_Z;

	m_myCharInfo->SetPosition( pos );
	m_myCharInfo->SetDirection( dir );

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::RecvGameSync()\n")
					_T("%s(%d) 캐릭터\n")
					_T("pos(%f, %f, %f)위치")
					_T("/ dir(%f, %f, %f)방향으로 전환\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z );

	SendGameSync();
}

void GameSession::RecvGameChangeState( SPacket &packet )
{
	SSynchronize Sync( this );

	int state;
	packet >> state;

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChangeDir()\n")
						_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_myCharInfo->SetState( state );

	SendGameChangeState();
}

void GameSession::RecvGameRotation( SPacket &packet )
{
	SSynchronize Sync( this );

	POINT3 dir;
	packet >> dir.m_X >> dir.m_Y >> dir.m_Z;

	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameRotation()\n")
						_T("캐릭터 정보가 유효하지 않습니다\n\n") );
		return;
	}

	m_myCharInfo->SetDirection( dir );

	SendGameRotation();
}

void GameSession::RecvGameChatting( SPacket &packet )
{
	SSynchronize Sync( this );

	int size;
	TCHAR tmpChatting[256] = {0,};
	TCHAR Chatting[300] = {0,};

	packet >> size;
	packet.GetData( tmpChatting, size );

	
	if( m_myCharInfo == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::RecvGameChatting()\n캐릭터의 정보가 유효하지 않습니다.\n\n") );
		return;
	}

	swprintf_s( Chatting, _T("[%s] %s"), m_myCharInfo->GetID(), tmpChatting );
	size = _tcslen( Chatting )*sizeof( TCHAR );

	GetLogger.PutLog( SLogger::LOG_LEVEL_CONINFO,
		_T("GameSession::RecvGameChatting()\n%s\n\n"), Chatting );

	SendGameChatting( Chatting, size );
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

BOOL GameSession::SendToSrvThatPlayerDisconnect()
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

BOOL GameSession::SendMoveChar()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_MOVE_CHAR );

// 	{
// 		//SSynchronize sync( m_myCharInfo );
// 		SSynchronize sync( this );

	POINT3 pos = m_myCharInfo->GetPosition();
	POINT3 dir = m_myCharInfo->GetDirection();

	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << m_myCharInfo->GetState();
	sendPacket << pos.m_X << pos.m_Y << pos.m_Z;
	sendPacket << dir.m_X << dir.m_Y << dir.m_Z;
	sendPacket << m_myCharInfo->GetDirInt();

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::SendMoveChar()\n")
					_T("%s(%d) 캐릭터가 %d번 상태\n")
					_T("pos(%f, %f, %f)위치")
					_T("/ dir(%f, %f, %f, %i)방향으로 전환\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, m_myCharInfo->GetState()
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z, m_myCharInfo->GetDirInt() );
//	}

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMoveChar()\n방정보에 문제가 있습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameSync()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_SYNC );

	//{
		//SSynchronize sync( m_myCharInfo );
		//SSynchronize sync( this );

	const POINT3 pos = m_myCharInfo->GetPosition();
	const POINT3 dir = m_myCharInfo->GetDirection();
	

	sendPacket << m_myCharInfo->GetIndexId();
	//sendPacket << m_myCharInfo->GetState();
	sendPacket << pos.m_X;
	sendPacket << pos.m_Y;
	sendPacket << pos.m_Z;

	sendPacket << dir.m_X;
	sendPacket << dir.m_Y;
	sendPacket << dir.m_Z;
	//sendPacket << m_myCharInfo->GetDirInt();
	//}

	GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
					_T("GameSession::SendGameSync()\n")
					_T("%s(%d) 캐릭터\n")
					_T("pos(%f, %f, %f)위치\n")
					_T("dir(%f, %f, %f)방향으로 전환\n\n")
					, m_myCharInfo->GetID()
					, m_myCharInfo->GetIndexId()
					, pos.m_X, pos.m_Y, pos.m_Z
					, dir.m_X, dir.m_Y, dir.m_Z );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMoveChar()\n방정보에 문제가 있습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameChatting( TCHAR* chatting, int size )
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHATTING );

	sendPacket << size;
	sendPacket.PutData( chatting, size );

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChatting()\n방 게임 proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}
	m_myGameProc->SendAllPlayerInGame( sendPacket );

	return TRUE;
}

BOOL GameSession::SendGameChangeState()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_CHANGE_STATE );
	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << m_myCharInfo->GetState();

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameChangeDir()\n")
						_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}

BOOL GameSession::SendGameRotation()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_GAME_ROTATION );

	POINT3 dir = m_myCharInfo->GetDirection();

	sendPacket << m_myCharInfo->GetIndexId();
	sendPacket << dir.m_X << dir.m_Y << dir.m_Z;

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG,
						_T("GameSession::SendGameRotation()\n")
						_T("게임proc정보가 유효하지 않습니다.\n\n") );
		return FALSE;
	}

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
