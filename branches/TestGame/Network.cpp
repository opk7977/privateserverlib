#include "Network.h"
#include "main.h"

#ifdef _DEBUG
	#include "SLogger.h"
#endif

#include "TestGProtocol.h"
#include "CharacterMgr.h"

extern BOOL isStart;
extern int sessionID;

extern int Min;
extern int Sec;

//������ �����Լ�
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
	if( !m_srvSock.Init() )
	{
#ifdef _DEBUG
 		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] ������ Init() ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
 	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] ������ Init() ����\n") );
#endif

	if( !m_srvSock.CreateSocket() )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] ������ ���� ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] ������ ���� ����\n") );
#endif

	//������ ���� ���� non�� ������ �� �ش�.
	if( isNon )
	{
		if( !m_srvSock.SetNonBlkSock() )
		{
#ifdef _DEBUG
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] ���� nonblock���� ����\n") );
#endif
			return FALSE;
		}
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] ���� nonblock���� ����\n") );
#endif
	}

	return TRUE;
}

BOOL Network::ConnectToSrv( char* ipAddr, int port )
{
	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL);

	//������ ����
	unsigned int threadId = 0;
	m_recvThread = (HANDLE)_beginthreadex( NULL, 0, &::_RecvFunc, (void*)this, 0, &threadId );
	if( !m_recvThread )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::ConnectToSrv()] ������ ������ ����\n") );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::ConnectToSrv()] ������ ������ ����\n") );
#endif

	if( !m_srvSock.ConnectSock( ipAddr, port ) )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, "[Network::ConnectToSrv()] �������� ���ῡ ���� �Ͽ����ϴ� IP:%d/ Port:%d\n", ipAddr, port );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Network::ConnectToSrv()] �������� ���ῡ ���� �Ͽ����ϴ� IP:%d/ Port:%d\n", ipAddr, port );
#endif

	//���� ��ȣ
	SetEvent( m_startWork );

	return TRUE;
}

BOOL Network::Reconnect( char* ipAddr, int port )
{
	//======================================
	// �켱 ���� ����
	//======================================
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Reconnect()] ���������� ���� ������ �����ϴ�.\n") );
#endif
	ResetEvent( m_startWork );

	m_srvSock.Release();

	//======================================
	// �ٽ� ����
	//======================================
	Init( TRUE );

	if( !m_srvSock.ConnectSock( ipAddr, port ) )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, "[Network::Reconnect()] �������� ���ῡ ���� �Ͽ����ϴ� IP:%d/ Port:%d\n", ipAddr, port );
#endif
		return FALSE;
	}
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Network::Reconnect()] �������� ���ῡ ���� �Ͽ����ϴ� IP:%d/ Port:%d\n", ipAddr, port );
#endif

	//���� ��ȣ
	SetEvent( m_startWork );

	return TRUE;
}

void Network::DisConnect()
{
	ResetEvent( m_startWork );
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network] �������� ������ �����ϴ�.\n") );
#endif
	m_srvSock.Release();
}

void Network::Release()
{
	ResetEvent( m_startWork );
	m_srvSock.Release();
}

unsigned int Network::RecvEvent()
{
	const int tmpbufSize = 512;

	char buffer[DEFAULT_BUFFER_SIZE] = {0,};
	char *tmpbuf = buffer;
	//char tmp[512];

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	while(1)
	{
		Sleep( 10 );

		WaitForSingleObject( m_startWork, INFINITE );
		retval = recv( m_srvSock.GetSocket(), tmpbuf, tmpbufSize, 0 );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
#ifdef _DEBUG
 				GetLogger.ErrorLog( errorCode, _T("[Network::RecvEvent()] ") );
				GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::RecvEvent()] ������ ������ϴ�\n") );
#endif
				return FALSE;
			}
		}
		else
		{
			recvSize += retval;

			while( recvSize > 0 )
			{
				m_recvPack.CopyToPacket( buffer, recvSize );

				if( m_recvPack.IsValidPacket() && m_recvPack.GetPacketSize() <= recvSize )
				{
					//��Ŷ�� ó�� �� �� �ִ� �Լ� ȣ��
					PacketParsing( &m_recvPack );

					//Ȥ�� ���� ��Ŷ�� �Ѱ� �̻��̶�� �� Ȯ���ؾ� �Ѵ�
					recvSize -= m_recvPack.GetPacketSize();
					if( recvSize > 0 )
					{
						CopyMemory( buffer, ( m_recvPack.GetDataBufferPtr()+recvSize-4 ), recvSize );
						tmpbuf += recvSize;
					}
				}
				else
				{
					//�ƴϸ� ���ư��� �� ����.
					tmpbuf += recvSize;
					break;
				}
			}
		}
	}
	return 0;
}

void Network::PacketParsing( SPacket* packet )
{
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::PacketParsing] Complete Recv Packet / size : %d\n"), packet->GetPacketSize() );
#endif

	switch( packet->GetID() )
	{
	case SC_TGAME_CONNECT_OK:
		RecvConnectOK();
		break;
	case SC_TGAME_STARTGAME:
		RecvStartGame();
		break;
	case SC_TGAME_COUNTDWON_TIME:
		RecvTimeCountDwon();
		break;
	case SC_TGAME_STATEROTATE:
		RecvChangeRotation( packet );
		break;
	case SC_TGAME_CREATECHAR:
		RecvCreateCharacter( packet );
		break;
	case SC_TGAME_ENDGAME:
		RecvEndGame();
		break;
	case SC_TGAME_STATECHANGE:
		RecvChangeState( packet );
		break;
	case SC_TGAME_DELETECHAR:
		RecvDeleteCharacter( packet );
		break;
	}
}

int Network::SendPacket( SPacket* packet )
{
	int retval = send( m_srvSock.GetSocket(), packet->GetDataBufferPtr(), packet->GetPacketSize(), 0 );

	if( retval < 0 )
	{
#ifdef _DEBUG
		GetLogger.ErrorLog( WSAGetLastError(), _T("[Network::SendPacket()] ") );
#endif
	}
	else if( retval == 0 )
	{
		//���� ����
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::SendPacket()] �������� ������ ������ϴ�.\n") );
#endif
		Release();
	}
	else
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::SendPacket()] Complete Send Packet/ size : %d\n"), retval );
#endif
	}
	return retval;
}

//==============================================================

void Network::RecvConnectOK()
{
	//�� ������ ������./////test//////////////////////////////////////////////
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Networsk] RecvConnectOK()\n") );
#endif

	//sessionID, roomNo
	SendMyInfo( sessionID, 1 );
	//////////////////////////////////////////////////////////////////////////
}

void Network::RecvCreateCharacter( SPacket* packet )
{
	int count;
	(*packet) >> count;

#ifdef _DEBUG
  	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvCreateCharacter--------------------------------------\n")
  												  _T("\tPACKET_ID: SC_TGAME_CREATECHAR / PACKET_SIZE: %d\n")
  												  _T("\tPlayerCount: %d\n")
  												  , packet->GetPacketSize()
												  , count );
#endif

	//���� ����
	int session, state;
	float dirx, diry, dirz, posx, posy, posz;

	for( int i=0; i<count; ++i )
	{
		(*packet) >> session;
		(*packet) >> state;
		(*packet) >> dirx;
		(*packet) >> diry;
		(*packet) >> dirz;
		(*packet) >> posx;
		(*packet) >> posy;
		(*packet) >> posz;

		//ĳ���� ����
		GetCharMgr.CreateCharacter( session, (int)posx, (int)posy, (int)posz, (int)dirx, state );


		//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\t---------------------------------------------\n")
													  _T("\tSessionId	: %d\n")
													  _T("\tState		: %d\n")
													  _T("\tDirection	: %f, %f, %f\n")
													  _T("\tPos(x/y/z)	: %f, %f, %f\n")
													  , session
													  , state
													  , dirx, diry, dirz
													  , posx, posy, posz );
#endif
		//////////////////////////////////////////////////////////////////////////
	}

	if( GetCharMgr.GetNowPlayerCount() == GetCharMgr.GetPlayerCount() )
		SendReadyOK();
}

void Network::RecvStartGame()
{
	//ȭ���� �����Ѵ�.
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvStartGame\n") );
#endif
	isStart = TRUE;
}

void Network::RecvChangeRotation( SPacket* packet )
{
	int session, rot;

	(*packet) >> session;
	(*packet) >> rot;

#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvChangeRotation--------------------------------------\n")
													_T("\tPACKET_ID: SC_TGAME_STATECHANGE / PACKET_SIZE: %d\n")
													_T("\tSession		: %d\n")
													_T("\tState		: %d\n")
													_T("[Network] -------------------------------------RecvChangeRotation\n")
													, packet->GetPacketSize()
													, session
													, rot );
#endif


	/////����κФ�������������
}

void Network::RecvChangeState( SPacket* packet )
{
	int session, state;
	float dirx, diry, dirz, posx, posy, posz;

	(*packet) >> session;
	(*packet) >> state;
	(*packet) >> dirx;
	(*packet) >> diry;
	(*packet) >> dirz;
	(*packet) >> posx;
	(*packet) >> posy;
	(*packet) >> posz;


	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvChangeState--------------------------------------\n")
												  _T("\tPACKET_ID: SC_TGAME_STATECHANGE / PACKET_SIZE: %d\n")
												  _T("\tSession		: %d\n")
												  _T("\tState		: %d\n")
												  _T("\tDirection	: %f, %f, %f\n")
												  _T("\tPos(x/y/z)	: %f, %f, %f\n")
												  _T("[Network] -------------------------------------RecvChangeState\n")
												  , packet->GetPacketSize()
												  , session
												  , state
												  , dirx, diry, dirz
												  , posx, posy, posz );
#endif
	//////////////////////////////////////////////////////////////////////////

	//�� ������ ���� ���ص� ��
	if( GetCharMgr.GetHeroId() == session )
		return;

	CCharacter* tmpChar;
	tmpChar = GetCharMgr.FindChar( session );
	if( tmpChar == NULL )
	{
#ifdef _DEBUG
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network] RecvChangeState() SessionId�� �߸��Ǿ����ϴ�.\n") );
#endif
		return;
	}

	tmpChar->SetDir( (int)dirx );
	tmpChar->SetState( state );
	//if( state != CCharacter::STATE_STAND )
		tmpChar->SetPos( (int)posx, (int)posy, (int)posz );
}

void Network::RecvDeleteCharacter( SPacket* packet )
{
	int session;
	(*packet) >> session;

	GetCharMgr.DeleteCharacter( session );
}

void Network::RecvTimeCountDwon()
{
	//�ð��� ����ߴٴ� ��Ŷ
	if( --Sec < 0 )
	{
		Sec = 59;
		--Min;
	}
}

void Network::RecvEndGame()
{
	//������ ����Ǿ��ٴ� ��Ŷ
	isStart = FALSE;
}

//--------------------------------------------------------------

BOOL Network::SendMyInfo( int sessionId, int roomNo )
{
	SPacket sendPacket;

	sendPacket.SetID( CS_TGAME_SENDINFO );
	sendPacket << sessionId;
	sendPacket << roomNo;

	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendMyInfo--------------------------------------\n")
												  _T("\tPACKET_ID: CS_TGAME_SENDINFO / PACKET_SIZE: %d\n")
												  _T("\tsessionId	  : %d\n")
												  _T("\troomNo	  : %d\n")
												  _T("[Network] -------------------------------------SendMyInfo\n")
												  , sendPacket.GetPacketSize()
												  , sessionId
												  , roomNo );
#endif
	//////////////////////////////////////////////////////////////////////////

	int retval = SendPacket( &sendPacket );
	
	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL Network::SendReadyOK()
{
	SPacket sendPacket;

	sendPacket.SetID( CS_TGAME_READYOK );

	int retval = SendPacket( &sendPacket );

	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL Network::SendChangeRotation( int sessionId, int rot )
{
	SPacket sendPacket;

	sendPacket.SetID( CS_TGAME_STATEROTATE );
	sendPacket << sessionId;
	sendPacket << rot;


#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendChangeRotation--------------------------------------\n")
													_T("\tPACKET_ID: CS_TGAME_STATECHANGE / PACKET_SIZE: %d\n")
													_T("\tsessionId	: %d\n")
													_T("\rotation	: %d\n")
													_T("[Network] -------------------------------------SendChangeRotation\n")
													, sendPacket.GetPacketSize()
													, sessionId
													, rot );
#endif

	int retval = SendPacket( &sendPacket );

	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL Network::SendChangeState( int sessionId, int state, float dirX, float dirY, float dirZ, float posX, float posY, float posZ )
{
	SPacket sendPacket;

	sendPacket.SetID( CS_TGAME_STATECHANGE );
	sendPacket << sessionId;
	sendPacket << state;
	sendPacket << dirX;
	sendPacket << dirY;
	sendPacket << dirZ;
	sendPacket << posX;
	sendPacket << posY;
	sendPacket << posZ;

	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendChangeState--------------------------------------\n")
												  _T("\tPACKET_ID: CS_TGAME_STATECHANGE / PACKET_SIZE: %d\n")
												  _T("\tsessionId	: %d\n")
												  _T("\tstate		: %d\n")
												  _T("\tdir(x,y,z)	: %f, %f, %f\n")
												  _T("\tpos(x,y,z)	: %f, %f, %f\n")
												  _T("[Network] -------------------------------------SendChangeState\n")
												  , sendPacket.GetPacketSize()
												  , sessionId
												  , state
												  , dirX, dirY, dirZ
												  , posX, posY, posZ );
#endif
	//////////////////////////////////////////////////////////////////////////

	int retval = SendPacket( &sendPacket );

	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}
