// AddLogTable.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "AddLogTable.h"


// CAddLogTable ��ȭ �����Դϴ�.

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


// CAddLogTable �޽��� ó�����Դϴ�.

void CAddLogTable::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CAddLogTable::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
