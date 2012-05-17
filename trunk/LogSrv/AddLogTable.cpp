// AddLogTable.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "AddLogTable.h"

#include "LogDataBase.h"


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

	//--------------------------------------
	// udata�� update�޾� �´�
	//--------------------------------------
	UpdateData( TRUE );

	//--------------------------------------
	// ������̸� �ȵ�
	//--------------------------------------
	if( m_editTableName == "" )
	{
		MessageBox( _T("���ڿ��δ� table�� ���� �� �����ϴ�."), _T("�ٽ�"), MB_OK );
		return;
	}

	if( !GetDB.CreateLogTable( (TCHAR*)m_editTableName.GetString() ) )
	{
		MessageBox( _T("���̺� ������ �����Ͽ����ϴ�. ���ڿ� Ȥ�� DB�� �ٽ� Ȯ���� �ּ���."), _T("�˼�"), MB_OK );
		return;
	}

	MessageBox( _T("���̺� ���� �Ϸ�."), _T("����"), MB_OK );

	OnOK();
}

void CAddLogTable::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
