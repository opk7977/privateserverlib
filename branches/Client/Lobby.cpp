// Lobby.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Lobby.h"

#include "Network.h"
#include "Login.h"

#include "Scheduler.h"

#include "ClientDoc.h"
#include "ClientView.h"


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
	GetScheduler.Init();
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

	if( !GetNetwork.Init() )
	{
		MessageBox( _T("소켓 초기화 실패"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	if( !GetNetwork.ConnectToSrv( "192.168.0.70", 8880 ) )
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
	loginDlg->DoModal();
	delete loginDlg;
}

