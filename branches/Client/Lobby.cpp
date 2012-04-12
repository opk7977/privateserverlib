// Lobby.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Lobby.h"


// CLobby

IMPLEMENT_DYNCREATE(CLobby, CFormView)

CLobby::CLobby()
	: CFormView(CLobby::IDD)
{

}

CLobby::~CLobby()
{
}

void CLobby::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLobby, CFormView)
END_MESSAGE_MAP()


// CLobby 진단입니다.

#ifdef _DEBUG
void CLobby::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLobby::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLobby 메시지 처리기입니다.
