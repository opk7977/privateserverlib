
// LogSrvDoc.cpp : CLogSrvDoc Ŭ������ ����
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


// CLogSrvDoc ����/�Ҹ�

CLogSrvDoc::CLogSrvDoc()
: m_isDBConnect(FALSE)
, m_isSettingSrv(FALSE)
, m_isNetConnect(FALSE)
, m_view(NULL)
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	// DB���Ͽ� �����Ѵ�
	if( !m_database->Init( _T("LogDatabase.mdb" ) ) )
	{
		MessageBox( NULL, _T("db���ῡ ���� �Ͽ����ϴ�"), _T("�Ф�"), MB_OK );
		return FALSE;
	}

	//======================================
	// table�������� �ٽ� �Ϸ��� Ȱ������
	//======================================
// 	m_database->CreateServerTbl();
// 	m_database->CreateTableListTbl();
// 	m_database->CreateLogLvTbl();
	//======================================

	//////////////////////////////////////////////////////////////////////////
	//db�� ���������� ����Ǿ��ٴ� flagȤ�� ��ȣ�� �ش�
	//////////////////////////////////////////////////////////////////////////
	m_isDBConnect = TRUE;

	return TRUE;
}


// CLogSrvDoc serialization

void CLogSrvDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CLogSrvDoc ����

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


// CLogSrvDoc ���

//======================================
// Menu
//======================================
void CLogSrvDoc::OnMenuLogsrvstart()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( m_isNetConnect )
	{
		MessageBox( NULL, _T("�̹� ������ ���� �ֽ��ϴ�."), _T("No"), MB_OK );
		return;
	}
	if( !m_isSettingSrv )
	{
		MessageBox( NULL, _T("���� ������ �Ǿ� ���� �ʽ��ϴ�."), _T("�ϰ� ������"), MB_OK );
		return;
	}

	//���� ���� �� ����
	//������ 20����?..
	if( !m_network->Init( 20, m_srvPort ) )
	{
		MessageBox( NULL, _T("���� ������ ���� �߽��ϴ�."), _T("�Ӵ� ��"), MB_OK );
		return;
	}

	//���� ������ �Ϸ� ��
	MessageBox( NULL, _T("���� ������ �����߽��ϴ�."), _T("�� ��"), MB_OK );
	m_isNetConnect = TRUE;
}

void CLogSrvDoc::OnMenuLogsrvsetting()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( m_isNetConnect )
	{
		MessageBox( NULL, _T("�̹� ������ ���� �ֽ��ϴ�."), _T("No"), MB_OK );
		return;
	}

	CServerSetting *dlgSrvSetting = new CServerSetting;

	if( dlgSrvSetting->DoModal() == IDCANCEL )
		return;

	//��� ������ ������ flag�� �ٲ��ش�
	m_isSettingSrv = TRUE;
}

//======================================
// Add
//======================================
void CLogSrvDoc::OnAddTable()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//--------------------------------------
	// Log ���̺��� �ϳ� ����
	//--------------------------------------
	CAddLogTable *addLogTblDlg = new CAddLogTable;

	if( addLogTblDlg->DoModal() == IDOK )
	{
		// Log table�� �ϳ� �� �����Ǿ����̴�...
		// View���� Table Combobox�� ������Ʈ �� �ش�.
	}

	delete addLogTblDlg;

	if( m_view == NULL )
		return;

	m_view->SettingTableList();

}

void CLogSrvDoc::OnAddServer()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//--------------------------------------
	// ����ID�� �߰��մϴ�
	//--------------------------------------
	CAddServer *addSrvID = new CAddServer;

	if( addSrvID->DoModal() == IDOK )
	{
		// Server�� �ϳ� �� �߰��Ǿ����ϴ�.
		// view���� serverID combobox�� ������Ʈ �� �ּ���
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
// ������ Table�� ����°� �ƴϰ� Ȱ���� ���ִ� ����
//--------------------------------------
void CLogSrvDoc::OnDeleteTable()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//--------------------------------------
	// LogTable�� ���� �մϴ�
	//--------------------------------------
	CDelTable *delLogTable = new CDelTable;

	/*delLogTable->SettingInit();*/
	if( delLogTable->DoModal() == IDOK )
	{
		// Log table�� �ϳ� ���� �Ǿ����ϴ�
		// View���� Table Combobox�� ������Ʈ �� �ش�.
	}

	delete delLogTable;

	if( m_view == NULL )
		return;

	m_view->SettingTableList();
}