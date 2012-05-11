// Lobby.cpp : ���� �����Դϴ�.
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


// CLobby �����Դϴ�.

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


// CLobby �޽��� ó�����Դϴ�.

void CLobby::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//GetScheduler.Init();
	//���� ����� �д�
	GetRoomMgr.CreateRoom();

	//���⼭ charMgr�� roomMgr���� �� �ָ� ����..
	GetCharMgr.m_pLobby = this;
	GetRoomMgr.m_pLobby = this;
}

void CLobby::OnNetConn()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if( pDoc->isConnectToLogin || pDoc->isConnectToLobby )
	{
		MessageBox( _T("�̹� ������ ����Ǿ� �ֽ��ϴ�."), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

// 	if( !GetNetwork.Init() )
// 	{
// 		MessageBox( _T("���� �ʱ�ȭ ����"), _T("error"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
// 
// 	if( !GetNetwork.ConnectToSrv( "192.168.0.56", 8800 ) )
// 	{
// 		MessageBox( _T("���� ����.."), _T("?!?!?"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
	if( !GetNetwork.Init() )
	{
		MessageBox( _T("���� �ʱ�ȭ ����"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	if( !GetNetwork.ConnectToSrv( "192.168.0.56", 8800 ) )
	//if( !GetNetwork.ConnectToSrv( "127.0.0.1", 8800 ) )
	{
		MessageBox( _T("���� ����.."), _T("?!?!?"), MB_OK | MB_ICONERROR );
		return;
	}
}

void CLobby::OnNetLogin()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CClientDoc* pDoc = (CClientDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//���� �α��� ������ ������ �Ǿ� ���� ������ �������� �ʴ´�.
	if( !pDoc->isConnectToLogin )
	{
		MessageBox( _T("�α��� ������ ���ӵǾ� ���� �ʽ��ϴ�."), _T("error"), MB_OK | MB_ICONERROR );
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 1 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom2New()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 2 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom3New()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 3 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom4New()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	//room->SetRoomNum( 4 );

	CRoom* pRoom = (CRoom*)((CMainFrame*)AfxGetMainWnd())->wndSplitterRow.GetPane( 1, 0 );
	room->m_pRoom = pRoom;

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom1Insert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		MessageBox( _T("CLobby::OnBnClickedRoom1Insert()\n����ũ�Ⱑ �ٸ���?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("������ �ǻ�!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("������ ����\n�� ������ �޾� �ɴϴ�."), _T("info"), MB_OK );

	pDoc->myRoomNum = 1;
}

void CLobby::OnBnClickedRoom2Insert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		MessageBox( _T("CLobby::OnBnClickedRoom2Insert()\n����ũ�Ⱑ �ٸ���?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("������ �ǻ�!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("������ ����\n�� ������ �޾� �ɴϴ�."), _T("info"), MB_OK );

	pDoc->myRoomNum = 2;
}

void CLobby::OnBnClickedRoom3Insert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		MessageBox( _T("CLobby::OnBnClickedRoom3Insert()\n����ũ�Ⱑ �ٸ���?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("������ �ǻ�!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("������ ����\n�� ������ �޾� �ɴϴ�."), _T("info"), MB_OK );

	pDoc->myRoomNum = 3;
}

void CLobby::OnBnClickedRoom4Insert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
		MessageBox( _T("CLobby::OnBnClickedRoom4Insert()\n����ũ�Ⱑ �ٸ���?"), _T("error"), MB_OK );
	}

	pDoc->isRecvResult = FALSE;
	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		MessageBox( _T("������ �ǻ�!"), _T("info"), MB_OK );
		return;
	}
	MessageBox( _T("������ ����\n�� ������ �޾� �ɴϴ�."), _T("info"), MB_OK );

	pDoc->myRoomNum = 4;
}
