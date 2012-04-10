// Join.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Join.h"


// CJoin 대화 상자입니다.

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


// CJoin 메시지 처리기입니다.

void CJoin::OnBnClickedCheckid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CJoin::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CJoin::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
