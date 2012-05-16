// DelTable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "DelTable.h"


// CDelTable 대화 상자입니다.

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


// CDelTable 메시지 처리기입니다.

void CDelTable::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CDelTable::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
