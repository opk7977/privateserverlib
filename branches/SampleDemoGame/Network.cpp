#include "StdAfx.h"
#include "Network.h"
#include "SLogger.h"

#include "SampleDemoGameDoc.h"
#include "SampleDemoGameView.h"

#include "DemoProtocol.h"
#include "CharacterMgr.h"

unsigned int __stdcall _RecvFunc(void* pArg)
{
	return ((CNetwork*)pArg)->RecvEvent();
}

CNetwork::CNetwork(void)
{
}

CNetwork::~CNetwork(void)
{
	Release();
}

BOOL CNetwork::Init( BOOL isNon )
{
	if( !m_srvSock.Init() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] 소켓의 Init() 실패\n") );
		return FALSE;
	}
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] 소켓의 Init() 성공\n") );

	if( !m_srvSock.CreateSocket() )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] 소켓의 생성 실패\n") );
		return FALSE;
	}
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] 소켓의 생성 성공\n") );


	//설정에 따라 소켓 non블럭 설정을 해 준다.
	if( isNon )
	{
		if( !m_srvSock.SetNonBlkSock() )
		{
			GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::Init()] 소켓 nonblock설정 실패\n") );
			return FALSE;
		}
		GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::Init()] 소켓 nonblock설정 성공\n") );
	}

	return TRUE;
}

BOOL CNetwork::ConnectNet( char* ipAddr, int port )
{
	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL);

	//쓰레드 생성
	unsigned int threadId = 0;
	m_recvThread = (HANDLE)_beginthreadex( NULL, 0, &::_RecvFunc, (void*)this, 0, &threadId );
	if( !m_recvThread )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::ConnectNet()] 쓰레드 생성에 실패\n") );
		return FALSE;
	}
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::ConnectNet()] 쓰레드 생성에 성공\n") );

	if( !m_srvSock.ConnectSock( ipAddr, port ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, "[Network::ConnectNet()] 서버와의 연결에 실패 하였습니다 IP:%d/ Port:%d\n", ipAddr, port );
		return FALSE;
	}
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Network::ConnectNet()] 서버와의 연결에 성공 하였습니다 IP:%d/ Port:%d\n", ipAddr, port );

	//연결 신호
	SetEvent( m_startWork );

	return TRUE;
}

BOOL CNetwork::ReConnectNet( char* ipAddr, int port )
{
	//======================================
	// 우선 열결 끊기
	//======================================
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::ReConnectNet()] 서버변경을 위해 연결을 끊습니다.\n") );

	ResetEvent( m_startWork );

	m_srvSock.Release();

	//======================================
	// 다시 연결
	//======================================
	Init( TRUE );

	if( !m_srvSock.ConnectSock( ipAddr, port ) )
	{
		GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, "[Network::ReConnectNet()] 서버와의 연결에 실패 하였습니다 IP:%d/ Port:%d\n", ipAddr, port );
		return FALSE;
	}
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, "[Network::ReConnectNet()] 서버와의 연결에 성공 하였습니다 IP:%d/ Port:%d\n", ipAddr, port );

	//연결 신호
	SetEvent( m_startWork );

	return TRUE;
}

void CNetwork::DisConnect()
{
	ResetEvent( m_startWork );

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	pDoc->SetConnect( FALSE );
	pDoc->SetLogin( FALSE );

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::DisConnect()] 서버와의 연결을 끊습니다.\n") );

	m_srvSock.Release();
}

void CNetwork::Release()
{
	ResetEvent( m_startWork );
	m_srvSock.Release();
}
//////////////////////////////////////////////////////////////////////////

unsigned int CNetwork::RecvEvent()
{
	const int tmpbufSize = 512;

	char buffer[1024] = {0,};
	char tmpbuf[512];
	char tmp[512];

	int retval;
	INT32 errorCode;
	int recvSize = 0;

	while(1)
	{
		WaitForSingleObject( m_startWork, INFINITE );

		memset( tmpbuf, 0, tmpbufSize );

		retval = recv( m_srvSock.GetSocket(), tmpbuf, tmpbufSize, 0 );

		if( retval == SOCKET_ERROR )
		{
			errorCode = WSAGetLastError();

			if( errorCode != WSAEWOULDBLOCK )
			{
				GetLogger.ErrorLog( errorCode, _T("[Network::RecvEvent()] ") );
				GetLogger.PutLog( SLogger::LOG_LEVEL_WORRNIG, _T("[Network::RecvEvent()] 접속이 끊겼습니다\n") );
				return FALSE;
			}
		}
		else
		{
			recvSize += retval;

			while( recvSize > 0 )
			{
				CopyMemory( buffer, tmpbuf, recvSize );
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

void CNetwork::PacketParsing( SPacket* packet )
{
	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::PacketParsing()] Complete Recv Packet / size : %d\n"), packet->GetPacketSize() );

	switch( packet->GetID() )
	{
	case SC_LOGIN_CONNECT_OK:
		RecvConnectComplete();
		break;
	case SC_LOGIN_NONID:
		RecvResultNonId();
		break;
	case SC_LOGIN_NOTPW:
		RecvResultNotPw();
		break;
	case SC_LOGIN_SUCCESS:
		RecvResultLogin( packet );
		break;
	case SC_GAME_CONNECT_OK:
		RecvConnectCompleteToG();
		break;
	case SC_GAME_CONNECT_FAILED:
		RecvConnectFailed();
		break;
	case SC_GAME_CREATE_CHARACTER:
		RecvCreateCharacter( packet );
		break;
	case SC_GAME_CHANGE_STATE:
		RecvMoveCharacter( packet );
		break;
	case SC_GAME_DELETE_CHARACTER:
		RecvDeleteCharacter( packet );
		break;
	}
}

int CNetwork::SendPacket( SPacket* packet )
{
	int retval = send( m_srvSock.GetSocket(), packet->GetDataBufferPtr(), packet->GetPacketSize(), 0 );

	GetLogger.PutLog( SLogger::LOG_LEVEL_SYSTEM, _T("[Network::SendPacket()] Complete Send Packet / size : %d\n"), retval );

	return retval;
}

//////////////////////////////////////////////////////////////////////////
//======================================
// 보내는 함수들 
//======================================
BOOL CNetwork::SendLogin( TCHAR* id, TCHAR* pw )
{
	SPacket sendPacket;
	//id설정
	sendPacket.SetID( CS_LOGIN_LOGIN );

	int size = _tcslen( id )*sizeof( TCHAR );
	sendPacket << size;
	sendPacket.PutData( id, size );

	size = _tcslen( pw )*sizeof( TCHAR );
	sendPacket << size;
	sendPacket.PutData( pw, size );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendLogin-------------------------------------\n")
												  _T("\tPACKET ID : CS_LOGIN_LOGIN\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tid : %s\n")
												  _T("\tpw : %s\n")
												  _T("[Network] -------------------------------------SendLogin\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , id
												  , pw );
	/////////////////////////////////////////////////////////////////////////////////////////

	int retval;
	retval = SendPacket( &sendPacket );

	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL CNetwork::SendSessionId( int sessionID, TCHAR* charID )
{
	SPacket sendPacket;

	sendPacket.SetID( CS_GAME_SEND_INDECX );

	sendPacket << sessionID;
	int size = _tcslen( charID ) * sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( charID, size );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendSessionId-------------------------------------\n")
												  _T("\tPACKET ID : CS_GAME_SEND_INDECX\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tsessionID	: %d\n")
												  _T("\tchar ID : %s\n")
												  _T("[Network] -------------------------------------SendSessionId\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , sessionID
												  , charID );
	/////////////////////////////////////////////////////////////////////////////////////////


	int retval = SendPacket( &sendPacket );

	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

BOOL CNetwork::SendChangeState( int indexId, int state, int x, int y )
{
	SPacket sendPacket;

	sendPacket.SetID( CS_GAME_CHANGE_STATE );

	sendPacket << indexId;
	sendPacket << state;
	sendPacket << x;
	sendPacket << y;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] SendChangeState-------------------------------------\n")
												  _T("\tPACKET ID : CS_GAME_CHANGE_STATE\n")
												  _T("\tPACKET SIZE / DATA SIZE : %d / %d\n")
												  _T("\tsessionID : %d\n")
												  _T("\tstate : %d\n")
												  _T("\tx, y : %d, %d\n")
												  _T("[Network] -------------------------------------SendChangeState\n")
												  , sendPacket.GetPacketSize()
												  , sendPacket.GetDateFiledSize()
												  , indexId
												  , state
												  , x
												  , y );
	/////////////////////////////////////////////////////////////////////////////////////////


	int retval = SendPacket( &sendPacket );

	if( retval == sendPacket.GetPacketSize() )
		return TRUE;

	return FALSE;
}

//======================================
// 받아 처리하는 함수들
//======================================
void CNetwork::RecvConnectComplete()
{
	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvConnectComplete-------------------------------------\n")
												  _T("\tPACKET ID : SC_LOGIN_CONNECT_OK\n")
												  _T("\t로그인 서버로부터 정상적으로 연결되었다는 패킷을 받음\n")
												  _T("[Network] -------------------------------------RecvConnectComplete\n") );
	/////////////////////////////////////////////////////////////////////////////////////////


	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->SetConnect( TRUE );
}

void CNetwork::RecvResultNonId()
{
	MessageBox( NULL, _T("입력하신 id가 존재하지 않습니다."), _T("Info"), MB_OK | MB_ICONWARNING );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvResultNonId-------------------------------------\n")
												  _T("\tPACKET ID : SC_LOGIN_NONID\n")
												  _T("\t로그인 실패_ 존재하지 않은 id로 로그인 시도\n")
												  _T("[Network] -------------------------------------RecvResultNonId\n") );
	/////////////////////////////////////////////////////////////////////////////////////////


	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->SetIndexId( 0 );
}

void CNetwork::RecvResultNotPw()
{
	MessageBox( NULL, _T("password가 틀립니다."), _T("Info"), MB_OK | MB_ICONWARNING );

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvResultNotPw-------------------------------------\n")
												  _T("\tPACKET ID : SC_LOGIN_NOTPW\n")
												  _T("\t로그인 실패_ 비밀번호 틀림\n")
												  _T("[Network] -------------------------------------RecvResultNotPw\n") );
	/////////////////////////////////////////////////////////////////////////////////////////


	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->SetIndexId( 0 );
}

void CNetwork::RecvResultLogin( SPacket* packet )
{
	//여기선 doc의 정보를 바꿔 줘야 한다.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int id = 0;
	(*packet) >> id;

	pDoc->SetIndexId( id );
	pDoc->SetLogin( TRUE );

	//여기서 서버를 바꾸는 함수를 호출한다.
	int len = 0;
	char ipAddr[20];
	int Port = 0;
	(*packet) >> len;
	packet->GetData( ipAddr, len );
	ipAddr[len] = '\0';
	(*packet) >> Port;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, "[Network] RecvResultLogin-------------------------------------\n"
												  "\tPACKET ID : SC_LOGIN_SUCCESS\n"
												  "\t로그인 성공\n"
												  "\tSessionID : %d\n\n"
												  "\t======서버를 이동합니다======\n"
												  "\tip : %s\n"
												  "\tport : %d\n"
												  "[Network] -------------------------------------RecvResultLogin\n"
												  , id
												  , ipAddr
												  , Port );
	/////////////////////////////////////////////////////////////////////////////////////////


	//연결을 끊고
	DisConnect();
	//다시 연결
	ReConnectNet( ipAddr, Port );
}

void CNetwork::RecvConnectCompleteToG()
{
	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvConnectCompleteToG-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CONNECT_OK\n")
												  _T("\t게임 서버로부터 정상적으로 연결되었다는 패킷을 받음\n")
												  _T("[Network] -------------------------------------RecvConnectCompleteToG\n") );
	/////////////////////////////////////////////////////////////////////////////////////////

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	//ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->SetConnectToG( TRUE );
}

void CNetwork::RecvConnectFailed()
{
	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvConnectFailed-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CONNECT_FAILED\n")
												  _T("\t게임 서버로부터 정상적으로 연결되지 못했다는 패킷을 받음\n")
												  _T("[Network] -------------------------------------RecvConnectFailed\n") );
	/////////////////////////////////////////////////////////////////////////////////////////

	MessageBox( NULL, _T("게임서버와의 정상적인 연결이 되지 않습니다.\n다시 접속해 주세요."), _T("Info"), MB_OK | MB_ICONWARNING );

	return;
}

void CNetwork::RecvCreateCharacter( SPacket* packet )
{
	int charCount;
	(*packet) >> charCount;

	int indexId, state, x, y, len;
	TCHAR id[20];
	CCharacter* tmp;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvCreateCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CREATE_CHARACTER\n")
												  _T("\t캐릭터 생성 \n")
												  _T("\t생성캐릭터 수 : %d\n")
												  , charCount );	
	/////////////////////////////////////////////////////////////////////////////////////////

	for( int i=0; i<charCount; ++i )
	{
		(*packet) >> indexId;
		(*packet) >> len;
		packet->GetData( id, len );
		(*packet) >> state;
		(*packet) >> x;
		(*packet) >> y;
	
		//////////////////////////////////////////////////////////////////////////////////////////
		GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("\tSessionId    : %d\n")
													  _T("\tChar Id      : %s\n")
													  _T("\tChar state   : %d\n")
													  _T("\tChar pos(x/y): %d/ %d\n")
													  , indexId
													  , id
													  , state
													  , x
													  , y );
		//////////////////////////////////////////////////////////////////////////////////////////

		GetCharMgr.CreateCharacter( indexId, x, y, id );

		tmp = GetCharMgr.FindChar( indexId );

		tmp->SetState( state );
	}

	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] -------------------------------------RecvCreateCharacter\n") );
}

void CNetwork::RecvMoveCharacter( SPacket* packet )
{
	int indexId, state, x, y;

	(*packet) >> indexId;
	// 내 정보면 넘어 가자
	if( indexId == GetCharMgr.GetHeroId() )
		return;

	(*packet) >> state;
	(*packet) >> x;
	(*packet) >> y;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvCreateCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_CHANGE_STATE\n")
												  _T("\t캐릭터 상태 변화\n")
												  _T("\tSesseionId    : %d\n")
												  _T("\tchar staet    : %d\n")
												  _T("\tchar pos(x/y) : %d/ %d\n")
												  _T("[Network] -------------------------------------RecvMoveCharacter\n")
												  , indexId
												  , state
												  , x
												  , y );
	/////////////////////////////////////////////////////////////////////////////////////////

	GetCharMgr.StartCritical();

	CCharacter* tmp;
	tmp = GetCharMgr.FindChar( indexId );

	tmp->SetState( state );
	tmp->SetPos( x, y );

	GetCharMgr.EndCritical();

}

void CNetwork::RecvDeleteCharacter( SPacket* packet )
{
	int index;

	(*packet) >> index;

	//log////////////////////////////////////////////////////////////////////////////////////
	GetLogger.PutLog( SLogger::LOG_LEVEL_DBGINFO, _T("[Network] RecvCreateCharacter-------------------------------------\n")
												  _T("\tPACKET ID : SC_GAME_DELETE_CHARACTER\n")
												  _T("\t캐릭터 접속종료 등으로 인한 삭제\n")
												  _T("\tSesseionId : %d\n")
												  _T("[Network] -------------------------------------RecvDeleteCharacter\n")
												  , index );
	/////////////////////////////////////////////////////////////////////////////////////////

	GetCharMgr.DeleteCharacter( index );
}

