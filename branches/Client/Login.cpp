// Login.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"


// CLogin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
	, m_csId(_T(""))
	, m_csPw(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOGIN_ID, m_csId);
	DDV_MaxChars(pDX, m_csId, 20);
	DDX_Text(pDX, IDC_LOGIN_PW, m_csPw);
	DDV_MaxChars(pDX, m_csPw, 20);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	ON_BN_CLICKED(IDC_JOIN, &CLogin::OnBnClickedJoin)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLogin 메시지 처리기입니다.

void CLogin::OnBnClickedJoin()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CLogin::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CLogin::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
