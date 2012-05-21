
// LogSrvDoc.cpp : CLogSrvDoc 클래스의 구현
//

#include "stdafx.h"
#include "LogSrv.h"

#include "LogSrvDoc.h"
#include "LogSrvView.h"

#include "LogDataBase.h"
#include "Network.h"


#include "ServerSetting.h"

#include "AddLogTable.h"
#include "AddServer.h"

#include "DelTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogSrvDoc

IMPLEMENT_DYNCREATE(CLogSrvDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogSrvDoc, CDocument)
	ON_COMMAND(ID_ADD_TABLE, &CLogSrvDoc::OnAddTable)
	ON_COMMAND(ID_ADD_SERVER, &CLogSrvDoc::OnAddServer)
	ON_COMMAND(ID_DELETE_TABLE, &CLogSrvDoc::OnDeleteTable)
	ON_COMMAND(ID_MENU_LOGSRVSTART, &CLogSrvDoc::OnMenuLogsrvstart)
	ON_COMMAND(ID_MENU_LOGSRVSETTING, &CLogSrvDoc::OnMenuLogsrvsetting)
END_MESSAGE_MAP()


// CLogSrvDoc 생성/소멸

CLogSrvDoc::CLogSrvDoc()
: m_isDBConnect(FALSE)
, m_isSettingSrv(FALSE)
, m_isNetConnect(FALSE)
, m_view(NULL)
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_database	= &GetDB;
	m_network	= &GetNetwork;
}

CLogSrvDoc::~CLogSrvDoc()
{
	m_database->Release();
}

BOOL CLogSrvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	// DB파일에 연결한다
	if( !m_database->Init( _T("LogDatabase.mdb" ) ) )
	{
		MessageBox( NULL, _T("db연결에 실패 하였습니다"), _T("ㅠㅠ"), MB_OK );
		return FALSE;
	}

	//======================================
	// table설정부터 다시 하려면 활성하자
	//======================================
// 	m_database->CreateServerTbl();
// 	m_database->CreateTableListTbl();
// 	m_database->CreateLogLvTbl();
	//======================================

	//////////////////////////////////////////////////////////////////////////
	//db에 정상적으로 연결되었다는 flag혹은 신호를 준다
	//////////////////////////////////////////////////////////////////////////
	m_isDBConnect = TRUE;

	return TRUE;
}


// CLogSrvDoc serialization

void CLogSrvDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CLogSrvDoc 진단

#ifdef _DEBUG
void CLogSrvDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLogSrvDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLogSrvDoc 명령

//======================================
// Menu
//======================================
void CLogSrvDoc::OnMenuLogsrvstart()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( m_isNetConnect )
	{
		MessageBox( NULL, _T("이미 서버가 열려 있습니다."), _T("No"), MB_OK );
		return;
	}
	if( !m_isSettingSrv )
	{
		MessageBox( NULL, _T("서버 설정이 되어 있지 않습니다."), _T("하고 오세요"), MB_OK );
		return;
	}

	//서버 설정 해 놓기
	//세션은 20정도?..
	if( !m_network->Init( 20, m_srvPort ) )
	{
		MessageBox( NULL, _T("서버 설정에 실패 했습니다."), _T("왓더 헬"), MB_OK );
		return;
	}

	//서버 연결이 완료 된
	MessageBox( NULL, _T("서버 설정에 성공했습니다."), _T("굳 좝"), MB_OK );
	m_isNetConnect = TRUE;
}

void CLogSrvDoc::OnMenuLogsrvsetting()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( m_isNetConnect )
	{
		MessageBox( NULL, _T("이미 서버가 열려 있습니다."), _T("No"), MB_OK );
		return;
	}

	CServerSetting *dlgSrvSetting = new CServerSetting;

	if( dlgSrvSetting->DoModal() == IDCANCEL )
		return;

	//모든 셋팅이 끝나면 flag를 바꿔준다
	m_isSettingSrv = TRUE;
}

//======================================
// Add
//======================================
void CLogSrvDoc::OnAddTable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//--------------------------------------
	// Log 테이블을 하나 생성
	//--------------------------------------
	CAddLogTable *addLogTblDlg = new CAddLogTable;

	if( addLogTblDlg->DoModal() == IDOK )
	{
		// Log table이 하나 더 생성되었습미다...
		// View에서 Table Combobox를 업데이트 해 준다.
	}

	delete addLogTblDlg;

	if( m_view == NULL )
		return;

	m_view->SettingTableList();

}

void CLogSrvDoc::OnAddServer()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//--------------------------------------
	// 서버ID를 추가합니다
	//--------------------------------------
	CAddServer *addSrvID = new CAddServer;

	if( addSrvID->DoModal() == IDOK )
	{
		// Server가 하나 더 추가되었습니다.
		// view에서 serverID combobox를 업데이트 해 주세요
	}

	delete addSrvID;

	if( m_view == NULL )
		return;

	m_view->SettingServerList();
}

//======================================
// Delete
//======================================
//--------------------------------------
// 정말로 Table을 지우는게 아니고 활성을 꺼주는 것임
//--------------------------------------
void CLogSrvDoc::OnDeleteTable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//--------------------------------------
	// LogTable을 삭제 합니다
	//--------------------------------------
	CDelTable *delLogTable = new CDelTable;

	/*delLogTable->SettingInit();*/
	if( delLogTable->DoModal() == IDOK )
	{
		// Log table이 하나 삭제 되었습니다
		// View에서 Table Combobox를 업데이트 해 준다.
	}

	delete delLogTable;

	if( m_view == NULL )
		return;

	m_view->SettingTableList();
}