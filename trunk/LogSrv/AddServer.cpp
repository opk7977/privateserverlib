// AddServer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "AddServer.h"


// CAddServer ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAddServer, CDialog)

CAddServer::CAddServer(CWnd* pParent /*=NULL*/)
	: CDialog(CAddServer::IDD, pParent)
	, m_editServerName(_T(""))
	, m_iServerID(0)
{

}

CAddServer::~CAddServer()
{
}

void CAddServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADD_SERVER_NAME, m_editServerName);
	DDV_MaxChars(pDX, m_editServerName, 20);
	DDX_Text(pDX, IDC_EDIT_ADD_SERVER_ID, m_iServerID);
	DDV_MinMaxInt(pDX, m_iServerID, 1, 10000000);
}


BEGIN_MESSAGE_MAP(CAddServer, CDialog)
	ON_BN_CLICKED(IDOK, &CAddServer::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddServer::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddServer �޽��� ó�����Դϴ�.

void CAddServer::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CAddServer::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
