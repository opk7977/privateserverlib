// Room.cpp : ���� �����Դϴ�.
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


// CRoom �����Դϴ�.

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


// CRoom �޽��� ó�����Դϴ�.
