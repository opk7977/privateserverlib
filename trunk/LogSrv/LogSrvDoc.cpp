
// LogSrvDoc.cpp : CLogSrvDoc Ŭ������ ����
//

#include "stdafx.h"
#include "LogSrv.h"

#include "LogSrvDoc.h"

#include "LogDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogSrvDoc

IMPLEMENT_DYNCREATE(CLogSrvDoc, CDocument)

BEGIN_MESSAGE_MAP(CLogSrvDoc, CDocument)
END_MESSAGE_MAP()


// CLogSrvDoc ����/�Ҹ�

CLogSrvDoc::CLogSrvDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	m_database = &GetDB;
}

CLogSrvDoc::~CLogSrvDoc()
{
}

BOOL CLogSrvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	// DB���Ͽ� �����Ѵ�
	if( !m_database->Init( _T("LogDatabase.mdb" ) ) )
		MessageBox( NULL, _T("db���ῡ ���� �Ͽ����ϴ�"), _T("�Ф�"), MB_OK );

	// DB�� �������̺��� �����
	m_database->CreateServerTbl();

	// DB�� table list ���̺��� �����

	// DB������ ���´�.
	m_database->Release();

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
