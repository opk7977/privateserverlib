// DelTable.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "DelTable.h"

#include "LogDataBase.h"


// CDelTable ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDelTable, CDialog)

CDelTable::CDelTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDelTable::IDD, pParent)
	, m_editDelTablePw(_T(""))
{
}

CDelTable::~CDelTable()
{
	/*m_comboDelLogTable.ResetContent();*/
}

void CDelTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEL_TABLE, m_comboDelLogTable);
	DDX_Text(pDX, IDC_EDIT_DELTABLE_PW, m_editDelTablePw);
	DDV_MaxChars(pDX, m_editDelTablePw, 20);
}


BEGIN_MESSAGE_MAP(CDelTable, CDialog)
	ON_BN_CLICKED(IDOK, &CDelTable::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDelTable::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDelTable �޽��� ó�����Դϴ�.

BOOL CDelTable::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//--------------------------------------
	// DB���� Table list�� ��� ���� ������ �ش�
	//--------------------------------------
	GetDB.SettingTableListCombobox( &m_comboDelLogTable );

	//--------------------------------------
	// �⺻���� �׳� 0������ ����
	//--------------------------------------
	m_comboDelLogTable.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDelTable::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ���� ���ź���
	UpdateData( TRUE );

	//���� �˻�
	if( m_editDelTablePw == "" )
	{
		MessageBox( _T("PassWord�� �Է��ϼ���"), _T("�ٽ�"), MB_OK );
		return;
	}

	//��� �˻�
	if( m_editDelTablePw != password )
	{
		MessageBox( _T("PassWord�� Ʋ�Ƚ��ϴ�."), _T("������ �� �����ϴ�"), MB_OK );
		return;
	}

	int selectInt;
	CString tableName;

	selectInt = m_comboDelLogTable.GetCurSel();
	m_comboDelLogTable.GetLBText( selectInt, tableName );

	if( !GetDB.DisabledTable( (TCHAR*)tableName.GetString() ) )
		return;

	MessageBox( _T("table�� ���������� ���� �Ǿ����ϴ�"), _T("����"), MB_OK );

	OnOK();
}

void CDelTable::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}

