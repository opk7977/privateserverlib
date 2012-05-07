#include "StdAfx.h"
#include "Scheduler.h"

#include "SNetwork.h"
#include "SPacketQueue.h"
#include "NetProtocol.h"

//#include "ClientDoc.h"
#include "ClientView.h"

#include "CharMgr.h"
#include "RoomMgr.h"

#include "Room.h"

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

	//doc를 받아 둔다.
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

	//30보다 크면 그냥 30으로
	if( count > 30 )
		count = 30;

	for( int i=0; i<count; ++i )
	{
		GetPacketQ.GetPacket( m_packet );

		//패킷 처리
		PacketParsing();

		//처리가 끝나고 위치를 이동한다.
		//GetPacketQ.MoveReadPos();
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
		RecvLobbyOpenRoom();
		break;
	case SC_LOBBY_CLOSE_ROOM:
		RecvLobbyCloseRoom();
		break;
	case SC_ROOM_RESULT_INSERT:
		RecvRoomResultInsert();
		break;
	case SC_LOBBY_INSERT_ROOM:
		RecvLobbyInsertRoom();
		break;
	case SC_ROOM_OTHER_CHARINFO:
		RecvRoomOtherChar();
		break;
	case SC_ROOM_LEADER:
		RecvRoomLeader();
		break;
	case SC_ROOM_CHAR_OUT:
		RecvRoomCharOut();
		break;
	case SC_LOBBY_ROOMPLAYER_COUNTDOWN:
		RecvRoomPlayerCountDown();
		break;
	case SC_ROOM_CHAR_READY:
		RecvCharReady();
		break;
	case SC_ROOM_TEAM_CHANGE:
		RecvTeamChange();
		break;
	case SC_LOBBY_CHAT:
		RecvLobbyChat();
		break;
	case SC_LOBBY_PLAYER_DISCONNECT:
		RecvLobbyPlayerDisconnect();
		break;

	//==============================================================> GameSrv

	//==============================================================> Error
	default:
		MessageBox( NULL, _T("무슨 패킷이야 이건?"), _T("?????"), MB_OK );
	}
}

//패킷을 처리하는 함수

//======================================
// LoginSrv
//======================================
void CScheduler::RecvLoginConnectOK()
{
	if (!m_pDoc)
		return;

	MessageBox( NULL, _T("로그인 서버와 연결되었습니다."), _T("Info"), MB_OK );

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
		MessageBox( NULL, _T("이미 사용중인 id입니다."), _T("다시!"), MB_OK );
	}
	else
	{
		MessageBox( NULL, _T("사용할 수 있는 id입니다."), _T("Good"), MB_OK );
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
		MessageBox( NULL, _T("계정생성 실패."), _T("다시!"), MB_OK );
	}
	else
	{
		MessageBox( NULL, _T("계정이 생성되었습니다."), _T("Good"), MB_OK );
	}

	m_pDoc->isCreateResult = result;
}

void CScheduler::RecvLoginLoginResult()
{
	if (!m_pDoc)
		return;

	int result;
	m_packet >> result;

	if( result == -1 )
	{
		MessageBox( NULL, _T("존재하지 않는 id입니다."), _T("error"), MB_OK | MB_ICONERROR );
	}
	else if( result == 0 )
	{
		MessageBox( NULL, _T("password가 틀립니다."), _T("error"), MB_OK | MB_ICONERROR );
	}
	else if( result == -5 )
	{
		MessageBox( NULL, _T("이미 로그인된 ID입니다."), _T("error"), MB_OK | MB_ICONERROR );
	}
	else if( result == -10 )
	{
		MessageBox( NULL, _T("서버 오류입니다."), _T("error"), MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( NULL, _T("로그인에 성공~"), _T("good"), MB_OK );
	}
	
	m_pDoc->SessionID = result;

	if( result > 0 )
	{
		int size, port;
		char ip[20] = {0,};
		
		m_packet >> size;
		m_packet.GetData( ip, size );
		m_packet >> port;

		if( !GetNetwork.ReConnect( ip, port ) )
			MessageBox( NULL, _T("연결 실패!"), _T("error"), MB_OK );
	}

	//로비에 있는 상태로 바꿔준다.
	//m_pDoc->isSceneState = 1;
}

//======================================
// LobbySrv
//======================================
void CScheduler::RecvLobbyConnectOK()
{
	if (!m_pDoc)
		return;

	MessageBox( NULL, _T("로비 서버와 연결되었습니다."), _T("Info"), MB_OK );

	m_pDoc->isConnectToLogin = FALSE;
	m_pDoc->isConnectToLobby = TRUE;

	//로비에 있는 상태로 바꿔준다.
	m_pDoc->isSceneState = 1;

	//내 정보를 보낸다.
	SPacket sendPacket;
	sendPacket.SetID( CS_LOBBY_INSERT_LOBBY );

	sendPacket << m_pDoc->SessionID;
// 	int size = _tcslen( m_pDoc->strId ) * sizeof(TCHAR);
// 	sendPacket << size;
// 	sendPacket.PutData( m_pDoc->strId, size );
	sendPacket << m_pDoc->myRoomNum;

	GetNetwork.SendPacket( &sendPacket );
}

void CScheduler::RecvLobbyPlayerInfo()
{
	int count, sessionId, size;
	TCHAR id[50];

	m_packet >> count;

	//캐릭터 생성
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

	//result가 0보다 작으면 방만들기에 실패했음
	if( result < 0 )
	{
		m_pDoc->Revcvresult = result;
		m_pDoc->isRecvResult = TRUE;
		return;
	}
	else
		m_

	Character* tmpChar = GetCharMgr.GetMe();
	
	if( tmpChar == NULL )
	{
		MessageBox( NULL, _T("CScheduler::RecvRoomResultCreate()\n나 없는데?"), _T("error"), MB_OK );
		return;
	}
	m_packet >> team;
	tmpChar->SetTeam( team );

	m_pDoc->Revcvresult = result;
	m_pDoc->isRecvResult = TRUE;
}

void CScheduler::RecvLobbyOpenRoom()
{
	int roomNum, size;
	TCHAR title[50];

	m_packet >> roomNum;
	m_packet >> size;
	m_packet.GetData( title, size );

	RoomObj* tmpRoom = GetRoomMgr.FindRoom( roomNum );
	if( tmpRoom == NULL )
	{
		MessageBox( NULL, _T("CScheduler::RecvLobbyOpenRoom()\n방 없는데?"), _T("error"), MB_OK );
		return;
	}

	tmpRoom->SetRoomTitle( title );
}

void CScheduler::RecvLobbyCloseRoom()
{
	int room;
	m_packet >> room;

	RoomObj* tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
	{
		MessageBox( NULL, _T("CScheduler::RecvLobbyCloseRoom()\n방 없는데?"), _T("error"), MB_OK );
		return;
	}

	tmpRoom->Init();
}

void CScheduler::RecvRoomResultInsert()
{
	if( !m_pDoc )
		return;

	if( !m_pRoom )
		return;

	int result;
	m_packet >> result;

	m_pDoc->Revcvresult = result;
	m_pDoc->isRecvResult = TRUE;


	while( m_pDoc->myRoomNum <= 0 )
	{
		//방 번호가 설정될때까지 잠시 대기
	}

	//Character* me = GetCharMgr.GetMe();

	m_pRoom->OpenRoom( m_pDoc->myRoomNum, GetRoomMgr.FindRoom( m_pDoc->myRoomNum )->GetRoomTitle() );
	
// 	m_pRoom->SetMe( me->GetSessionID() );
// 	m_pRoom->SetLeader( me->GetSessionID() );

	//m_pRoom->AddPlayer( me->GetSessionID(), me->GetID(), me->GetTeam(), 0 );

	//나는 방으로 들어가게 되니까 로비의 방정보를 모두 지운다
	GetRoomMgr.ClearAllRoom();

	//캐릭터 정보도 모두 지운다.
	GetCharMgr.ClearCharacter();
}

void CScheduler::RecvLobbyInsertRoom()
{
	int roomNum, sessionID;

	m_packet >> sessionID;
	m_packet >> roomNum;

	RoomObj* tmpRoom = GetRoomMgr.FindRoom( roomNum );
	if( tmpRoom == NULL )
		MessageBox( NULL, _T("CScheduler::RecvLobbyOpenRoom()\n방 없는데?"), _T("error"), MB_OK );

	tmpRoom->AddPlayer();

	//캐릭터 정보를 지운다
	GetCharMgr.DelChar( sessionID );
}

//

void CScheduler::RecvRoomOtherChar()
{
	if(!m_pDoc)
		return;

	if(!m_pRoom)
		return;

	int count, sessionId, size, team, ready;
	TCHAR tmpID[30]={0,};

	m_packet >> count;

	for( int i=0; i<count; ++i )
	{
		m_packet >> sessionId;
		m_packet >> size;
		m_packet.GetData( tmpID, size );
		m_packet >> team;
		m_packet >> ready;

		m_pRoom->AddPlayer( sessionId, tmpID, team, ready );
	}

	if( m_pRoom->m_itMe == NULL )
	{
		m_pRoom->SetMe( m_pDoc->SessionID );
	}
}

void CScheduler::RecvRoomLeader()
{
	if( !m_pRoom )
		return;

	int leader;
	
	m_packet >> leader;

	m_pRoom->SetLeader( leader );
}

void CScheduler::RecvRoomCharOut()
{
	if( !m_pRoom )
		return;

	int sessionid;
	
	m_packet >> sessionid;

	m_pRoom->DelPlayer( sessionid );
}

void CScheduler::RecvRoomPlayerCountDown()
{
	int room;

	m_packet >> room;

	RoomObj* tmpRoom = GetRoomMgr.FindRoom( room );
	if( tmpRoom == NULL )
	{
		MessageBox( NULL, _T("CScheduler::RecvRoomPlayerCountDown()\n방 정보를 받아오지 못함"), _T("error"), MB_OK );
		return;
	}
	tmpRoom->DelPlayer();
}

void CScheduler::RecvCharReady()
{
	if( !m_pRoom )
		return;

	int sessionId, ready;
	
	m_packet >> sessionId;
	m_packet >> ready;

	RoomPlayer* tmpPlayer =	m_pRoom->FindChar( sessionId );
	if( tmpPlayer == NULL )
		return;

	tmpPlayer->SetReady( ready );
	(m_pRoom->m_player_Ready[tmpPlayer->GetRoomSortNo()]).SetWindowText( m_pRoom->ReadyString[ready].GetString() );
}

void CScheduler::RecvTeamChange()
{
	if( !m_pRoom )
		return;

	int sessionId, team;

	m_packet >> sessionId;
	m_packet >> team;

	RoomPlayer* tmpPlayer =	m_pRoom->FindChar( sessionId );
	if( tmpPlayer == NULL )
		return;

	tmpPlayer->SetTeam( team );
	(m_pRoom->m_player_Team[tmpPlayer->GetRoomSortNo()]).SetWindowText( m_pRoom->TeamString[team].GetString() );
}

void CScheduler::RecvLobbyChat()
{
	if( !m_pRoom )
		return;

	int roomNum, size;
	TCHAR chatText[100]={0,};

	m_packet >> roomNum;
	m_packet >> size;
	m_packet.GetData( chatText, size );

	if( roomNum == 0 )
	{
		//로비에있는 채팅 창에 적음
	}
	else
	{
		//방에 있는 채팅창
		m_pRoom->m_ctrl_ChatView.SetWindowText( chatText );
	}
	
}
