// NewRoom.cpp : 구현 파일입니다.
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



// CNewRoom 대화 상자입니다.

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


// CNewRoom 메시지 처리기입니다.

void CNewRoom::SetRoomNum( int n )
{
	m_roomNum = n;
}

void CNewRoom::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !m_pRoom )
		return;

	UpdateData(TRUE);
	
	//check
	if( m_RoomTitle == "" )
	{
		MessageBox( _T("CNewRoom::OnBnClickedOk()\n입력안하고 누르지마"), _T("error"), MB_OK );
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
		MessageBox( _T("CNewRoom::OnBnClickedOk()\n패킷 크기와 전송 크기가 다릅니다...\n왜 그럴까?,,,ㅠㅠ"), _T("error"), MB_OK | MB_ICONERROR );
		return;
	}

	//doc를 받아 둔다.
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
		//만들어지지 않음
		MessageBox( _T("CNewRoom::OnBnClickedOk()\n방생성 실패!"), _T("info"), MB_OK );
		return;
	}

	//잘 만들어짐..
	MessageBox( _T("CNewRoom::OnBnClickedOk()\n방생에 성공했습니다"), _T("info"), MB_OK );


	Character* me = GetCharMgr.GetMe();

	m_pRoom->OpenRoom( m_roomNum, (TCHAR*)(m_RoomTitle.GetString()) );
	m_pRoom->AddPlayer( me->GetSessionID(), me->GetID(), me->GetTeam(), 0 );
	m_pRoom->SetMe( me->GetSessionID() );
	m_pRoom->SetLeader( me->GetSessionID() );

	//나는 방으로 들어가게 되니까 로비의 방정보를 모두 지운다
	GetRoomMgr.ClearAllRoom();
	
	//캐릭터 정보도 모두 지운다.
	GetCharMgr.ClearCharacter();
	
	OnOK();
}

void CNewRoom::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
