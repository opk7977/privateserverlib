#include "StdAfx.h"
#include "Scheduler.h"

#include "PacketQueue.h"

#include "ClientDoc.h"
#include "ClientView.h"


CScheduler::CScheduler(void) : pDoc(NULL)
{
}

CScheduler::~CScheduler(void)
{
}

void CScheduler::Run()
{
	int count = GetPacketQ.GetDataCount();

	//30보다 크면 그냥 30으로
	if( count > 30 )
		count = 30;

	char* tmpBuf;

	for( int i=0; i<count; ++i )
	{
		tmpBuf = GetPacketQ.GetPacket();
		if( tmpBuf == NULL )
			return;

		m_packet.CopyToPacket( tmpBuf, PACKETDATA_SIZE );

		PacketParsing();
	}
}

void CScheduler::PacketParsing()
{
	switch( m_packet.GetID() )
	{
	case SC_LOGIN_CONNECT_OK:
		//RecvConnectOK();
		break;
	case SC_LOGIN_CHECK_ID_RESULT:
		//RecvCheckID();
		break;
	case SC_LOGIN_CREATE_RESULT:
		//RecvCreateResult();
		break;
	case SC_LOGIN_LOGIN_RESULT:
		//RecvLoginResult();
		break;
	}
}




/*
CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
CSampleDemoGameView* pView = (CSampleDemoGameView*) pChild->GetActiveView();
CSampleDemoGameDoc* pDoc = pView->GetDocument();

if (!pDoc)
return;
*/

//패킷을 처리하는 함수
//SC_LOGIN_CONNECT_OK
void CScheduler::RecvConnectOK()
{
	if( pDoc == NULL )
	{
		//없으면 넣어 놓고 시작
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
		CClientView* pView = (CClientView*) pChild->GetActiveView();
		pDoc = pView->GetDocument();
	}

	if (!pDoc)
		return;
	
	//CClientView* pView = ((CClientView*)AfxGetMainWnd())->GetAc

	//::SetWindowText( pDoc->GetV

	//로그인 서버에 연결되었다는 것을 확인할 수 있게 윈도우 텍스트를 바꿔야 하나?..

	pDoc->isConnectToLoginSrv = TRUE;
}

//SC_LOGIN_CHECK_ID_RESULT
void CScheduler::RecvCheckID()
{
	if( pDoc == NULL )
	{
		//없으면 넣어 놓고 시작
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
		CClientView* pView = (CClientView*) pChild->GetActiveView();
		pDoc = pView->GetDocument();
	}

	if (!pDoc)
		return;

	int result;
	m_packet >> result;

	pDoc->CheckID = result;
	pDoc->isCheckID = TRUE;
}

//SC_LOGIN_CREATE_RESULT
void CScheduler::RecvCreateResult()
{
	if( pDoc == NULL )
	{
		//없으면 넣어 놓고 시작
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
		CClientView* pView = (CClientView*) pChild->GetActiveView();
		pDoc = pView->GetDocument();
	}

	if (!pDoc)
		return;

	int result;
	m_packet >> result;

	pDoc->isCreateResult = result;
	pDoc->RecvCreateResult = TRUE;
}

//SC_LOGIN_LOGIN_RESULT
void CScheduler::RecvLoginResult()
{
	if( pDoc == NULL )
	{
		//없으면 넣어 놓고 시작
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
		CClientView* pView = (CClientView*) pChild->GetActiveView();
		pDoc = pView->GetDocument();
	}

	if (!pDoc)
		return;

	int result;
	m_packet >> result;


}



