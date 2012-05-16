// DelServer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "DelServer.h"


// CDelServer ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDelServer, CDialog)

CDelServer::CDelServer(CWnd* pParent /*=NULL*/)
	: CDialog(CDelServer::IDD, pParent)
	, m_editDelPw(_T(""))
{

}

CDelServer::~CDelServer()
{
}

void CDelServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEL_SERVER, m_comboServer);
	DDX_Text(pDX, IDC_EDIT_DELSERVER_PW, m_editDelPw);
	DDV_MaxChars(pDX, m_editDelPw, 20);
}


BEGIN_MESSAGE_MAP(CDelServer, CDialog)
	ON_BN_CLICKED(IDOK, &CDelServer::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDelServer::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDelServer �޽��� ó�����Դϴ�.

void CDelServer::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CDelServer::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
