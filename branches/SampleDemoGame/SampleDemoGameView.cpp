
// SampleDemoGameView.cpp : CSampleDemoGameView 클래스의 구현
//

#include "stdafx.h"
#include "SampleDemoGame.h"

#include "SampleDemoGameDoc.h"
#include "SampleDemoGameView.h"

#include "ConnectDlg.h"

#include "CharacterMgr.h"
#include "Network.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleDemoGameView

IMPLEMENT_DYNCREATE(CSampleDemoGameView, CView)

BEGIN_MESSAGE_MAP(CSampleDemoGameView, CView)
	ON_COMMAND(ID_CONN_CONNECT, &CSampleDemoGameView::OnConnConnect)
	ON_COMMAND(ID_CONN_DISCONN, &CSampleDemoGameView::OnConnDisconn)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// CSampleDemoGameView 생성/소멸

CSampleDemoGameView::CSampleDemoGameView() : pOldBit(0)
										   , frame(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CSampleDemoGameView::~CSampleDemoGameView()
{
	m_dump.End();
}

BOOL CSampleDemoGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSampleDemoGameView 그리기

void CSampleDemoGameView::OnDraw(CDC* pDC)
{
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if( pOldBit )
	{
		CBrush myBrush, *pOldBrush;
		myBrush.CreateSolidBrush(RGB(0, 255, 0));
		pOldBrush = memoryDC.SelectObject(&myBrush);
		//뒤에 배경(?)한장 그리고
		memoryDC.Rectangle( 0, 0, 800, 600 );

		memoryDC.SelectObject(pOldBrush);
		myBrush.DeleteObject();

		//캐릭터 그리고
		GetCharMgr.RenderAll( &memoryDC );

		//끗_ 화면으로 옮겨 준다.
		pDC->BitBlt( 0, 0, 800, 600, &memoryDC, 0, 0, SRCCOPY );
	}
}


// CSampleDemoGameView 진단

#ifdef _DEBUG
void CSampleDemoGameView::AssertValid() const
{
	CView::AssertValid();
}

void CSampleDemoGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSampleDemoGameDoc* CSampleDemoGameView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleDemoGameDoc)));
	return (CSampleDemoGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CSampleDemoGameView 메시지 처리기

//======================================
// 재정의
//======================================
void CSampleDemoGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//덤프 시작
	m_dump.Begin();

	//우선 화면은 셋팅해 주고
	BeginScene();
}


//======================================
// 메뉴 함수
//======================================
void CSampleDemoGameView::OnConnConnect()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//이미 연결되어 있는 상태 인지를 확인한다
	if( pDoc->GetConnect() || pDoc->GetConnectToG() )
	{
		MessageBox( _T("이미 서버와 연결되어 있습니다"), _T("Error"), MB_OK | MB_ICONINFORMATION );
		return;
	}

	CConnectDlg* pConn = new CConnectDlg;

	int result = pConn->DoModal();

	if( result == IDOK )
	{
		//제목표시줄을 바꾼다
		//this->SetWindowText( _T("연결 되었습니다.") );
	}

	delete( pConn );

	Invalidate( false );
}

void CSampleDemoGameView::OnConnDisconn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//연결 플레그를 모두 꺼주고
	pDoc->SetConnect( FALSE );
	pDoc->SetLogin( FALSE );
	pDoc->SetIndexId(-1);
	pDoc->SetConnectToG( FALSE );

	//연결을 모두 꺼주고
	GetNetwork.DisConnect();

	//캐릭터를 모두 지워주고
	GetCharMgr.Release();
}

//======================================
// 사용자 정의 함수
//======================================
void CSampleDemoGameView::BeginScene()
{
	//double buffer
	CClientDC dc(this);
	memoryDC.CreateCompatibleDC(&dc);
	memoryBit.CreateCompatibleBitmap(&dc, 800, 600);
	pOldBit = memoryDC.SelectObject(&memoryBit);

	//시간
	SetTimer( 1, 100, NULL );
}

void CSampleDemoGameView::EndScene()
{
	//double buffer
	if( pOldBit )
		memoryDC.SelectObject(pOldBit);

	memoryBit.DeleteObject();
	memoryDC.DeleteDC();
}

//======================================
// 이벤트 함수
//======================================
void CSampleDemoGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	++frame;

	if( frame >= 3 )
	{
		//캐릭터들 움직임
		GetCharMgr.MoveCharacter();
		Invalidate( FALSE );
	}

	CView::OnTimer(nIDEvent);
}

void CSampleDemoGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//이미 연결되어 있는 상태 인지를 확인한다. 아직 연결 전이라면 입력을 받지 않는다.
	if( !pDoc->GetConnectToG() )
		return;

	//////////////////////////////////////////////////////////////////////////
	CCharacter* myChar;
	myChar = GetCharMgr.GetHero();

	if( myChar == NULL )
		return;

	//키가 눌렸는지를 가리는 변수
	BOOL isKeyDown = FALSE;

	switch( nChar )
	{
	//case VK_UP:
	case 'w':
	case 'W':
		myChar->SetTransition( CCharacter::EVENT_U_DOWN );
		isKeyDown = TRUE;
		break;

	//case VK_DOWN:
	case 's':
	case 'S':
		myChar->SetTransition( CCharacter::EVENT_B_DOWN );
		isKeyDown = TRUE;
		break;

	//case VK_LEFT:
	case 'a':
	case 'A':
		myChar->SetTransition( CCharacter::EVENT_L_DOWN );
		isKeyDown = TRUE;
		break;
			
	//case VK_RIGHT:
	case 'd':
	case 'D':
		myChar->SetTransition( CCharacter::EVENT_R_DOWN );
		isKeyDown = TRUE;
		break;
	}

	if( isKeyDown )
		Invalidate( FALSE );	

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CSampleDemoGameView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//이미 연결되어 있는 상태 인지를 확인한다. 아직 연결 전이라면 입력을 받지 않는다.
	if( !pDoc->GetConnectToG() )
		return;

	//////////////////////////////////////////////////////////////////////////
	CCharacter* myChar;
	myChar = GetCharMgr.GetHero();

	if( myChar == NULL )
		return;

	myChar->SetTransition( CCharacter::EVENT_KYE_UP );

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

