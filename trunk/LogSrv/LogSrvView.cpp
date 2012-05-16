
// LogSrvView.cpp : CLogSrvView Ŭ������ ����
//

#include "stdafx.h"
#include "LogSrv.h"

#include "LogSrvDoc.h"
#include "LogSrvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogSrvView

IMPLEMENT_DYNCREATE(CLogSrvView, CFormView)

BEGIN_MESSAGE_MAP(CLogSrvView, CFormView)
END_MESSAGE_MAP()

// CLogSrvView ����/�Ҹ�

CLogSrvView::CLogSrvView()
	: CFormView(CLogSrvView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CLogSrvView::~CLogSrvView()
{
}

void CLogSrvView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECT_TABLE, m_TableCombo);
	DDX_Control(pDX, IDC_COMBO_SELECT_SERVER, m_ServerCombo);
	DDX_Control(pDX, IDC_COMBO_LOGLEVEL, m_LogLevelCombo);
	DDX_Control(pDX, IDC_LOG_LIST, m_LogList);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_SendQuery);
}

BOOL CLogSrvView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CLogSrvView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CLogSrvView ����

#ifdef _DEBUG
void CLogSrvView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLogSrvView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLogSrvDoc* CLogSrvView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogSrvDoc)));
	return (CLogSrvDoc*)m_pDocument;
}
#endif //_DEBUG


// CLogSrvView �޽��� ó����
