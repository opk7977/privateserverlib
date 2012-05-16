
// LogSrvDoc.cpp : CLogSrvDoc 클래스의 구현
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


// CLogSrvDoc 생성/소멸

CLogSrvDoc::CLogSrvDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_database = &GetDB;
}

CLogSrvDoc::~CLogSrvDoc()
{
}

BOOL CLogSrvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	// DB파일에 연결한다
	if( !m_database->Init( _T("LogDatabase.mdb" ) ) )
		MessageBox( NULL, _T("db연결에 실패 하였습니다"), _T("ㅠㅠ"), MB_OK );

	// DB에 서버테이블을 만든다
	m_database->CreateServerTbl();

	// DB에 table list 테이블을 만든다

	// DB연결을 끊는다.
	m_database->Release();

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
