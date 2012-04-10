
// SampleDemoGameView.cpp : CSampleDemoGameView Ŭ������ ����
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

// CSampleDemoGameView ����/�Ҹ�

CSampleDemoGameView::CSampleDemoGameView() : pOldBit(0)
										   , frame(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CSampleDemoGameView::~CSampleDemoGameView()
{
	m_dump.End();
}

BOOL CSampleDemoGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CSampleDemoGameView �׸���

void CSampleDemoGameView::OnDraw(CDC* pDC)
{
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	if( pOldBit )
	{
		CBrush myBrush, *pOldBrush;
		myBrush.CreateSolidBrush(RGB(0, 255, 0));
		pOldBrush = memoryDC.SelectObject(&myBrush);
		//�ڿ� ���(?)���� �׸���
		memoryDC.Rectangle( 0, 0, 800, 600 );

		memoryDC.SelectObject(pOldBrush);
		myBrush.DeleteObject();

		//ĳ���� �׸���
		GetCharMgr.RenderAll( &memoryDC );

		//��_ ȭ������ �Ű� �ش�.
		pDC->BitBlt( 0, 0, 800, 600, &memoryDC, 0, 0, SRCCOPY );
	}
}


// CSampleDemoGameView ����

#ifdef _DEBUG
void CSampleDemoGameView::AssertValid() const
{
	CView::AssertValid();
}

void CSampleDemoGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSampleDemoGameDoc* CSampleDemoGameView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSampleDemoGameDoc)));
	return (CSampleDemoGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CSampleDemoGameView �޽��� ó����

//======================================
// ������
//======================================
void CSampleDemoGameView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	//���� ����
	m_dump.Begin();

	//�켱 ȭ���� ������ �ְ�
	BeginScene();
}


//======================================
// �޴� �Լ�
//======================================
void CSampleDemoGameView::OnConnConnect()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//�̹� ����Ǿ� �ִ� ���� ������ Ȯ���Ѵ�
	if( pDoc->GetConnect() || pDoc->GetConnectToG() )
	{
		MessageBox( _T("�̹� ������ ����Ǿ� �ֽ��ϴ�"), _T("Error"), MB_OK | MB_ICONINFORMATION );
		return;
	}

	CConnectDlg* pConn = new CConnectDlg;

	int result = pConn->DoModal();

	if( result == IDOK )
	{
		//����ǥ������ �ٲ۴�
		//this->SetWindowText( _T("���� �Ǿ����ϴ�.") );
	}

	delete( pConn );

	Invalidate( false );
}

void CSampleDemoGameView::OnConnDisconn()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//���� �÷��׸� ��� ���ְ�
	pDoc->SetConnect( FALSE );
	pDoc->SetLogin( FALSE );
	pDoc->SetIndexId(-1);
	pDoc->SetConnectToG( FALSE );

	//������ ��� ���ְ�
	GetNetwork.DisConnect();

	//ĳ���͸� ��� �����ְ�
	GetCharMgr.Release();
}

//======================================
// ����� ���� �Լ�
//======================================
void CSampleDemoGameView::BeginScene()
{
	//double buffer
	CClientDC dc(this);
	memoryDC.CreateCompatibleDC(&dc);
	memoryBit.CreateCompatibleBitmap(&dc, 800, 600);
	pOldBit = memoryDC.SelectObject(&memoryBit);

	//�ð�
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
// �̺�Ʈ �Լ�
//======================================
void CSampleDemoGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	++frame;

	if( frame >= 3 )
	{
		//ĳ���͵� ������
		GetCharMgr.MoveCharacter();
		Invalidate( FALSE );
	}

	CView::OnTimer(nIDEvent);
}

void CSampleDemoGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//�̹� ����Ǿ� �ִ� ���� ������ Ȯ���Ѵ�. ���� ���� ���̶�� �Է��� ���� �ʴ´�.
	if( !pDoc->GetConnectToG() )
		return;

	//////////////////////////////////////////////////////////////////////////
	CCharacter* myChar;
	myChar = GetCharMgr.GetHero();

	if( myChar == NULL )
		return;

	//Ű�� ���ȴ����� ������ ����
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CSampleDemoGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//�̹� ����Ǿ� �ִ� ���� ������ Ȯ���Ѵ�. ���� ���� ���̶�� �Է��� ���� �ʴ´�.
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

