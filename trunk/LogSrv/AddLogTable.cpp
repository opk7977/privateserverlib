// AddLogTable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "AddLogTable.h"


// CAddLogTable 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddLogTable, CDialog)

CAddLogTable::CAddLogTable(CWnd* pParent /*=NULL*/)
	: CDialog(CAddLogTable::IDD, pParent)
	, m_editTableName(_T(""))
{

}

CAddLogTable::~CAddLogTable()
{
}

void CAddLogTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADD_TABLE_NAME, m_editTableName);
	DDV_MaxChars(pDX, m_editTableName, 20);
}


BEGIN_MESSAGE_MAP(CAddLogTable, CDialog)
	ON_BN_CLICKED(IDOK, &CAddLogTable::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddLogTable::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddLogTable 메시지 처리기입니다.

void CAddLogTable::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CAddLogTable::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
