// ConnectDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SampleDemoGame.h"
#include "SampleDemoGameDoc.h"
#include "SampleDemoGameView.h"

#include "ConnectDlg.h"
#include "Network.h"


// CConnectDlg ��ȭ �����Դϴ�.

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

// CConnectDlg �޽��� ó�����Դϴ�.

void CConnectDlg::OnBnClickedBtnConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//���� ���¸� Ȯ���ϴ�.
	if( pDoc->GetConnect() )
	{
		MessageBox( _T("�̹� ������ ����Ǿ� �ֽ��ϴ�."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	//�׸��� ��� �������� ������ �Ѵ�.
	char ipAddr[20];
	sprintf_s( ipAddr, "%d.%d.%d.%d", m_iIp1, m_iIp2, m_iIp3, m_iIp4 );

	GetNetwork.Init( TRUE );
	if( !GetNetwork.ConnectNet( ipAddr, m_iPort ) )
	{
		MessageBox( _T("���ῡ �����Ͽ����ϴ�"), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	while( !pDoc->GetConnect() )
	{
		//������ �ɶ����� ��ٸ���
	}

	//������ �Ǿ��ٸ�
	pDoc->SetConnect( TRUE );
}


void CConnectDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// �켱 ��Ʈ�ѿ� �ִ� ������ ������ �Ű� �ش�
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
		MessageBox( _T("������ ����Ǿ� ���� �ʽ��ϴ�."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	//��� �׸��� ���� �ִ����� Ȯ���Ѵ�.
	if( _tcscmp( m_strId, _T("") ) == 0 )
	{
		MessageBox( _T("ID �׸��� �ۼ��ؾ� �մϴ�."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}
	if( _tcscmp( m_strPw, _T("") ) == 0 )
	{
		MessageBox( _T("PASSWORD �׸��� �ۼ��ؾ� �մϴ�."), _T("Info"), MB_OK | MB_ICONWARNING );
		return;
	}

	//�α��� ��Ŷ�� �����ش�.
	GetNetwork.SendLogin( (TCHAR*)m_strId.GetString(), (TCHAR*)m_strPw.GetString() );

	while( !pDoc->GetLogin() )
	{
		//�α����� �Ϸ� �ɶ������� ��ٸ���.
		if( pDoc->GetIndexId() == 0 )
		{
			//��Ŷ�� �޾����� �α����� ������ �ʾҴ�.
			//�ٽ� �ʱ�ȭ ���ְ� ���� ����.
			pDoc->SetIndexId( -1 );
			return;
		}
	}

	while( !pDoc->GetConnectToG() )
	{
		//���� ������ ������ �Ǳ⸦ ��ٸ���
	}

	//������ �Ǿ��ٸ� �ڽ��� ���� id�� ������
	GetNetwork.SendSessionId( pDoc->GetIndexId(), (TCHAR*)m_strId.GetString() );	

	OnOK();
}

void CConnectDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
	CSampleDemoGameDoc* pDoc = pView->GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//���� ���¸� ���!
	pDoc->SetConnect( FALSE );

	//���� ���ᵵ ���´�
	GetNetwork.DisConnect();

	OnCancel();
}

