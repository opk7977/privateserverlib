
// LogSrvView.cpp : CLogSrvView Ŭ������ ����
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

// CLogSrvView ����/�Ҹ�

CLogSrvView::CLogSrvView()
	: CFormView(CLogSrvView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

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

	//�޺��ڽ��� �߰��Ҳ� ����
	m_logDB->SettingServerListCombobox( &m_ServerCombo );
	m_logDB->SettingTableListCombobox( &m_TableCombo );
	m_logDB->SettingLogLevelCombobox( &m_LogLevelCombo );

	//�׸��� �޺��ڽ� SetCur����
	m_ServerCombo.SetCurSel(0);
	m_TableCombo.SetCurSel(0);
	m_LogLevelCombo.SetCurSel(0);
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//�׸� ���� �޾� �ɽô�
	TCHAR tableName[30]={0,};
	TCHAR serverName[30]={0,};
	TCHAR Loglevel[30]={0,};

	int selNum;

	//���̺� �̸�
	selNum = m_TableCombo.GetCurSel();
	m_TableCombo.GetLBText( selNum, tableName );

	//�����̸�
	selNum = m_ServerCombo.GetCurSel();
	m_ServerCombo.GetLBText( selNum, serverName );

	//�α׷��� �̸�
	selNum = m_LogLevelCombo.GetCurSel();
	m_LogLevelCombo.GetLBText( selNum, Loglevel );

	if( !m_logDB->SetLogList( tableName, serverName, Loglevel, &m_logList ) )
		MessageBox( _T("�ȵǴµ�?"), _T("����"), MB_OK );
}
