#include "StdAfx.h"
#include "Scheduler.h"

#include "PacketQueue.h"

#include "ClientDoc.h"
#include "ClientView.h"

#include "NetProtocol.h"

<<<<<<< .mine

unsigned int __stdcall _Schdul( void* pArg )
{
	return ((CScheduler*)pArg)->Run();
}

CScheduler::CScheduler(void)
=======
CScheduler::CScheduler(void) : pDoc(NULL)
>>>>>>> .r10
{
}

CScheduler::~CScheduler(void)
{
}

void CScheduler::Init()
{
<<<<<<< .mine
	m_startWork = ::CreateEvent( NULL, TRUE, FALSE, NULL); 
=======
	int count = GetPacketQ.GetDataCount();
>>>>>>> .r10

<<<<<<< .mine
	unsigned int iThread;
	m_schThread = (HANDLE)_beginthreadex( NULL, 0, &::_Schdul, (void*)this, 0, &iThread );
	if( !m_schThread )
		return;

	m_timer.Init();
	m_frame = 0.f;

	SetEvent( m_startWork );
=======
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
>>>>>>> .r10
}
<<<<<<< .mine

unsigned int CScheduler::Run()
{
	while(1)
	{
		Sleep(10);

		WaitForSingleObject( m_startWork, INFINITE );

		m_timer.ProcessTime();

		m_frame += m_timer.GetElapsedTime();

		if( m_frame >= 1.f )
		{
			m_frame -= 1.f;

			PacketProcess();
		}
	}
}

void CScheduler::PacketProcess()
{
	int count = GetPacketQ.GetDataCount();

	if( count <= 0 )
		return;

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

		//패킷 처리
		PacketParsing();

		//처리가 끝나고 위치를 이동한다.
		GetPacketQ.MoveReadPos();
	}
}

void CScheduler::PacketParsing()
{
	switch( m_packet.GetID() )
	{
	case SC_LOGIN_CONNECT_OK:
		RecvConnectOK();
		break;
	case SC_LOGIN_CHECK_ID_RESULT:
		RecvCheckID();
		break;
	case SC_LOGIN_CREATE_RESULT:
		RecvCreateResult();
		break;
	case SC_LOGIN_LOGIN_RESULT:
		RecvLoginResult();
		break;
	}
}

//패킷을 처리하는 함수
//SC_LOGIN_CONNECT_OK
void CScheduler::RecvConnectOK()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	MessageBox( NULL, _T("로그인 서버와 연결되었습니다."), _T("Info"), MB_OK );

	pDoc->isConnectToLogin = TRUE;
}

//SC_LOGIN_CHECK_ID_RESULT
void CScheduler::RecvCheckID()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	int result;
	m_packet >> result;

	if( result < 0 )
	{
		MessageBox( NULL, _T("이미 사용중인 id입니다."), _T("다시!"), MB_OK );
	}
	else
	{
		MessageBox( NULL, _T("사용할 수 있는 id입니다."), _T("Good"), MB_OK );
	}

	pDoc->CheckID = result;
	pDoc->isCheckID = TRUE;
}

//SC_LOGIN_CREATE_RESULT
void CScheduler::RecvCreateResult()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	int result;
	m_packet >> result;

	if( result < 0 )
	{
		MessageBox( NULL, _T("계정생성 실패."), _T("다시!"), MB_OK );
	}
	else
	{
		MessageBox( NULL, _T("계정이 생성되었습니다."), _T("Good"), MB_OK );
	}

	pDoc->isCreateResult = result;
	pDoc->iRecvCreateResult = TRUE;
}

//SC_LOGIN_LOGIN_RESULT
void CScheduler::RecvLoginResult()
{
	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if (!pDoc)
		return;

	int result;
	m_packet >> result;

	if( result < 0 )
	{
		MessageBox( NULL, _T("존재하지 않는 id입니다."), _T("error"), MB_OK | MB_ICONERROR );
	}
	if( result == 0 )
	{
		MessageBox( NULL, _T("password가 틀립니다."), _T("error"), MB_OK | MB_ICONERROR );
	}
	else
	{
		MessageBox( NULL, _T("로그인에 성공~"), _T("good"), MB_OK );
	}
	
	pDoc->SessionID = result;
	pDoc->isReturnLogin = TRUE;
}



=======

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



>>>>>>> .r10
