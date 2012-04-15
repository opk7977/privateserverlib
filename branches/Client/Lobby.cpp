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
}

BEGIN_MESSAGE_MAP(CLobby, CFormView)
	ON_COMMAND(ID_NET_CONN, &CLobby::OnNetConn)
	ON_COMMAND(ID_NET_LOGIN, &CLobby::OnNetLogin)
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

	//���⼭ charMgr���� �� �ָ� ����..
	GetCharMgr.m_pLobby = this;
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
