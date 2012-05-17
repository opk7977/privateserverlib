// ServerSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"

#include "LogSrvDoc.h"
#include "LogSrvView.h"

#include "ServerSetting.h"

#include "LogDataBase.h"

// CServerSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(CServerSetting, CDialog)

CServerSetting::CServerSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSetting::IDD, pParent)
	, m_iIp1(192)
	, m_iIp2(168)
	, m_iIp3(0)
	, m_iIp4(56)
	, m_srvPort(7800)
{

}

CServerSetting::~CServerSetting()
{
}

void CServerSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP1, m_iIp1);
	DDV_MinMaxInt(pDX, m_iIp1, 0, 255);
	DDX_Text(pDX, IDC_EDIT_IP2, m_iIp2);
	DDV_MinMaxInt(pDX, m_iIp2, 0, 255);
	DDX_Text(pDX, IDC_EDIT_IP3, m_iIp3);
	DDV_MinMaxInt(pDX, m_iIp3, 0, 255);
	DDX_Text(pDX, IDC_EDIT_IP4, m_iIp4);
	DDV_MinMaxInt(pDX, m_iIp4, 0, 255);
	DDX_Text(pDX, IDC_EDIT_PORT, m_srvPort);
	DDV_MinMaxInt(pDX, m_srvPort, 0, 100000);
	DDX_Control(pDX, IDC_COMBO_SETTING_TABLE, m_comboUseTable);
}


BEGIN_MESSAGE_MAP(CServerSetting, CDialog)
	ON_BN_CLICKED(IDOK, &CServerSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerSetting::OnBnClickedCancel)
END_MESSAGE_MAP()


// CServerSetting 메시지 처리기입니다.

BOOL CServerSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDB.SettingTableListCombobox( &m_comboUseTable );

	m_comboUseTable.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CServerSetting::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData( TRUE );

	//ip를 합침
	char strIP[20]={0,};
	sprintf_s( strIP, 20, "%d.%d.%d.%d", m_iIp1, m_iIp2, m_iIp3, m_iIp4 );
	
	//사용할 테이블 이름 얻기
	TCHAR useTbl[20]={0,};
	int sel = m_comboUseTable.GetCurSel();
	m_comboUseTable.GetLBText( sel, useTbl );


	//pDoc를 얻어 온다
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd* pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CLogSrvView* pView = (CLogSrvView*) pChild->GetActiveView();
	CLogSrvDoc* pDoc = pView->GetDocument();

	pDoc->SetPort( m_srvPort );
	pDoc->SetIP( strIP );

	pDoc->SetLogTbl( useTbl );
	GetDB.SetTable( useTbl );
	
	OnOK();
}

void CServerSetting::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
