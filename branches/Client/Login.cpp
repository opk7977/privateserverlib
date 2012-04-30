// Login.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"

#include "Join.h"

#include "SNetwork.h"
#include "NetProtocol.h"

#include "ClientDoc.h"
#include "ClientView.h"



// CLogin ��ȭ �����Դϴ�.

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


// CLogin �޽��� ó�����Դϴ�.

void CLogin::OnBnClickedJoin()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CJoin* NewAccount = new CJoin;
	NewAccount->DoModal();
	delete NewAccount;

// 	CJoin NewAccount;
// 	NewAccount.Create( IDD_JOIN );
// 	NewAccount.ShowWindow( SW_SHOW );
}

void CLogin::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);		//�Է��� �ڷ�(?)�� �޾� �´�

// 	while(1)
// 	{
// 	}
	if( m_csId == "" )
	{
		MessageBox( _T("id�� �Է��ϼ���"), _T("warning"), MB_OK );
		return;
	}

	if( m_csPw == "" )
	{
		MessageBox( _T("password�� �Է��ϼ���"), _T("warning"), MB_OK );
		return;
	}

	//======================================
	// ��� �ԷµǾ� ������ ��Ŷ�� ������.
	//======================================
	SPacket sendPacket;
	int size;

	sendPacket.SetID( CS_LOGIN_LOGIN );
	//id�ְ�
	size = _tcslen(m_csId.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_csId.GetString(), size );
	//pw�ְ�
	size = _tcslen(m_csPw.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_csPw.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("��Ŷ ũ��� ���� ũ�Ⱑ �ٸ��ϴ�...\n�� �׷���?,,,�Ф�"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	//doc�� �޾� �д�.
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd* pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if( pDoc != NULL )
		CopyMemory( pDoc->strId, (TCHAR*)(m_csId.GetString()), m_csId.GetLength()*sizeof(TCHAR) );
	else
		MessageBox( _T("CLogin::OnBnClickedOk()_pDoc�� ����"), _T("error"), MB_OK );

	OnOK();
}

void CLogin::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
