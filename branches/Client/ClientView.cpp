
// ClientView.cpp : CClientView 클래스의 구현
//

#include "stdafx.h"
#include "Client.h"

#include "ClientDoc.h"
#include "ClientView.h"

#include "Network.h"
#include "Login.h"

#include "Scheduler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientView

IMPLEMENT_DYNCREATE(CClientView, CView)

BEGIN_MESSAGE_MAP(CClientView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_NET_CONN, &CClientView::OnNetConn)
	ON_COMMAND(ID_NET_LOGIN, &CClientView::OnNetLogin)
END_MESSAGE_MAP()

// CClientView 생성/소멸

CClientView::CClientView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CClientView::~CClientView()
{
}

BOOL CClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CClientView 그리기

void CClientView::OnDraw(CDC* /*pDC*/)
{
	CClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CClientView 인쇄

BOOL CClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CClientView 진단

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CView::AssertValid();
}

void CClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClientDoc* CClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientDoc)));
	return (CClientDoc*)m_pDocument;
}
#endif //_DEBUG


//======================================
// 메뉴 함수
//======================================
// CClientView 메시지 처리기

void CClientView::OnNetConn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// 	CClientDoc* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
// 	if (!pDoc)
// 		return;
// 
// 	if( pDoc->isConnectToLogin || pDoc->isConnectToLobby )
// 	{
// 		MessageBox( _T("이미 서버와 연결되어 있습니다."), _T("error"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
// 
// 	if( !GetNetwork.Init() )
// 	{
// 		MessageBox( _T("소켓 초기화 실패"), _T("error"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
// 
// 	if( !GetNetwork.ConnectToSrv( "192.168.0.70", 8880 ) )
// 	{
// 		MessageBox( _T("연결 실패.."), _T("?!?!?"), MB_OK | MB_ICONERROR );
// 		return;
// 	}

	//GetNetwork.Begin

// 	while( !pDoc->isConnectToLogin )
// 	{
// 	}
// 
// 	MessageBox( _T("로그인 서버와 연결되었습니다."), _T("Info"), MB_OK );
}

void CClientView::OnNetLogin()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// 	CClientDoc* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
// 	if (!pDoc)
// 		return;
// 
// 	//아직 로그인 서버에 연결이 되어 있지 않으면 실행하지 않는다.
// 	if( !pDoc->isConnectToLogin )
// 	{
// 		MessageBox( _T("로그인 서버에 접속되어 있지 않습니다."), _T("error"), MB_OK | MB_ICONERROR );
// 		return;
// 	}
// 
// 	CLogin*	loginDlg = new CLogin;
// 	loginDlg->DoModal();
// 	delete loginDlg;

// 	CLogin logInDlg;
// 	logInDlg.Create( IDD_LOGIN );
// 	logInDlg.ShowWindow( SW_SHOW );
}

//======================================


void CClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//GetScheduler.Init();
}
