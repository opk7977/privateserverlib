
// ClientDoc.cpp : CClientDoc 클래스의 구현
//

#include "stdafx.h"
#include "Client.h"

#include "ClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDoc

IMPLEMENT_DYNCREATE(CClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientDoc, CDocument)
END_MESSAGE_MAP()


// CClientDoc 생성/소멸

CClientDoc::CClientDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	Init();
}

CClientDoc::~CClientDoc()
{
}

BOOL CClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CClientDoc serialization

void CClientDoc::Serialize(CArchive& ar)
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


// CClientDoc 진단

#ifdef _DEBUG
void CClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CClientDoc 명령

//======================================
// 사용자 정의 함수
//======================================
void CClientDoc::Init()
{
	isConnectToLogin = FALSE;
	isCheckID = FALSE;
	CheckID = 0;
	isReturnLogin = FALSE;
	isConnectToLobby = FALSE;

	SessionID = 0;
	strId = "";
}

