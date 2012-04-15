// Room.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Room.h"


// CRoom

IMPLEMENT_DYNCREATE(CRoom, CFormView)

CRoom::CRoom()
	: CFormView(CRoom::IDD)
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
