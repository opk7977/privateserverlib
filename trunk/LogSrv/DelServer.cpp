// DelServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "DelServer.h"


// CDelServer 대화 상자입니다.

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


// CDelServer 메시지 처리기입니다.

void CDelServer::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CDelServer::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
