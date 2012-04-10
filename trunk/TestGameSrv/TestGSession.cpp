#include "TestGSession.h"
#include "TestGProtocol.h"

#include "GProcMgr.h"
#include "GameProc.h"

#include "SSynchronize.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

#include <time.h>

SServerObj		g_sbCritical;

SIMPLEMENT_DYNAMIC(TestGSession)
SIMPLEMENT_DYNCREATE(TestGSession)

TestGSession::TestGSession(void)
{
	Init();
}

TestGSession::~TestGSession(void)
{
}

void TestGSession::OnCreate()
{
	SSession::OnCreate();

	//연결이 성공 패킷을 보냄
	SendPacket( SC_TGAME_CONNECT_OK );
}

void TestGSession::Init()
{
	m_iSession = 0;
	m_iGameNo = 0;
	m_iState = 0;
	m_iRotate = 0;

	m_iDirX = 0;
	m_iDirY = 0;
	m_iDirZ = 1;

	m_iPosX = 0;
	m_iPosY = 0;
	m_iPosZ = 0;
}

void TestGSession::OnDestroy()
{
	if( m_iGameNo != 0 )
	{
		//정보가 등록되지 않은 사람이면 그냥 지워 준다....
		GameProc* tmpGame;
		tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
		if( tmpGame != NULL )
		{
			if( tmpGame->IsPlay() )
			{
				//게임 방에서 자신을 지워 주고
				tmpGame->DelPlayer( this );

				//내가 접속을 종료 했다는 패킷을 모두에게 보낸다
				SendDeleteCharacter();

			}
		}
	}

	//데이터는 날려주고
	Init();

	SSession::OnDestroy();

}

int TestGSession::GetSessionId()
{
	return m_iSession;
}

int TestGSession::GetGameNo()
{
	return m_iGameNo;
}

void TestGSession::SetState( int state )
{
	m_iState = state;
}

int TestGSession::GetState()
{
	return m_iState;
}

void TestGSession::SetDir( float x, float y, float z )
{
	m_iDirX = x;
	m_iDirY = y;
	m_iDirZ = z;
}

float TestGSession::GetDirX()
{
	return m_iDirX;
}

float TestGSession::GetDirY()
{
	return m_iDirY;
}

float TestGSession::GetDirZ()
{
	return m_iDirZ;
}

void TestGSession::SetPos( float x, float y, float z )
{
	m_iPosX = x;
	m_iPosY = y;
	m_iPosZ = z;
}

float TestGSession::GetPosX()
{
	return m_iPosX;
}

float TestGSession::GetPosY()
{
	return m_iPosY;
}

float TestGSession::GetPosZ()
{
	return m_iPosZ;
}

//==============================================================

void TestGSession::PacketParsing( SPacket& packet )
{

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[TestGSession::PacketParsing()] Complete Recv Packet / size : %d\n"), packet.GetPacketSize() );
#endif

	switch( packet.GetID() )
	{
	case CS_TGAME_SENDINFO:
		RecvInfo( packet );
		break;
	case CS_TGAME_READYOK:
		RecvReadyOK();
		break;
	case CS_TGAME_STATEROTATE:
		RecvChangRotate( packet );
		break;
	case CS_TGAME_STATECHANGE:
		RecvChangeState( packet );
		break;
	}
}

//--------------------------------------------------------------

void TestGSession::SendCreateCharacter()
{
	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );

	//접속한 사람이 나 혼자면(2명이 안되면) 안보내도 됨
	if( tmpGame->GetNowPlayerCount() < 2 )
		return;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendCreateCharacter--------------------------------------\n")
												  _T("\tPlayerCount: %d\n")
												  , tmpGame->GetNowPlayerCount()-1 );
#endif
	//--------------------------------------------------------------

	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_CREATECHAR );
	//나 빼고
	sendPacket << (int)(tmpGame->GetNowPlayerCount()-1);
	
	{
		SSynchronize sync( &g_sbCritical );
		//나 뺴고 모든 player를 패킷에 담는다
		tmpGame->PackageAllPlayer( *this, sendPacket );
	}
	//--------------------------------------------------------------
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\tPACKET_ID: SC_TGAME_CREATECHAR / PACKET_SIZE: %d\n")
												  _T("[TestGSession] -------------------------------------SendCreateCharacter\n")
												  , sendPacket.GetPacketSize() );
#endif
	//////////////////////////////////////////////////////////////////////////

	//만들어진 패킷을 보낸다.
	SendPacket( sendPacket );
}

void TestGSession::SendCreateMyCharacter()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_CREATECHAR );

	//우선 만들어야 하는건 나 하나
	sendPacket << 1;

	//정보를 넣는다
	sendPacket << m_iSession;
	sendPacket << m_iState;
	sendPacket << m_iDirX;
	sendPacket << m_iDirY;
	sendPacket << m_iDirZ;
	sendPacket << m_iPosX;
	sendPacket << m_iPosY;
	sendPacket << m_iPosZ;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendCreateMyCharacter--------------------------------------\n")
												  _T("\tPACKET_ID: SC_TGAME_CREATECHAR / PACKET_SIZE: %d\n")
												  _T("\tPlayerCount	: 1\n")
												  _T("\tSessionId	: %d\n")
												  _T("\tState		: %d\n")
												  _T("\tDirection	: %f, %f, %f\n")
												  _T("\tPos(x/y/z)	: %f, %f, %f\n")
												  _T("[TestGSession] -------------------------------------SendCreateMyCharacter\n")
												  , sendPacket.GetPacketSize()
												  , m_iSession
												  , m_iState
												  , m_iDirX, m_iDirY, m_iDirZ
												  , m_iPosX, m_iPosY, m_iPosZ );
#endif
	//////////////////////////////////////////////////////////////////////////

	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
	if( tmpGame == NULL )
	{
		//이미 방이 지워 진듯?...
#ifdef _DEBUG
		GetLogger.PutLog( 0, _T("[TestGSession] SendCreateMyCharacter() 방이 존재하지 않습니다.\n") );
#endif
		return;
	}
	tmpGame->SendAllPlayer( sendPacket );
}

void TestGSession::SendStartGame()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_STARTGAME );

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendStartGame--------------------------------------\n")
												  _T("\tPACKET_ID: SC_TGAME_STARTGAME / PACKET_SIZE: %d\n")
												  _T("[TestGSession] -------------------------------------SendStartGame\n")
												  , sendPacket.GetPacketSize() );
#endif
	//////////////////////////////////////////////////////////////////////////

	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
	if( tmpGame == NULL )
	{
		//이미 방이 지워 진듯?...
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendStartGame() 방이 존재하지 않습니다.\n") );
#endif
		return;
	}
	tmpGame->SendAllPlayer( sendPacket );
}

void TestGSession::SendChangeRotate()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_STATEROTATE );

	sendPacket << m_iSession;
	sendPacket << m_iRotate;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendChangeRotate--------------------------------------\n")
													_T("\tPACKET_ID: SC_TGAME_STATEROTATE / PACKET_SIZE: %d\n")
													_T("\tsessionId: %d\n")
													_T("\trotate: %d\n")
													_T("[TestGSession] -------------------------------------SendChangeRotate\n")
													, sendPacket.GetPacketSize()
													, m_iSession
													, m_iRotate );
#endif
	//////////////////////////////////////////////////////////////////////////

	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
	if( tmpGame == NULL )
	{
		//이미 방이 지워 진듯?...
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendStartGame() 방이 존재하지 않습니다.\n") );
#endif
		return;
	}
	tmpGame->SendAllPlayer( sendPacket );
}

void TestGSession::SendChangeState()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_STATECHANGE );

	sendPacket << m_iSession;
	sendPacket << m_iState;
	sendPacket << m_iDirX;
	sendPacket << m_iDirY;
	sendPacket << m_iDirZ;
	sendPacket << m_iPosX;
	sendPacket << m_iPosY;
	sendPacket << m_iPosZ;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendChangeState--------------------------------------\n")
												  _T("\tPACKET_ID: SC_TGAME_STATECHANGE / PACKET_SIZE: %d\n")
												  _T("\tSessionId	: %d\n")
												  _T("\tState		: %d\n")
												  _T("\tDirection	: %f, %f, %f\n")
												  _T("\tPos(x/y/z)	: %f, %f, %f\n")
												  _T("[TestGSession] -------------------------------------SendChangeState\n")
												  , sendPacket.GetPacketSize()
												  , m_iSession
												  , m_iState
												  , m_iDirX, m_iDirY, m_iDirZ
												  , m_iPosX, m_iPosY, m_iPosZ );
#endif
	//////////////////////////////////////////////////////////////////////////

	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
	if( tmpGame == NULL )
	{
		//이미 방이 지워 진듯?...
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendChangeState() 방이 존재하지 않습니다.\n") );
#endif
		return;
	}
	tmpGame->SendAllPlayer( sendPacket );
}

void TestGSession::SendDeleteCharacter()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_DELETECHAR );

	sendPacket << m_iSession;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendDeleteCharacter--------------------------------------\n")
												  _T("\tPACKET_ID: SC_TGAME_DELETECHAR / PACKET_SIZE: %d\n")
												  _T("\tSessionId: %d\n")
												  _T("[TestGSession] -------------------------------------SendDeleteCharacter\n")
												  , sendPacket.GetPacketSize()
												  , m_iSession );
#endif
	//////////////////////////////////////////////////////////////////////////

	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
	if( tmpGame == NULL )
	{
		//이미 방이 지워 진듯?... 
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendDeleteCharacter() 방이 존재하지 않습니다.\n") );
#endif
		return;
	}
	tmpGame->SendAllPlayer( sendPacket );
}

//--------------------------------------------------------------

void TestGSession::RecvInfo( SPacket& packet )
{
	//받아 온 정보를 해석(?)
	int session, roomNo;

	packet >> session;
	packet >> roomNo;

	m_iSession = session;
	m_iGameNo = roomNo;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] RecvInfo--------------------------------------\n")
												  _T("\tPACKET_ID: CS_TGAME_SENDINFO / PACKET_SIZE: %d\n")
												  _T("\tSession: %d\n")
												  _T("\troomNo : %d\n")
												  _T("[TestGSession] -------------------------------------RecvInfo\n")
												  , packet.GetPacketSize()
												  , session
												  , roomNo );
#endif
	//////////////////////////////////////////////////////////////////////////

	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );

	//현재 방번호에 해당하는 게임proc이 실행중이 아니면 새로 만들어 준다.
	if( tmpGame == NULL )
	{
		tmpGame = GetGPMgr.CreateGameProcess( m_iGameNo, 2 );
		if( tmpGame == NULL )
		{	
			//뭔가 받아온 정보, 혹은 서버의 문제
#ifdef _DEBUG
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[TestGSession::RecvInfo()] 받아온 방번호에 해당하는 게임process가 존재하지 않습니다.\n") );
#endif
			return;
		}
	}

	tmpGame->AddPlayer( this );
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession::RecvInfo()] Add Player\n") );
#endif

	//해당 캐릭터의 좌표값 설정
	srand( (unsigned int)time(NULL) );
	SetPos( (float)(rand()%10), 0, (float)(rand()%10) );

	//나빼고 다른 모두에게 내 정보를 다 보냄
	SendCreateMyCharacter();

	Sleep(10);

	//나는 다른 사람들의 정보를 받아 간다.
	SendCreateCharacter();
}

void TestGSession::RecvReadyOK()
{
	GameProc* tmpGame;
	tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );

	if( tmpGame == NULL )
		return;

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession::RecvReadyOK()] Add ReadyPlayer No: %d\n"), m_iSession );
#endif
	tmpGame->PlayerCountUp();
}

void TestGSession::RecvChangRotate( SPacket& packet )
{
	int session;

	packet >> session;

	if( m_iSession != session )
		return;

	packet >> m_iRotate;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] RecvChangRotate--------------------------------------\n")
		_T("\tPACKET_ID: CS_TGAME_STATEROTATE / PACKET_SIZE: %d\n")
		_T("\tRotate	: %d\n")
		_T("[TestGSession] -------------------------------------RecvChangRotate\n")
		, packet.GetPacketSize()
		,m_iRotate );
#endif
	//////////////////////////////////////////////////////////////////////////

	SendChangeRotate();
}

void TestGSession::RecvChangeState( SPacket& packet )
{
	int session;

	packet >> session;

	if( m_iSession != session )
		return;

	packet >> m_iState;
	packet >> m_iDirX;
	packet >> m_iDirY;
	packet >> m_iDirZ;
	packet >> m_iPosX;
	packet >> m_iPosY;
	packet >> m_iPosZ;

	//======================================
	// log
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] RecvChangeState--------------------------------------\n")
												  _T("\tPACKET_ID: CS_TGAME_STATECHANGE / PACKET_SIZE: %d\n")
												  _T("\tState		: %d\n")
												  _T("\tDirection	: %f, %f, %f\n")
												  _T("\tPos(x/y/z)	: %f, %f, %f\n")
												  _T("[TestGSession] -------------------------------------RecvChangeState\n")
												  , packet.GetPacketSize()
												  , m_iState
												  , m_iDirX, m_iDirY, m_iDirZ
												  , m_iPosX, m_iPosY, m_iPosZ );
#endif
	//////////////////////////////////////////////////////////////////////////

	SendChangeState();
}


