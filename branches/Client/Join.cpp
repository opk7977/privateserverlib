// Join.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Client.h"
#include "Join.h"


// CJoin ��ȭ �����Դϴ�.

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


// CJoin �޽��� ó�����Դϴ�.
