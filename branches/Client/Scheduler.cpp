#include "StdAfx.h"
#include "Scheduler.h"

#include "PacketQueue.h"
#include "NetProtocol.h"
#include "Network.h"

//#include "ClientDoc.h"
#include "ClientView.h"

#include "CharMgr.h"
#include "RoomMgr.h"

unsigned int __stdcall _Schdul( void* pArg )
{
	return ((CScheduler*)pArg)->Run();
}

CScheduler::CScheduler(void)
{
}

CScheduler::~CScheduler(void)
{
}

void CScheduler::Init()
{

	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL); 

	unsigned int iThread;
	m_schThread = (HANDLE)_beginthreadex( NULL, 0, &::_Schdul, (void*)this, 0, &iThread );
	if( !m_schThread )
		return;

	m_timer.Init();
	m_frame = 0.f;

	//doc�� �޾� �д�.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	m_pDoc = pView->GetDocument();

	SetEvent( m_startWork );
}

unsigned int CScheduler::Run()
{
	while(1)
	{
		Sleep(10);

		WaitForSingleObject( m_startWork, INFINITE );

		m_timer.ProcessTime();

		m_frame += m_timer.GetElapsedTime();

		if( m_frame >= 1.f )
		{
			m_frame -= 1.f;

			PacketProcess();
		}
	}
}

void CScheduler::PacketProcess()
{
	int count = GetPacketQ.GetDataCount();

	if( count <= 0 )
		return;

	//30���� ũ�� �׳� 30����
	if( count > 30 )
		count = 30;

	char* tmpBuf;

	for( int i=0; i<count; ++i )
	{
		tmpBuf = GetPacketQ.GetPacket();
		if( tmpBuf == NULL )
			return;

		m_packet.CopyToPacket( tmpBuf, PACKETDATA_SIZE );

		//��Ŷ ó��
		PacketParsing();

		//ó���� ������ ��ġ�� �̵��Ѵ�.
		GetPacketQ.MoveReadPos();
	}
}

void CScheduler::PacketParsing()
{
	switch( m_packet.GetID() )
	{
	//==============================================================> LoginSrv
	case SC_LOGIN_CONNECT_OK:
		RecvLoginConnectOK();
		break;
	case SC_LOGIN_CHECK_ID_RESULT:
		RecvLoginCheckID();
		break;
	case SC_LOGIN_CREATE_RESULT:
		RecvLoginCreateResult();
		break;
	case SC_LOGIN_LOGIN_RESULT:
		RecvLoginLoginResult();
		break;
	//==============================================================> LobbySrv
	case SC_LOBBY_CONNECT_OK:
		RecvLobbyConnectOK();
		break;
	case SC_LOBBY_OTHER_CHARINFO:
		RecvLobbyPlayerInfo();
		break;
	case SC_LOBBY_ROOMINFO:
		RecvLobbyRoomInfo();
		break;
	case SC_ROOM_RESULT_CREATE:
		RecvRoomResultCreate();
		break;
	case SC_LOBBY_OPEN_ROOM:
		//RecvLobbyOpenRoom();
		break;
	//case SC_LOBBY_CLOSE_ROOM:
		//RecvLobbyCloseRoom();
		//break;
	//case SC_ROOM_RESULT_INSERT:
		//RecvRoomResultInsert();
		//break;
	case SC_LOBBY_INSERT_ROOM:
		//RecvLobbyInsertRoom();
		break;


	case SC_LOBBY_PLAYER_DISCONNECT:
		RecvLobbyPlayerDisconnect();
		break;
	//==============================================================> GameSrv
	default:
		MessageBox( NULL, _T("���� ��Ŷ�̾� �̰�?"), _T("?????"), MB_OK );
	}
}

//��Ŷ�� ó���ϴ� �Լ�

//======================================
// LoginSrv
//======================================
void CScheduler::RecvLoginConnectOK()
{
	if (!m_pDoc)
		return;

	MessageBox( NULL, _T("�α��� ������ ����Ǿ����ϴ�."), _T("Info"), MB_OK );

	m_pDoc->isConnectToLogin = TRUE;
}

void CScheduler::RecvLoginCheckID()
{
	if (!m_pDoc)
		return;

	int result;
	m_packet >> result;

	if( result < 0 )
	{
		MessageBox( NULL, _T("�̹� ������� id�Դϴ�."), _T("�ٽ�!"), MB_OK );
	}
	else
	{
		MessageBox( NULL, _T("����� �� �ִ� id�Դϴ�."), _T("Good"), MB_OK );
	}

	m_pDoc->CheckID = result;
}

void CScheduler::RecvLoginCreateResult()
{
	if (!m_pDoc)
		return;

	int result;
	m_packet >> result;

	if( result < 0 )
	{
		MessageBox( NULL, _T("�������� ����."), _T("�ٽ�!"), MB_OK );
	}
	else
	{
		MessageBox( NULL, _T("������ �����Ǿ����ϴ�."), _T("Good"), MB_OK );
	}

	m_pDoc->isCreateResult = result;
}

void CScheduler::RecvLoginLoginResult()
{
	if (!m_pDoc)
		return;

	int result;
	m_packet >> result;

	if( result < 0 )
	{
		MessageBox( NULL, _T("�������� �ʴ� id�Դϴ�."), _T("error"), MB_OK | MB_ICONERROR );
	}
	if( result == 0 )
	{
		MessageBox( NULL, _T("password�� Ʋ���ϴ�."), _T("error"), MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( NULL, _T("�α��ο� ����~"), _T("good"), MB_OK );
	}
	
	m_pDoc->SessionID = result;

	if( result > 0 )
	{
		int size, port;
		char ip[20] = {0,};
		
		m_packet >> size;
		m_packet.GetData( ip, size );
		m_packet >> port;

		if( !GetNetwork.Reconnect( ip, port ) )
			MessageBox( NULL, _T("���� ����!"), _T("error"), MB_OK );
	}

	//�κ� �ִ� ���·� �ٲ��ش�.
	//m_pDoc->isSceneState = 1;
}

//======================================
// LobbySrv
//======================================
void CScheduler::RecvLobbyConnectOK()
{
	if (!m_pDoc)
		return;

	MessageBox( NULL, _T("�κ� ������ ����Ǿ����ϴ�."), _T("Info"), MB_OK );

	m_pDoc->isConnectToLogin = FALSE;
	m_pDoc->isConnectToLobby = TRUE;

	//�κ� �ִ� ���·� �ٲ��ش�.
	m_pDoc->isSceneState = 1;

	//�� ������ ������.
	SPacket sendPacket;
	sendPacket.SetID( CS_LOBBY_INSERT_LOBBY );

	sendPacket << m_pDoc->SessionID;
	int size = _tcslen( m_pDoc->strId ) * sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( m_pDoc->strId, size );
	sendPacket << m_pDoc->myRoomNum;

	GetNetwork.SendPacket( &sendPacket );
}

void CScheduler::RecvLobbyPlayerInfo()
{
	int count, sessionId, size;
	TCHAR id[50];

	m_packet >> count;

	//ĳ���� ����
	for( int i=0; i<count; ++i )
	{
		m_packet >> sessionId;
		m_packet >> size;
		m_packet.GetData( id, size );

		GetCharMgr.AddChar( sessionId, id );
	}

}

void CScheduler::RecvLobbyRoomInfo()
{
	int count, roomid, playerCount, size, state;
	TCHAR title[50] = {0,};

	m_packet >> count;

	RoomObj* tmpRoom;

	for( int i=0; i<count; ++i )
	{
		m_packet >> roomid;
		m_packet >> playerCount;
		m_packet >> size;
		m_packet.GetData( title, size );
		m_packet >> state;

		tmpRoom = GetRoomMgr.FindRoom( roomid );
		if( tmpRoom == NULL )
			continue;
		tmpRoom->SetPlayerCount( playerCount );
		tmpRoom->SetRoomTitle( title );
		tmpRoom->SetRoomState( state );
	}
}

void CScheduler::RecvLobbyPlayerDisconnect()
{
	int sessionId;

	m_packet >> sessionId;

	GetCharMgr.DelChar( sessionId );
}

void CScheduler::RecvRoomResultCreate()
{
	if (!m_pDoc)
		return;

	int result, team;
	m_packet >> result;

	//result�� 0���� ������ �游��⿡ ��������
	if( result < 0 )
	{
		m_pDoc->Revcvresult = result;
		m_pDoc->isRecvResult = TRUE;
		return;
	}

	Character* tmpChar = GetCharMgr.GetMe( );
	
	if( tmpChar == NULL )
	{
		MessageBox( NULL, _T("�� ���µ�?"), _T("error"), MB_OK );
		return;
	}
	m_packet >> team;
	tmpChar->SetTeam( team );

	m_pDoc->Revcvresult = result;
	m_pDoc->isRecvResult = TRUE;
}

void CScheduler::RecvLobbyOpenRoom()
{

}

void CScheduler::RecvLobbyCloseRoom()
{

}

void CScheduler::RecvRoomResultInsert()
{

}

void CScheduler::RecvLobbyInsertRoom()
{

}

