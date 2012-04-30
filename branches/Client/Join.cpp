// Join.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Join.h"

#include "SNetwork.h"
#include "NetProtocol.h"

#include "ClientDoc.h"
#include "ClientView.h"



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
	UpdateData(TRUE);		//입력한 자료(?)를 받아 온다

	if( m_csId == _T("") )
	{
		MessageBox( _T("id를 입력하세요"), _T("warning"), MB_OK );
		return;
	}

	//======================================
	// 입력되어 있으면 패킷을 보낸다.
	//======================================
	SPacket sendPacket;
	int size;

	sendPacket.SetID( CS_LOGIN_CHECK_ID );
	//id넣고
	size = _tcslen(m_csId.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_csId.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("패킷 크기와 전송 크기가 다릅니다...\n왜 그럴까?,,,ㅠㅠ"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

}

void CJoin::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	if( pDoc->CheckID <= 0 )
	{
		MessageBox( _T("id중복 체크를 해 주세요"), _T("왜 안해?"), MB_OK );
		return;
	}

	UpdateData( TRUE );
	if( m_csId == "" )
	{
		MessageBox( _T("id를 입력해 주세요"), _T("왜 안해?"), MB_OK );
		return;
	}
	if( m_csPw == "" )
	{
		MessageBox( _T("password를 입력해 주세요"), _T("왜 안해?"), MB_OK );
		return;
	}
	if( m_csMail == "" )
	{
		MessageBox( _T("e_mail을 입력해 주세요"), _T("왜 안해?"), MB_OK );
		return;
	}

	//======================================
	// 입력되어 있으면 패킷을 보낸다.
	//======================================
	SPacket sendPacket;
	int size;

	sendPacket.SetID( CS_LOGIN_CREATE_ACCOUNT );
	//id넣고
	size = _tcslen(m_csId.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( (TCHAR*)m_csId.GetString(), size );

	//pw넣고
	size = _tcslen(m_csPw.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( (TCHAR*)m_csPw.GetString(), size );

	//mail넣고
	size = _tcslen(m_csMail.GetString())*sizeof(TCHAR);
	sendPacket << size;
	sendPacket.PutData( (TCHAR*)m_csMail.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("패킷 크기와 전송 크기가 다릅니다...\n왜 그럴까?,,,ㅠㅠ"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	OnOK();
}

void CJoin::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
