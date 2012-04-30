// Login.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"

#include "Join.h"

#include "SNetwork.h"
#include "NetProtocol.h"

#include "ClientDoc.h"
#include "ClientView.h"



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
	CJoin* NewAccount = new CJoin;
	NewAccount->DoModal();
	delete NewAccount;

// 	CJoin NewAccount;
// 	NewAccount.Create( IDD_JOIN );
// 	NewAccount.ShowWindow( SW_SHOW );
}

void CLogin::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);		//입력한 자료(?)를 받아 온다

// 	while(1)
// 	{
// 	}
	if( m_csId == "" )
	{
		MessageBox( _T("id를 입력하세요"), _T("warning"), MB_OK );
		return;
	}

	if( m_csPw == "" )
	{
		MessageBox( _T("password를 입력하세요"), _T("warning"), MB_OK );
		return;
	}

	//======================================
	// 모두 입력되어 있으면 패킷을 보낸다.
	//======================================
	SPacket sendPacket;
	int size;

	sendPacket.SetID( CS_LOGIN_LOGIN );
	//id넣고
	size = _tcslen(m_csId.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_csId.GetString(), size );
	//pw넣고
	size = _tcslen(m_csPw.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_csPw.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("패킷 크기와 전송 크기가 다릅니다...\n왜 그럴까?,,,ㅠㅠ"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	//doc를 받아 둔다.
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd* pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if( pDoc != NULL )
		CopyMemory( pDoc->strId, (TCHAR*)(m_csId.GetString()), m_csId.GetLength()*sizeof(TCHAR) );
	else
		MessageBox( _T("CLogin::OnBnClickedOk()_pDoc가 없음"), _T("error"), MB_OK );

	OnOK();
}

void CLogin::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
