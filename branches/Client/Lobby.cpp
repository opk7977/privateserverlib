// Lobby.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "Lobby.h"

#include "Network.h"
#include "Login.h"

#include "Scheduler.h"

#include "ClientDoc.h"
#include "ClientView.h"

#include "CharMgr.h"
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
	GetScheduler.Init();
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

	if( !GetNetwork.Init() )
	{
		MessageBox( _T("���� �ʱ�ȭ ����"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	if( !GetNetwork.ConnectToSrv( "192.168.0.70", 8880 ) )
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
	room->SetRoomNum( 1 );

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom2New()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	room->SetRoomNum( 2 );

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom3New()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	room->SetRoomNum( 3 );

	room->DoModal();

	delete room;
}

void CLobby::OnBnClickedRoom4New()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CNewRoom* room = new CNewRoom;
	room->SetRoomNum( 4 );

	room->DoModal();

	delete room;
}
