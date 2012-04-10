
// SampleDemoGameDoc.cpp : CSampleDemoGameDoc 클래스의 구현
//

#include "stdafx.h"
#include "SampleDemoGame.h"

#include "SampleDemoGameDoc.h"

#include "CharacterMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleDemoGameDoc

IMPLEMENT_DYNCREATE(CSampleDemoGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CSampleDemoGameDoc, CDocument)
END_MESSAGE_MAP()


// CSampleDemoGameDoc 생성/소멸

CSampleDemoGameDoc::CSampleDemoGameDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	isConnectToLoginSrv = FALSE;
	isLogin = FALSE;
	myindexID = -1;

	isConnectToGameSrv = FALSE;

}

CSampleDemoGameDoc::~CSampleDemoGameDoc()
{
}

BOOL CSampleDemoGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CSampleDemoGameDoc serialization

void CSampleDemoGameDoc::Serialize(CArchive& ar)
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


// CSampleDemoGameDoc 진단

#ifdef _DEBUG
void CSampleDemoGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSampleDemoGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

#endif //_DEBUG


// CSampleDemoGameDoc 명령


//======================================
// 사용자 정의 함수
//======================================
void CSampleDemoGameDoc::SetConnect( BOOL conn )
{
	isConnectToLoginSrv = conn;
}

BOOL CSampleDemoGameDoc::GetConnect()
{
	return isConnectToLoginSrv;
}

void CSampleDemoGameDoc::SetLogin( BOOL login )
{
	isLogin = login;
}

BOOL CSampleDemoGameDoc::GetLogin()
{
	return isLogin;
}

void CSampleDemoGameDoc::SetIndexId( int index )
{
	//캐릭터에 자신의 캐릭터 아이티를 설정해 주고
	GetCharMgr.SetHeroId( index );

	myindexID = index;
}

int CSampleDemoGameDoc::GetIndexId()
{
	return myindexID;
}

void CSampleDemoGameDoc::SetConnectToG( BOOL conn )
{
	isConnectToGameSrv = conn;
}

BOOL CSampleDemoGameDoc::GetConnectToG()
{
	return isConnectToGameSrv;
}
