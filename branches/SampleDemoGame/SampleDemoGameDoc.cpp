
// SampleDemoGameDoc.cpp : CSampleDemoGameDoc Ŭ������ ����
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


// CSampleDemoGameDoc ����/�Ҹ�

CSampleDemoGameDoc::CSampleDemoGameDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

// CSampleDemoGameDoc serialization

void CSampleDemoGameDoc::Serialize(CArchive& ar)
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


// CSampleDemoGameDoc ����

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


// CSampleDemoGameDoc ���


//======================================
// ����� ���� �Լ�
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
	//ĳ���Ϳ� �ڽ��� ĳ���� ����Ƽ�� ������ �ְ�
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
