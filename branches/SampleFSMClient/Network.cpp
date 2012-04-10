#include "Network.h"

//#define USE_LOG
#ifdef USE_LOG
	#include "SLogger.h"
#endif


#include "FSMProtocol.h"
#include "CharacterMgr.h"

extern BOOL g_isStart;

unsigned int __stdcall _RecvFunc(void* pArg)
{
	return ((Network*)pArg)->RecvEvent();
}


Network::Network(void)
{
}

Network::~Network(void)
{
	Release();
}

BOOL Network::Init( BOOL isNon /*= FALSE */ )
{
	if( !m_mySock.Init() )
	{
#ifdef USE_LOG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] 소켓의 Init() 실패\n") );
#endif
		return FALSE;
	}
#ifdef USE_LOG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] 소켓의 Init() 성공\n") );
#endif

	if( !m_mySock.CreateSocket() )
	{
#ifdef USE_LOG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] 소켓의 생성 실패\n") );
#endif
		return FALSE;
	}
#ifdef USE_LOG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] 소켓의 생성 성공\n") );
#endif


	//설정에 따라 소켓 non블럭 설정을 해 준다.
	if( isNon )
	{
		if( !m_mySock.SetNonBlkSock() )
		{
#ifdef USE_LOG
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] 소켓 nonblock설정 실패\n") );
#endif
			return FALSE;
		}
#ifdef USE_LOG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] 소켓 nonblock설정 성공\n") );
#endif
	}

	return TRUE;
}

BOOL Network::ConnectSrv( char* ipAddr, int port )
{
	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL);

	//쓰레드 생성
	unsigned int threadId = 0;
	m_recvThread = (HANDLE)_beginthreadex( NULL, 0, &::_RecvFunc, (void*)this, 0, &threadId );
	if( !m_recvThread )
	{
#ifdef USE_LOG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::ConnectSrv()] 쓰레드 생성에 실패\n") );
#endif
		return FALSE;
	}
#ifdef USE_LOG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::ConnectSrv()] 쓰레드 생성에 성공\n") );
#endif

	if( !m_mySock.ConnectSock( ipAddr, port ) )
	{
#ifdef USE_LOG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::ConnectSrv()] 서버와의 연결에 실패 하였습니다\n") );
#endif
		return FALSE;
	}
#ifdef USE_LOG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::ConnectSrv()] 서버와의 연결에 성공하였습니다\n") );
#endif

	//연결 신호
	SetEvent( m_startWork );

	return TRUE;
}

void Network::Release()
{
	m_mySock.Release();
}

//////////////////////////////////////////////////////////////////////////

unsigned int Network::RecvEvent()
{
	const int tmpbufSize = 512;

	char buffer[1024] = {0,};
	char tmpbuf[512];
	char tmp[512];

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	WaitForSingleObject( m_startWork, INFINITE );

	while(1)
	{
		memset( tmpbuf, 0, tmpbufSize );

		retval = recv( m_mySock.GetSocket(), tmpbuf, tmpbufSize, 0 );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
#ifdef USE_LOG
			GetLogger.ErrorLog( errorCode, _T("[Network::RecvEvent()] ") );
			GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::RecvEvent()] 접속이 끊겼습니다\n") );
#endif
				return FALSE;
			}
		}
		else
		{
			recvSize += retval;

			while( recvSize > 0 )
			{
				CopyMemory( buffer, tmpbuf, recvSize );
				//strcat_s( buffer, strlen(tmpbuf), tmpbuf );
				m_recvPack.CopyToPacket( buffer, recvSize );

				if( m_recvPack.IsValidPacket() && m_recvPack.GetPacketSize() <= recvSize )
				{
					//패킷을 처리 할 수 있는 함수 호출
					PacketParsing( &m_recvPack );

					//혹시 받은 패킷이 한개 이상이라면 또 확인해야 한다
					recvSize -= m_recvPack.GetPacketSize();
					if( recvSize > 0 )
					{
						CopyMemory( tmp, (buffer + m_recvPack.GetPacketSize()), recvSize );
						CopyMemory( buffer, tmp, recvSize );
					}
				}
				else
				{
					//아니면 돌아가서 더 받자.
					break;
				}
			}
		}
	}

	return 0;
}

void Network::PacketParsing( SPacket* packet )
{
#ifdef USE_LOG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::PacketParsing()] Completed Recv Pakcet / bytes: %d\n"), packet->GetPacketSize() );
#endif

	switch( packet->GetID() )
	{
	case SERVER_CONNECT_OK:
		CreateCharacter( packet );
		break;
	case SERVER_CHANGE_CHARACTER:
		ChangeCharacter( packet );
		break;
	}
}

void Network::CreateCharacter( SPacket* packet )
{
	int charCount = 0;

	(*packet) >> charCount;
	//우선 캐릭터를 만들고
	GetCharMgr.CreateCharacter( charCount );

	NPCObj tmpObj;
	Character* tmpChar;

	//캐릭터 셋팅을 모두 해 주고
	for( int i=0; i<charCount; ++i )
	{
		(*packet) >> tmpObj.charId;
		(*packet) >> tmpObj.charState;
		(*packet) >> tmpObj.charSpeed;
		(*packet) >> tmpObj.charX;
		(*packet) >> tmpObj.charY;
		(*packet) >> tmpObj.goalX;
		(*packet) >> tmpObj.goalY;


#ifdef USE_LOG
		//======================================
		// Log Code
		//======================================
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] recv Packet==========================================\n")
													  _T("\tPacketID	: SERVER_CONNECT_OK\n")
													  _T("\tcharID		: %d\n")
													  _T("\tcharState	: %s\n")
													  _T("\tcharSpeed	: %d\n")
													  _T("\tcharX, Y	: %d, %d\n")
													  _T("\tgoalX, Y	: %d, %d\n")
													  _T("[Network] ==========================================recv Packet\n")
													  , tmpObj.charId
													  , (tmpObj.charState == 0 ) ? _T("stand") : _T("walk")
													  , tmpObj.charX, tmpObj.charY
													  , tmpObj.charSpeed
													  , tmpObj.goalX, tmpObj.goalY );
		//////////////////////////////////////////////////////////////////////////
#endif

		tmpChar = GetCharMgr.FindChar( tmpObj.charId );

		tmpChar->SetState( tmpObj.charState );
		tmpChar->SetTransition( tmpObj.charState );
		tmpChar->SetID( tmpObj.charId );
		tmpChar->SetSpeed( tmpObj.charSpeed );
		tmpChar->SetPosition( tmpObj.charX, tmpObj.charY );
		tmpChar->SetGoal( tmpObj.goalX, tmpObj.goalY );
	}

	//메인에서 프레임이 돌아 갈 수 있게 한다.
	g_isStart = TRUE;
}

void Network::ChangeCharacter( SPacket* packet )
{
	NPCObj tmpObj;
	Character* tmpChar;

	(*packet) >> tmpObj.charId;
	(*packet) >> tmpObj.charState;
	(*packet) >> tmpObj.charX;
	(*packet) >> tmpObj.charY;
	(*packet) >> tmpObj.goalX;
	(*packet) >> tmpObj.goalY;


#ifdef USE_LOG
	//======================================
	// Log Code
	//======================================
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] recv Packet==========================================\n")
												  _T("\tPacketID	: SERVER_CHANGE_CHARACTER\n")
												  _T("\tcharID		: %d\n")
												  _T("\tcharState	: %s\n")
												  _T("\tcharX, Y	: %d, %d\n")
												  _T("\tgoalX, Y	: %d, %d\n")
												  _T("[Network] ==========================================recv Packet\n")
												  , tmpObj.charId
												  , (tmpObj.charState == 0 ) ? _T("stand") : _T("walk")
												  , tmpObj.charX, tmpObj.charY
												  , tmpObj.goalX, tmpObj.goalY );
	//////////////////////////////////////////////////////////////////////////
#endif

	tmpChar = GetCharMgr.FindChar( tmpObj.charId );

	tmpChar->SetState( tmpObj.charState );
	tmpChar->SetTransition( tmpObj.charState );
	tmpChar->SetPosition( tmpObj.charX, tmpObj.charY );
	tmpChar->SetGoal( tmpObj.goalX, tmpObj.goalY );
}
