// Lobby.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"

#include "MainFrm.h"
#include "ClientDoc.h"
#include "ClientView.h"

#include "Lobby.h"
#include "Room.h"

//#include "Network.h"
#include "SNetwork.h"
#include "NetProtocol.h"
#include "Scheduler.h"
#include "CharMgr.h"

#include "Login.h"
#include "RoomMgr.h"

#include "NewRoom.h"



// CLobby

IMPLEMENT_DYNCREATE(CLobby, CFormView)

CLobby::CLobby()
	: CFormView(CLobby::IDD)
{
}

CLobby::~CLobby()
{
}

void CLobby::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ROOM1_TITLE, m_RoomTitle[0]);
	DDX_Control(pDX, IDC_ROOM2_TITLE, m_RoomTitle[1]);
	DDX_Control(pDX, IDC_ROOM3_TITLE, m_RoomTitle[2]);
	DDX_Control(pDX, IDC_ROOM4_TITLE, m_RoomTitle[3]);

	DDX_Control(pDX, IDC_ROOM1_PLAYERCOUNT, m_RoomPlayerCount[0]);
	DDX_Control(pDX, IDC_ROOM2_PLAYERCOUNT, m_RoomPlayerCount[1]);
	DDX_Control(pDX, IDC_ROOM3_PLAYERCOUNT, m_RoomPlayerCount[2]);
	DDX_Control(pDX, IDC_ROOM4_PLAYERCOUNT, m_RoomPlayerCount[3]);

	DDX_Control(pDX, IDC_ROOM1_NEW, m_btnNewRoom[0]);
	DDX_Control(pDX, IDC_ROOM2_NEW, m_btnNewRoom[1]);
	DDX_Control(pDX, IDC_ROOM3_NEW, m_btnNewRoom[2]);
	DDX_Control(pDX, IDC_ROOM4_NEW, m_btnNewRoom[3]);

	DDX_Control(pDX, IDC_ROOM1_INSERT, m_btnEnterRoom[0]);
	DDX_Control(pDX, IDC_ROOM2_INSERT, m_btnEnterRoom[1]);
	DDX_Control(pDX, IDC_ROOM3_INSERT, m_btnEnterRoom[2]);
	DDX_Control(pDX, IDC_ROOM4_INSERT, m_btnEnterRoom[3]);
}

BEGIN_MESSAGE_MAP(CLobby, CFormView)
	ON_COMMAND(ID_NET_CONN, &CLobby::OnNetConn)
	ON_COMMAND(ID_NET_LOGIN, &CLobby::OnNetLogin)
	ON_BN_CLICKED(IDC_ROOM1_NEW, &CLobby::OnBnClickedRoom1New)
	ON_BN_CLICKED(IDC_ROOM2_NEW, &CLobby::OnBnClickedRoom2New)
	ON_BN_CLICKED(IDC_ROOM3_NEW, &CLobby::OnBnClickedRoom3New)
	ON_BN_CLICKED(IDC_ROOM4_NEW, &CLobby::OnBnClickedRoom4New)
	ON_BN_CLICKED(IDC_ROOM1_INSERT, &CLobby::OnBnClickedRoom1Insert)
	ON_BN_CLICKED(IDC_ROOM2_INSERT, &CLobby::OnBnClickedRoom2Insert)
	ON_BN_CLICKED(IDC_ROOM3_INSERT, &CLobby::OnBnClickedRoom3Insert)
	ON_BN_CLICKED(IDC_ROOM4_INSERT, &CLobby::OnBnClickedRoom4Insert)
END_MESSAGE_MAP()


// CLobby 진단입니다.

#ifdef _DEBUG
void CLobby::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLobby::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLobby 메시지 처리기입니다.

void CLobby::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//GetScheduler.Init();
	//방을 만들어 둔다
	GetRoomMgr.CreateRoom();

	//여기서 charMgr와 roomMgr에게 날 주면 되지..
	GetCharMgr.m_pLobby = this;
	GetRoomMgr.m_pLobby = this;
}

void CLobby::OnNetConn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if( pDoc->isConnectToLogin || pDoc->isConnectToLobby )
	{
		MessageBox( _T("이미 서버와 연결되어 있습니다."), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

// 	if( !GetNetwork.Init() )
// 	{
// 		MessageBox( _T("소켓 초기화 실패"), _T("error"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
// 
// 	if( !GetNetwork.ConnectToSrv( "192.168.0.56", 8800 ) )
// 	{
// 		MessageBox( _T("연결 실패.."), _T("?!?!?"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
	if( !GetNetwork.Init() )
	{
		MessageBox( _T("소켓 초기화 실패"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	if( !GetNetwork.ConnectToSrv( "192.168.0.56", 8800 ) )
	//if( !GetNetwork.ConnectToSrv( "127.0.0.1", 8800 ) )
	{
		MessageBox( _T("연결 실패.."), _T("?!?!?"), MB_OK | MB_ICONERROR );
		return;
	}
}

void CLobby::OnNetLogin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//아직 로그인 서버에 연결이 되어 있지 않으면 실행하지 않는다.
	if( !pDoc->isConnectToLogin )
	{
		MessageBox( _T("로그인 서버에 접속되어 있지 않습니다."), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	CLogin*	loginDlg = new CLogin;
	int dlgResult = loginDlg->DoModal();
	delete loginDlg;
}

void CLobby::AddPlayerInList( TCHAR* player )
{
	CListBox *pList;
	pList = (CListBox *)GetDlgItem( IDC_LOBBY_PLAYERLIST );
	pList->AddString( player );
}

void CLobby::DelPlayerInList( TCHAR* player )
{
	CListBox *pList;
	pList = (CListBox *)GetDlgItem( IDC_LOBBY_PLAYERLIST );
	int index = pList->FindString( 0, player );

	if( index == LB_ERR )
		return;

	pList->DeleteString( index );
}

void CLobby::OnBnClickedRoom1New()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 1 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom2New()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 2 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom3New()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 3 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom4New()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 4 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom1Insert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SPacket sendPacket;

	sendPacket.SetID( CS_LOBBY_INSERT_ROOM );
	sendPacket << 1;

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("CLobby::OnBnClickedRoom1Insert()\n전송크기가 다른데?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("방입장 실퍠!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("방으로 입장\n방 정보를 받아 옵니다."), _T("info"), MB_OK );

	pDoc->myRoomNum = 1;
}

void CLobby::OnBnClickedRoom2Insert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SPacket sendPacket;

	sendPacket.SetID( CS_LOBBY_INSERT_ROOM );
	sendPacket << 2;

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("CLobby::OnBnClickedRoom2Insert()\n전송크기가 다른데?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("방입장 실퍠!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("방으로 입장\n방 정보를 받아 옵니다."), _T("info"), MB_OK );

	pDoc->myRoomNum = 2;
}

void CLobby::OnBnClickedRoom3Insert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SPacket sendPacket;

	sendPacket.SetID( CS_LOBBY_INSERT_ROOM );
	sendPacket << 3;

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("CLobby::OnBnClickedRoom3Insert()\n전송크기가 다른데?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("방입장 실퍠!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("방으로 입장\n방 정보를 받아 옵니다."), _T("info"), MB_OK );

	pDoc->myRoomNum = 3;
}

void CLobby::OnBnClickedRoom4Insert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SPacket sendPacket;

	sendPacket.SetID( CS_LOBBY_INSERT_ROOM );
	sendPacket << 4;

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("CLobby::OnBnClickedRoom4Insert()\n전송크기가 다른데?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("방입장 실퍠!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("방으로 입장\n방 정보를 받아 옵니다."), _T("info"), MB_OK );

	pDoc->myRoomNum = 4;
}
