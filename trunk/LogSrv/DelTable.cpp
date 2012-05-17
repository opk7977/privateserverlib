// DelTable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "DelTable.h"

#include "LogDataBase.h"


// CDelTable 대화 상자입니다.

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


// CDelTable 메시지 처리기입니다.

BOOL CDelTable::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//--------------------------------------
	// DB에서 Table list를 모두 얻어와 셋팅해 준다
	//--------------------------------------
	GetDB.SettingTableListCombobox( &m_comboDelLogTable );

	//--------------------------------------
	// 기본으로 그냥 0번으로 셋팅
	//--------------------------------------
	m_comboDelLogTable.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDelTable::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 변수 갱신부터
	UpdateData( TRUE );

	//변수 검사
	if( m_editDelTablePw == "" )
	{
		MessageBox( _T("PassWord를 입력하세요"), _T("다시"), MB_OK );
		return;
	}

	//비번 검사
	if( m_editDelTablePw != password )
	{
		MessageBox( _T("PassWord가 틀렸습니다."), _T("막지울 수 없습니다"), MB_OK );
		return;
	}

	int selectInt;
	CString tableName;

	selectInt = m_comboDelLogTable.GetCurSel();
	m_comboDelLogTable.GetLBText( selectInt, tableName );

	if( !GetDB.DisabledTable( (TCHAR*)tableName.GetString() ) )
		return;

	MessageBox( _T("table이 정상적으로 삭제 되었습니다"), _T("정보"), MB_OK );

	OnOK();
}

void CDelTable::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

