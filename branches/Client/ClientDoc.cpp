
// ClientDoc.cpp : CClientDoc Ŭ������ ����
//

#include "stdafx.h"
#include "Client.h"

#include "ClientDoc.h"
// 
// #include "MainFrm.h"
#include "CharMgr.h"
// 
// #include "Lobby.h"
// #include "Room.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientDoc

IMPLEMENT_DYNCREATE(CClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientDoc, CDocument)
END_MESSAGE_MAP()


// CClientDoc ����/�Ҹ�

CClientDoc::CClientDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
}

CClientDoc::~CClientDoc()
{
}

BOOL CClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	Init();

	return TRUE;
}




// CClientDoc serialization

void CClientDoc::Serialize(CArchive& ar)
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


// CClientDoc ����

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


// CClientDoc ���

//======================================
// ����� ���� �Լ�
//======================================
void CClientDoc::Init()
{
	isSceneState = 0;


	isConnectToLogin = FALSE;
	
	CheckID = 0;

	isCreateResult = 0;

	SessionID = 0;
	ZeroMemory( strId, 50 );

	isConnectToLobby = FALSE;

	myRoomNum = 0;
}

