
// LogSrvView.cpp : CLogSrvView 클래스의 구현
//

#include "stdafx.h"
#include "LogSrv.h"

#include "LogSrvDoc.h"
#include "LogSrvView.h"

#include "LogDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogSrvView

IMPLEMENT_DYNCREATE(CLogSrvView, CFormView)

BEGIN_MESSAGE_MAP(CLogSrvView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CLogSrvView::OnBnClickedButtonQuery)
END_MESSAGE_MAP()

// CLogSrvView 생성/소멸

CLogSrvView::CLogSrvView()
	: CFormView(CLogSrvView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_logDB = &GetDB;
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
	DDX_Control(pDX, IDC_LOG_LIST, m_logList);
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

	CLogSrvDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->m_view = this;

	//콤보박스에 추가할꺼 하장
	m_logDB->SettingServerListCombobox( &m_ServerCombo );
	m_logDB->SettingTableListCombobox( &m_TableCombo );
	m_logDB->SettingLogLevelCombobox( &m_LogLevelCombo );

	//그리고 콤보박스 SetCur하좌
	m_ServerCombo.SetCurSel(0);
	m_TableCombo.SetCurSel(0);
	m_LogLevelCombo.SetCurSel(0);
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

void CLogSrvView::SettingTableList()
{
	m_logDB->SettingTableListCombobox( &m_TableCombo );

	m_TableCombo.SetCurSel(0);
}

void CLogSrvView::SettingServerList()
{
	m_logDB->SettingServerListCombobox( &m_ServerCombo );

	m_ServerCombo.SetCurSel(0);
}

void CLogSrvView::OnBnClickedButtonQuery()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//항목 값을 받아 옵시다
	TCHAR tableName[30]={0,};
	TCHAR serverName[30]={0,};
	TCHAR Loglevel[30]={0,};

	int selNum;

	//테이블 이름
	selNum = m_TableCombo.GetCurSel();
	m_TableCombo.GetLBText( selNum, tableName );

	//서버이름
	selNum = m_ServerCombo.GetCurSel();
	m_ServerCombo.GetLBText( selNum, serverName );

	//로그레벨 이름
	selNum = m_LogLevelCombo.GetCurSel();
	m_LogLevelCombo.GetLBText( selNum, Loglevel );

	if( !m_logDB->SetLogList( tableName, serverName, Loglevel, &m_logList ) )
		MessageBox( _T("안되는데?"), _T("실패"), MB_OK );
}
