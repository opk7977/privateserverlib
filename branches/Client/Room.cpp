// Room.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Room.h"


// CRoom

IMPLEMENT_DYNCREATE(CRoom, CFormView)

CRoom::CRoom()
	: CFormView(CRoom::IDD)
	, m_csTitle(_T(""))
	, m_csPlayer01_id(_T(""))
	, m_csPlayer02_id(_T(""))
	, m_csPlayer03_id(_T(""))
	, m_csPlayer04_id(_T(""))
	, m_csPlayer05_id(_T(""))
	, m_csPlayer06_id(_T(""))
	, m_csPlayer07_id(_T(""))
	, m_csPlayer08_id(_T(""))
	, m_csPlayer01_Team(_T(""))
	, m_csPlayer02_Team(_T(""))
	, m_csPlayer03_Team(_T(""))
{

}

CRoom::~CRoom()
{
}

void CRoom::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROOM_TITLE, m_csTitle);
	DDV_MaxChars(pDX, m_csTitle, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER01_ID, m_csPlayer01_id);
	DDV_MaxChars(pDX, m_csPlayer01_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER02_ID, m_csPlayer02_id);
	DDV_MaxChars(pDX, m_csPlayer02_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER03_ID, m_csPlayer03_id);
	DDV_MaxChars(pDX, m_csPlayer03_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER04_ID, m_csPlayer04_id);
	DDV_MaxChars(pDX, m_csPlayer04_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER05_ID, m_csPlayer05_id);
	DDV_MaxChars(pDX, m_csPlayer05_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER06_ID, m_csPlayer06_id);
	DDV_MaxChars(pDX, m_csPlayer06_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER07_ID, m_csPlayer07_id);
	DDV_MaxChars(pDX, m_csPlayer07_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER08_ID, m_csPlayer08_id);
	DDV_MaxChars(pDX, m_csPlayer08_id, 50);
	DDX_Text(pDX, IDC_ROOM_PLAYER01_TEAM, m_csPlayer01_Team);
	DDV_MaxChars(pDX, m_csPlayer01_Team, 10);
	DDX_Text(pDX, IDC_ROOM_PLAYER02_TEAM, m_csPlayer02_Team);
	DDV_MaxChars(pDX, m_csPlayer02_Team, 10);
	DDX_Text(pDX, IDC_ROOM_PLAYER03_TEAM, m_csPlayer03_Team);
	DDV_MaxChars(pDX, m_csPlayer03_Team, 10);
}

BEGIN_MESSAGE_MAP(CRoom, CFormView)
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
#endif
#endif //_DEBUG


// CRoom 메시지 처리기입니다.
