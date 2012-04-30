// NewRoom.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "NewRoom.h"

#include "SNetwork.h"
#include "NetProtocol.h"

#include "ClientDoc.h"
#include "ClientView.h"

#include "CharMgr.h"
#include "RoomMgr.h"

#include "Room.h"



// CNewRoom ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNewRoom, CDialog)

CNewRoom::CNewRoom(CWnd* pParent /*=NULL*/)
	: CDialog(CNewRoom::IDD, pParent)
	, m_RoomTitle(_T(""))
	, m_pRoom(NULL)
{

}

CNewRoom::~CNewRoom()
{
}

void CNewRoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_RoomTitle);
	DDV_MaxChars(pDX, m_RoomTitle, 30);
}


BEGIN_MESSAGE_MAP(CNewRoom, CDialog)
	ON_BN_CLICKED(IDOK, &CNewRoom::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNewRoom::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNewRoom �޽��� ó�����Դϴ�.

void CNewRoom::SetRoomNum( int n )
{
	m_roomNum = n;
}

void CNewRoom::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( !m_pRoom )
		return;

	UpdateData(TRUE);
	
	//check
	if( m_RoomTitle == "" )
	{
		MessageBox( _T("CNewRoom::OnBnClickedOk()\n�Է¾��ϰ� ��������"), _T("error"), MB_OK );
		return;
	}

	SPacket sendPacket;
	sendPacket.SetID( CS_LOBBY_CREATE_ROOM );
	sendPacket << m_roomNum;
	int size = m_RoomTitle.GetLength() * sizeof( TCHAR );
	sendPacket << size;
	sendPacket.PutDataW( (TCHAR*)m_RoomTitle.GetString(), size );

	int result = GetNetwork.SendPacket( &sendPacket );

	if( result != sendPacket.GetPacketSize() )
	{
		MessageBox( _T("CNewRoom::OnBnClickedOk()\n��Ŷ ũ��� ���� ũ�Ⱑ �ٸ��ϴ�...\n�� �׷���?,,,�Ф�"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	//doc�� �޾� �д�.
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd* pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	pDoc->isRecvResult = FALSE;

	while( !pDoc->isRecvResult )
	{
	}

	if( pDoc->Revcvresult < 0 )
	{
		//��������� ����
		MessageBox( _T("CNewRoom::OnBnClickedOk()\n����� ����!"), _T("info"), MB_OK );
		return;
	}

	//�� �������..
	MessageBox( _T("CNewRoom::OnBnClickedOk()\n����� �����߽��ϴ�"), _T("info"), MB_OK );


	Character* me = GetCharMgr.GetMe();

	m_pRoom->OpenRoom( m_roomNum, (TCHAR*)(m_RoomTitle.GetString()) );
	m_pRoom->AddPlayer( me->GetSessionID(), me->GetID(), me->GetTeam(), 0 );
	m_pRoom->SetMe( me->GetSessionID() );
	m_pRoom->SetLeader( me->GetSessionID() );

	//���� ������ ���� �Ǵϱ� �κ��� �������� ��� �����
	GetRoomMgr.ClearAllRoom();
	
	//ĳ���� ������ ��� �����.
	GetCharMgr.ClearCharacter();
	
	OnOK();
}

void CNewRoom::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
