// ConnectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SampleDemoGame.h"
#include "SampleDemoGameDoc.h"
#include "SampleDemoGameView.h"

#include "ConnectDlg.h"
#include "Network.h"


// CConnectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConnectDlg, CDialog)

CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
	, m_iIp1(192)
	, m_iIp2(168)
	, m_iIp3(0)
	, m_iIp4(70)
	, m_iPort(8900)
	, m_strId(_T(""))
	, m_strPw(_T(""))
{
	time.Init();
}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iIp1);
	DDV_MinMaxInt(pDX, m_iIp1, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_iIp2);
	DDV_MinMaxInt(pDX, m_iIp2, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_iIp3);
	DDV_MinMaxInt(pDX, m_iIp3, 0, 255);
	DDX_Text(pDX, IDC_EDIT4, m_iIp4);
	DDV_MinMaxInt(pDX, m_iIp4, 0, 255);
	DDX_Text(pDX, IDC_EDIT5, m_iPort);
	DDX_Text(pDX, IDC_EDIT6, m_strId);
	DDV_MaxChars(pDX, m_strId, 10);
	DDX_Text(pDX, IDC_EDIT7, m_strPw);
	DDV_MaxChars(pDX, m_strPw, 10);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConnectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConnectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CConnectDlg::OnBnClickedBtnConnect)
END_MESSAGE_MAP()

// CConnectDlg 메시지 처리기입니다.

void CConnectDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//현재 상태를 확인하다.
	if( pDoc->GetConnect() )
	{
		MessageBox( _T("이미 서버와 연결되어 있습니다."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	//항목이 모두 적혔으면 연결을 한다.
	char ipAddr[20];
	sprintf_s( ipAddr, "%d.%d.%d.%d", m_iIp1, m_iIp2, m_iIp3, m_iIp4 );

	GetNetwork.Init( TRUE );
	if( !GetNetwork.ConnectNet( ipAddr, m_iPort ) )
	{
		MessageBox( _T("연결에 실패하였습니다"), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	while( !pDoc->GetConnect() )
	{
		//연결이 될때까지 기다리자
	}

	//연결이 되었다면
	pDoc->SetConnect( TRUE );
}


void CConnectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 우선 컨트롤에 있는 내용을 변수로 옮겨 준다
	UpdateData();

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if( !pDoc->GetConnect() )
	{
		MessageBox( _T("서버와 연결되어 있지 않습니다."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	//모든 항목이 적혀 있는지를 확인한다.
	if( _tcscmp( m_strId, _T("") ) == 0 )
	{
		MessageBox( _T("ID 항목을 작성해야 합니다."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}
	if( _tcscmp( m_strPw, _T("") ) == 0 )
	{
		MessageBox( _T("PASSWORD 항목을 작성해야 합니다."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	//로그인 패킷을 보내준다.
	GetNetwork.SendLogin( (TCHAR*)m_strId.GetString(), (TCHAR*)m_strPw.GetString() );

	while( !pDoc->GetLogin() )
	{
		//로그인이 완료 될때까지를 기다린다.
		if( pDoc->GetIndexId() == 0 )
		{
			//패킷은 받았지만 로그인이 되지는 않았다.
			//다시 초기화 해주고 돌아 간다.
			pDoc->SetIndexId( -1 );
			return;
		}
	}

	while( !pDoc->GetConnectToG() )
	{
		//게임 서버와 연결이 되기를 기다린다
	}

	//연결이 되었다면 자신의 세션 id를 보낸다
	GetNetwork.SendSessionId( pDoc->GetIndexId(), (TCHAR*)m_strId.GetString() );	

	OnOK();
}

void CConnectDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//연결 상태를 취소!
	pDoc->SetConnect( FALSE );

	//소켓 연결도 끊는다
	GetNetwork.DisConnect();

	OnCancel();
}

