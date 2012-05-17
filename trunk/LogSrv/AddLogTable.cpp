// AddLogTable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogSrv.h"
#include "AddLogTable.h"

#include "LogDataBase.h"


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

	//--------------------------------------
	// udata를 update받아 온다
	//--------------------------------------
	UpdateData( TRUE );

	//--------------------------------------
	// 빈공간이면 안되
	//--------------------------------------
	if( m_editTableName == "" )
	{
		MessageBox( _T("빈문자열로는 table을 만들 수 없습니다."), _T("다시"), MB_OK );
		return;
	}

	if( !GetDB.CreateLogTable( (TCHAR*)m_editTableName.GetString() ) )
	{
		MessageBox( _T("테이블 생성에 실패하였습니다. 문자열 혹은 DB를 다시 확인해 주세요."), _T("죄송"), MB_OK );
		return;
	}

	MessageBox( _T("테이블 생성 완료."), _T("성공"), MB_OK );

	OnOK();
}

void CAddLogTable::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
