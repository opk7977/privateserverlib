// ServerSetting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"

#include "LogSrvDoc.h"
#include "LogSrvView.h"

#include "ServerSetting.h"

#include "LogDataBase.h"

// CServerSetting ��ȭ �����Դϴ�.

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


// CServerSetting �޽��� ó�����Դϴ�.

BOOL CServerSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	GetDB.SettingTableListCombobox( &m_comboUseTable );

	m_comboUseTable.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CServerSetting::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData( TRUE );

	//ip�� ��ħ
	char strIP[20]={0,};
	sprintf_s( strIP, 20, "%d.%d.%d.%d", m_iIp1, m_iIp2, m_iIp3, m_iIp4 );
	
	//����� ���̺� �̸� ���
	TCHAR useTbl[20]={0,};
	int sel = m_comboUseTable.GetCurSel();
	m_comboUseTable.GetLBText( sel, useTbl );


	//pDoc�� ��� �´�
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
