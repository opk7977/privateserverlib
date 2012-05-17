// AddServer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "AddServer.h"

#include "LogDataBase.h"


// CAddServer 대화 상자입니다.

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


// CAddServer 메시지 처리기입니다.

void CAddServer::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 데이터를 받아 온다
	UpdateData( TRUE );

	// 변수의 값을 검사하자
	if( m_editServerName == "" )
	{
		MessageBox( _T("서버 이름에 빈공간을 쓰지 마세요."), _T("다시"), MB_OK );
		return;
	}
	//숫자는 mfc에서 알아서 해줌

	if( !GetDB.InsertServerID( (TCHAR*)m_editServerName.GetString(), m_iServerID ) )
	{
		MessageBox( _T("[추가 실패!!]\n실패 이유\n\t- 서버이름 중복\n\t- 서버ID중복\n\t- 문자열 데이터 이상\n등이 있습니다.\n\n이유까지 찾아 드릴 수 없습니다. 다시..."), _T("죄송"), MB_OK );
		return;
	}
	MessageBox( _T("[추가 성공]\nServerID가 추가 되었습니다."), _T("성공"), MB_OK );

	OnOK();
}

void CAddServer::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
