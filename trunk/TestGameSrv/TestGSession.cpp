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

	//������ ���� ��Ŷ�� ����
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
		//������ ��ϵ��� ���� ����̸� �׳� ���� �ش�....
		GameProc* tmpGame;
		tmpGame = GetGPMgr.GetGameProcess( m_iGameNo );
		if( tmpGame != NULL )
		{
			if( tmpGame->IsPlay() )
			{
				//���� �濡�� �ڽ��� ���� �ְ�
				tmpGame->DelPlayer( this );

				//���� ������ ���� �ߴٴ� ��Ŷ�� ��ο��� ������
				SendDeleteCharacter();

			}
		}
	}

	//�����ʹ� �����ְ�
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

	//������ ����� �� ȥ�ڸ�(2���� �ȵǸ�) �Ⱥ����� ��
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
	//�� ����
	sendPacket << (int)(tmpGame->GetNowPlayerCount()-1);
	
	{
		SSynchronize sync( &g_sbCritical );
		//�� ���� ��� player�� ��Ŷ�� ��´�
		tmpGame->PackageAllPlayer( *this, sendPacket );
	}
	//--------------------------------------------------------------
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\tPACKET_ID: SC_TGAME_CREATECHAR / PACKET_SIZE: %d\n")
												  _T("[TestGSession] -------------------------------------SendCreateCharacter\n")
												  , sendPacket.GetPacketSize() );
#endif
	//////////////////////////////////////////////////////////////////////////

	//������� ��Ŷ�� ������.
	SendPacket( sendPacket );
}

void TestGSession::SendCreateMyCharacter()
{
	SPacket sendPacket;
	sendPacket.SetID( SC_TGAME_CREATECHAR );

	//�켱 ������ �ϴ°� �� �ϳ�
	sendPacket << 1;

	//������ �ִ´�
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
		//�̹� ���� ���� ����?...
#ifdef _DEBUG
		GetLogger.PutLog( 0, _T("[TestGSession] SendCreateMyCharacter() ���� �������� �ʽ��ϴ�.\n") );
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
		//�̹� ���� ���� ����?...
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendStartGame() ���� �������� �ʽ��ϴ�.\n") );
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
		//�̹� ���� ���� ����?...
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendStartGame() ���� �������� �ʽ��ϴ�.\n") );
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
		//�̹� ���� ���� ����?...
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendChangeState() ���� �������� �ʽ��ϴ�.\n") );
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
		//�̹� ���� ���� ����?... 
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession] SendDeleteCharacter() ���� �������� �ʽ��ϴ�.\n") );
#endif
		return;
	}
	tmpGame->SendAllPlayer( sendPacket );
}

//--------------------------------------------------------------

void TestGSession::RecvInfo( SPacket& packet )
{
	//�޾� �� ������ �ؼ�(?)
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

	//���� ���ȣ�� �ش��ϴ� ����proc�� �������� �ƴϸ� ���� ����� �ش�.
	if( tmpGame == NULL )
	{
		tmpGame = GetGPMgr.CreateGameProcess( m_iGameNo, 2 );
		if( tmpGame == NULL )
		{	
			//���� �޾ƿ� ����, Ȥ�� ������ ����
#ifdef _DEBUG
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[TestGSession::RecvInfo()] �޾ƿ� ���ȣ�� �ش��ϴ� ����process�� �������� �ʽ��ϴ�.\n") );
#endif
			return;
		}
	}

	tmpGame->AddPlayer( this );
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[TestGSession::RecvInfo()] Add Player\n") );
#endif

	//�ش� ĳ������ ��ǥ�� ����
	srand( (unsigned int)time(NULL) );
	SetPos( (float)(rand()%10), 0, (float)(rand()%10) );

	//������ �ٸ� ��ο��� �� ������ �� ����
	SendCreateMyCharacter();

	Sleep(10);

	//���� �ٸ� ������� ������ �޾� ����.
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


