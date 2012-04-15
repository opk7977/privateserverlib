
// ClientView.cpp : CClientView Ŭ������ ����
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CClientView ����/�Ҹ�

CClientView::CClientView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CClientView::~CClientView()
{
}

BOOL CClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CClientView �׸���

void CClientView::OnDraw(CDC* /*pDC*/)
{
	CClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CClientView �μ�

BOOL CClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CClientView ����

#ifdef _DEBUG
void CClientView::AssertValid() const
{
	CView::AssertValid();
}

void CClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClientDoc* CClientView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClientDoc)));
	return (CClientDoc*)m_pDocument;
}
#endif //_DEBUG


//======================================
// �޴� �Լ�
//======================================
// CClientView �޽��� ó����

void CClientView::OnNetConn()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClientView::OnNetLogin()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

//======================================
