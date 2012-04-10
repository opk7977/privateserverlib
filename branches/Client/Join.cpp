// Join.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "Join.h"


// CJoin ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CJoin, CDialog)

CJoin::CJoin(CWnd* pParent /*=NULL*/)
	: CDialog(CJoin::IDD, pParent)
	, m_csId(_T(""))
	, m_csPw(_T(""))
	, m_csMail(_T(""))
{

}

CJoin::~CJoin()
{
}

void CJoin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_JOIN_ID, m_csId);
	DDV_MaxChars(pDX, m_csId, 20);
	DDX_Text(pDX, IDC_JOIN_PW, m_csPw);
	DDV_MaxChars(pDX, m_csPw, 20);
	DDX_Text(pDX, IDC_JOIN_MAIL, m_csMail);
	DDV_MaxChars(pDX, m_csMail, 40);
}


BEGIN_MESSAGE_MAP(CJoin, CDialog)
	ON_BN_CLICKED(IDC_CHECKID, &CJoin::OnBnClickedCheckid)
	ON_BN_CLICKED(IDOK, &CJoin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CJoin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CJoin �޽��� ó�����Դϴ�.

void CJoin::OnBnClickedCheckid()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CJoin::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CJoin::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
