// DelTable.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "DelTable.h"


// CDelTable ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDelTable, CDialog)

CDelTable::CDelTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDelTable::IDD, pParent)
	, m_editDelTablePw(_T(""))
{

}

CDelTable::~CDelTable()
{
}

void CDelTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEL_LOGTABLE, m_comboDelTable);
	DDX_Text(pDX, IDC_EDIT_DELTABLE_PW, m_editDelTablePw);
	DDV_MaxChars(pDX, m_editDelTablePw, 20);
}


BEGIN_MESSAGE_MAP(CDelTable, CDialog)
	ON_BN_CLICKED(IDOK, &CDelTable::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDelTable::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDelTable �޽��� ó�����Դϴ�.

void CDelTable::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}

void CDelTable::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
