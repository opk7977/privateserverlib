// Join.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "Join.h"

#include "SNetwork.h"
#include "NetProtocol.h"

#include "ClientDoc.h"
#include "ClientView.h"



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
	UpdateData(TRUE);		//�Է��� �ڷ�(?)�� �޾� �´�

	if( m_csId == _T("") )
	{
		MessageBox( _T("id�� �Է��ϼ���"), _T("warning"), MB_OK );
		return;
	}

	//======================================
	// �ԷµǾ� ������ ��Ŷ�� ������.
	//======================================
	SPacket sendPacket;
	int size;

	sendPacket.SetID( CS_LOGIN_CHECK_ID );
	//id�ְ�
	size = _tcslen(m_csId.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_csId.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("��Ŷ ũ��� ���� ũ�Ⱑ �ٸ��ϴ�...\n�� �׷���?,,,�Ф�"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

}

void CJoin::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	if( pDoc->CheckID <= 0 )
	{
		MessageBox( _T("id�ߺ� üũ�� �� �ּ���"), _T("�� ����?"), MB_OK );
		return;
	}

	UpdateData( TRUE );
	if( m_csId == "" )
	{
		MessageBox( _T("id�� �Է��� �ּ���"), _T("�� ����?"), MB_OK );
		return;
	}
	if( m_csPw == "" )
	{
		MessageBox( _T("password�� �Է��� �ּ���"), _T("�� ����?"), MB_OK );
		return;
	}
	if( m_csMail == "" )
	{
		MessageBox( _T("e_mail�� �Է��� �ּ���"), _T("�� ����?"), MB_OK );
		return;
	}

	//======================================
	// �ԷµǾ� ������ ��Ŷ�� ������.
	//======================================
	SPacket sendPacket;
	int size;

	sendPacket.SetID( CS_LOGIN_CREATE_ACCOUNT );
	//id�ְ�
	size = _tcslen(m_csId.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( (TCHAR*)m_csId.GetString(), size );

	//pw�ְ�
	size = _tcslen(m_csPw.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( (TCHAR*)m_csPw.GetString(), size );

	//mail�ְ�
	size = _tcslen(m_csMail.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( (TCHAR*)m_csMail.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("��Ŷ ũ��� ���� ũ�Ⱑ �ٸ��ϴ�...\n�� �׷���?,,,�Ф�"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	OnOK();
}

void CJoin::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
