// Join.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Client.h"
#include "Join.h"


// CJoin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CJoin, CDialog)

CJoin::CJoin(CWnd* pParent /*=NULL*/)
	: CDialog(CJoin::IDD, pParent)
{

}

CJoin::~CJoin()
{
}

void CJoin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CJoin, CDialog)
END_MESSAGE_MAP()


// CJoin 메시지 처리기입니다.
