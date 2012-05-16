// ServerSetting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "ServerSetting.h"


// CServerSetting ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CServerSetting, CDialog)

CServerSetting::CServerSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSetting::IDD, pParent)
	, m_iIp1(0)
	, m_iIp2(0)
	, m_iIp3(0)
	, m_iIp4(0)
	, m_srvPort(0)
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

void CServerSetting::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CServerSetting::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
