// Room.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Room.h"

#include "Scheduler.h"

#include "Network.h"

// CString RoomPlayer::TeamString[2] = { {_T("Att")}, {_T("Def")} };
// CString RoomPlayer::ReadyString[2] = { {_T("Non")}, {_T("Ready")} };


// CRoom

IMPLEMENT_DYNCREATE(CRoom, CFormView)

CRoom::CRoom()
: CFormView(CRoom::IDD)
, m_chatEdit(_T(""))
{
// 	CreateCharSpace();
// 	GetScheduler.m_pRoom = this;
// 	GetScheduler.Init();
	TeamString[0] = _T("Att");
	TeamString[1] = _T("Def");
	ReadyString[0] = _T("Non");
	ReadyString[1] = _T("Ready");
	m_itMe = NULL;
	m_leader = NULL;
}

CRoom::~CRoom()
{
	Release();
}

void CRoom::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ROOM_TITLE, m_csTitle);

	DDX_Control(pDX, IDC_PLAYER1_ID, m_player_ID[0]);
	DDX_Control(pDX, IDC_PLAYER2_ID, m_player_ID[1]);
	DDX_Control(pDX, IDC_PLAYER3_ID, m_player_ID[2]);
	DDX_Control(pDX, IDC_PLAYER4_ID, m_player_ID[3]);
	DDX_Control(pDX, IDC_PLAYER5_ID, m_player_ID[4]);
	DDX_Control(pDX, IDC_PLAYER6_ID, m_player_ID[5]);
	DDX_Control(pDX, IDC_PLAYER7_ID, m_player_ID[6]);
	DDX_Control(pDX, IDC_PLAYER8_ID, m_player_ID[7]);

	DDX_Control(pDX, IDC_PLAYER1_TEAM, m_player_Team[0]);
	DDX_Control(pDX, IDC_PLAYER2_TEAM, m_player_Team[1]);
	DDX_Control(pDX, IDC_PLAYER3_TEAM, m_player_Team[2]);
	DDX_Control(pDX, IDC_PLAYER4_TEAM, m_player_Team[3]);
	DDX_Control(pDX, IDC_PLAYER5_TEAM, m_player_Team[4]);
	DDX_Control(pDX, IDC_PLAYER6_TEAM, m_player_Team[5]);
	DDX_Control(pDX, IDC_PLAYER7_TEAM, m_player_Team[6]);
	DDX_Control(pDX, IDC_PLAYER8_TEAM, m_player_Team[7]);

	DDX_Control(pDX, IDC_PLAYER1_READY, m_player_Ready[0]);
	DDX_Control(pDX, IDC_PLAYER2_READY, m_player_Ready[1]);
	DDX_Control(pDX, IDC_PLAYER3_READY, m_player_Ready[2]);
	DDX_Control(pDX, IDC_PLAYER4_READY, m_player_Ready[3]);
	DDX_Control(pDX, IDC_PLAYER5_READY, m_player_Ready[4]);
	DDX_Control(pDX, IDC_PLAYER6_READY, m_player_Ready[5]);
	DDX_Control(pDX, IDC_PLAYER7_READY, m_player_Ready[6]);
	DDX_Control(pDX, IDC_PLAYER8_READY, m_player_Ready[7]);

	DDX_Control(pDX, IDC_CHANGE_TEAM, m_btn_changeTeam);
	DDX_Control(pDX, IDC_READY, m_btn_Ready);
	DDX_Control(pDX, IDC_OUT_ROOM, m_btn_outRoom);
	DDX_Control(pDX, IDC_CHAT_SEND, m_btn_chatSend);

	DDX_Text(pDX, IDC_CHAT_EDIT, m_chatEdit);
	DDV_MaxChars(pDX, m_chatEdit, 50);

	DDX_Control(pDX, IDC_CHAT_EDIT, m_control_chatEdit);
	DDX_Control(pDX, IDC_CHAT_VIEW, m_ctrl_ChatView);
	DDX_Control(pDX, IDC_ROOM_LEADER, m_roomLeader);
}

BEGIN_MESSAGE_MAP(CRoom, CFormView)
	ON_BN_CLICKED(IDC_CHAT_SEND, &CRoom::OnBnClickedChatSend)
	ON_BN_CLICKED(IDC_CHANGE_TEAM, &CRoom::OnBnClickedChangeTeam)
	ON_BN_CLICKED(IDC_READY, &CRoom::OnBnClickedReady)
END_MESSAGE_MAP()


// CRoom 진단입니다.

#ifdef _DEBUG
void CRoom::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRoom::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

// CRoom 메시지 처리기입니다.

void CRoom::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CreateCharSpace();
	GetScheduler.m_pRoom = this;
	GetScheduler.Init();
}

void CRoom::OnBnClickedChatSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//채팅 보내기
	UpdateData( TRUE );

	if( m_chatEdit == _T("") || m_chatEdit == _T(" ") || m_chatEdit == _T("  ") )
		return;

	SPacket packet;
	packet.SetID( CS_LOBBY_CHAT );
	int size = _tcslen(m_chatEdit.GetString())*sizeof(TCHAR);
	packet << size;
	packet.PutDataW( (TCHAR*)(m_chatEdit.GetString()), size );

	if( GetNetwork.SendPacket( &packet ) != packet.GetPacketSize() )
		MessageBox( _T("CRoom::OnBnClickedChatSend()\n보낸 data크기가 다른데?"), _T("error"), MB_OK );

	m_chatEdit = _T("");
	UpdateData( FALSE );
}

void CRoom::OnBnClickedChangeTeam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SPacket packet;
	packet.SetID( CS_ROOM_TEAM_CHANGE );
	packet << (int)( m_itMe->GetTeam() ? 0 : 1 );

	if( GetNetwork.SendPacket( &packet ) != packet.GetPacketSize() )
		MessageBox( _T("CRoom::OnBnClickedChangeTeam()\n보낸 data크기가 다른데?"), _T("error"), MB_OK );
}

void CRoom::OnBnClickedReady()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SPacket packet;
	packet.SetID( CS_ROOM_CHAR_READY );
	packet << (int)( m_itMe->GetReady() ? 0 : 1 );

	if( GetNetwork.SendPacket( &packet ) != packet.GetPacketSize() )
		MessageBox( _T("CRoom::OnBnClickedChangeTeam()\n보낸 data크기가 다른데?"), _T("error"), MB_OK );
}

//==============================================================

void CRoom::CreateCharSpace()
{
	m_roomNo = 0;
	m_itMe = NULL;
	m_leader = NULL;
	m_playerCount = m_AttTeamCount = m_DefTeamCount = 0;

	for( int i=0; i<6; ++i )
	{
		RoomPlayer* tmpPlayer = new RoomPlayer;
		tmpPlayer->Init();
		tmpPlayer->SetRoomSortNo( i );

		m_mapPlayer[i] = tmpPlayer;
	}
}

void CRoom::Release()
{
	if( m_mapPlayer.empty() )
		return;

	for( int i=0; i<6; ++i )
	{
		delete m_mapPlayer[i];
	}
	m_mapPlayer.clear();
}

void CRoom::OpenRoom( int roomNo, TCHAR* title  )
{
	m_roomNo = roomNo;

	TCHAR tmpTitle[50] = {0,};
	wsprintf( tmpTitle, _T("[%d번방] %s"), roomNo, title );
	m_csTitle.SetWindowText( tmpTitle );

	//캐릭터 공간을 초기화
	for( int i=0; i<6; ++i )
	{
		m_mapPlayer[i]->Init();
	}

	//버튼을 활성화
	m_btn_changeTeam.EnableWindow( TRUE );
	m_btn_Ready.EnableWindow( TRUE );
	m_btn_outRoom.EnableWindow( TRUE );
	m_btn_chatSend.EnableWindow( TRUE );

	//edit text박스를 활성화 해 준다.
	m_control_chatEdit.EnableWindow( TRUE );
}

void CRoom::CloseRoom()
{
	m_csTitle.SetWindowText( _T("") );

	for( int i=0; i<6; ++i )
	{
		m_player_ID[i].SetWindowText( _T("empty") );
		m_player_Team[i].SetWindowText( _T("empty") );
		m_player_Ready[i].SetWindowText( _T("empty") );
		m_roomLeader.SetWindowText( _T("empty") );
	}

	m_btn_changeTeam.EnableWindow( FALSE );
	m_btn_Ready.EnableWindow( FALSE );
	m_btn_outRoom.EnableWindow( FALSE );
	m_btn_chatSend.EnableWindow( FALSE );

	m_control_chatEdit.EnableWindow( FALSE );
}

void CRoom::AddPlayer( int sessionId, TCHAR* ID, int team, int ready )
{
	if( m_playerCount >= 8 )
		return;

	//정보를 저장하고
	m_mapPlayer[m_playerCount]->SetSessionID( sessionId );
	m_mapPlayer[m_playerCount]->SetMyID( ID );
	m_mapPlayer[m_playerCount]->SetTeam( team );
	m_mapPlayer[m_playerCount]->SetReady( ready );

	m_player_ID[m_playerCount].SetWindowText( ID );
	m_player_Team[m_playerCount].SetWindowText( TeamString[team] );
	m_player_Ready[m_playerCount].SetWindowText( ReadyString[ready] );

	++m_playerCount;
}

void CRoom::DelPlayer( int SessionId )
{
	//방에 그려져 있는 순서를 찾는다
	RoomPlayer* tmpChar = FindChar( SessionId );
	if( tmpChar == NULL )
	{
		MessageBox( _T("CRoom::SetMe()\n설정 실패"), _T("error"), MB_OK );
		return;
	}

	int sortNo = tmpChar->GetRoomSortNo();

	m_player_ID[sortNo].SetWindowText( _T("empty") );
	m_player_Team[sortNo].SetWindowText( _T("empty") );
	m_player_Ready[sortNo].SetWindowText( _T("empty") );

	m_mapPlayer[sortNo]->Init();

	--m_playerCount;
}

void CRoom::ChangeReady( int sessionId, int ready )
{
	RoomPlayer* tmpChar = FindChar( sessionId );
	if( tmpChar == NULL )
	{
		MessageBox( _T("CRoom::SetMe()\n설정 실패"), _T("error"), MB_OK );
		return;
	}

	int sortNo = tmpChar->GetRoomSortNo();

	TCHAR tmpchar[4]={0,};
	_itot_s( ready, tmpchar, 4 );
	m_player_Ready[sortNo].SetWindowText( tmpchar );
}

void CRoom::ChangeTeam( int sessionId, int team )
{
	RoomPlayer* tmpChar = FindChar( sessionId );
	if( tmpChar == NULL )
	{
		MessageBox( _T("CRoom::SetMe()\n설정 실패"), _T("error"), MB_OK );
		return;
	}

	int sortNo = tmpChar->GetRoomSortNo();

	TCHAR tmpchar[4]={0,};
	_itot_s( team, tmpchar, 4 );
	m_player_Team[sortNo].SetWindowText( tmpchar );
}

void CRoom::SetMe( int sessionId )
{
	RoomPlayer* tmpChar = FindChar( sessionId );
	if( tmpChar == NULL )
	{
		MessageBox( _T("CRoom::SetMe()\n설정 실패"), _T("error"), MB_OK );
		return;
	}

	m_itMe = tmpChar;
}

void CRoom::SetLeader( int sessionId )
{
	RoomPlayer* tmpChar = FindChar( sessionId );
	if( tmpChar == NULL )
	{
		MessageBox( _T("CRoom::SetMe()\n설정 실패"), _T("error"), MB_OK );
		return;
	}

	m_leader = tmpChar;

	m_roomLeader.SetWindowText( (TCHAR*)tmpChar->GetMyID() );
}

RoomPlayer* CRoom::FindChar( int sessionId )
{
	for( int i = 0; i<6; ++i )
	{
		if( m_mapPlayer[i]->GetSessionID() == sessionId )
		{
			return m_mapPlayer[i];
		}
	}
	return NULL;
}

#endif
#endif //_DEBUG


