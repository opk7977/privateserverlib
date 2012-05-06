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
	if( isEndGame )
	{
		//게임을 완전히 종료
		//user가 속한 곳의 player들 에게 접속을 끊었음을 알린다.
		if( m_myGameProc != NULL )
		{
			//방에서 나를 제거하고
			m_myGameProc->DelPlayer( this );

			//방 사람들에게 패킷 전송
			SendCharDisconnect();
		}
		
	}
	else
	{
		//로비로 이동
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
	}
}

void GameSession::PacketParsing( SPacket& packet )
{
	switch( packet.GetID() )
	{
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
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
	int state;
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

	{
		SSynchronize sync( m_myCharInfo );

		if( m_myCharInfo == NULL )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::RecvMoveChar()\n캐릭터 설정에 문제가 있습니다.\n\n") );
			return;
		}

		m_myCharInfo->SetState( state );
		m_myCharInfo->SetPosition( pos );
		m_myCharInfo->SetDirection( dir );
	}

	SendMoveChar();
}

//======================================
// 보내는 패킷 처리
//======================================
//--------------------------------------
// Lobby Srv와의 커뮤니케이션
//--------------------------------------

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
	}

	if( m_myGameProc == NULL )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("GameSession::SendMoveChar()\n방정보에 문제가 있습니다.\n\n") );
		return FALSE;
	}

	m_myGameProc->SendAllPlayerInGame( sendPacket, this );

	return TRUE;
}
