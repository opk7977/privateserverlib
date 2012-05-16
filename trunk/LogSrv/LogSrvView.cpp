
// LogSrvView.cpp : CLogSrvView 클래스의 구현
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

// CLogSrvView 생성/소멸

CLogSrvView::CLogSrvView()
	: CFormView(CLogSrvView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CLogSrvView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CLogSrvView 진단

#ifdef _DEBUG
void CLogSrvView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLogSrvView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CLogSrvDoc* CLogSrvView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogSrvDoc)));
	return (CLogSrvDoc*)m_pDocument;
}
#endif //_DEBUG


// CLogSrvView 메시지 처리기
