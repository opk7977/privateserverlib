#include "StdAfx.h"
#include "CharMgr.h"

#include "ClientDoc.h"
#include "ClientView.h"


CharMgr::CharMgr(void) : m_playerCount(0)
					   , m_itMe(NULL)
{
}

CharMgr::~CharMgr(void)
{
}

void CharMgr::AddChar( int sessionId, CString id )
{
	Character* tmpChar = new Character;

	tmpChar->SetSessionID( sessionId );
	tmpChar->SetID( id );
	tmpChar->SetRoomNo( 0 );

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd*) pFrame->GetActiveFrame();
	CClientView* pView = (CClientView*) pChild->GetActiveView();
	CClientDoc* pDoc = pView->GetDocument();

	if( sessionId == pDoc->SessionID )
		m_itMe = tmpChar;

	m_charList.push_back( tmpChar );
}

void CharMgr::DelChar( int sessionId )
{
	Character* tmpChar = FindChar( sessionId );

	if( tmpChar == NULL )
		return;

	m_charList.remove( tmpChar );
}

Character* CharMgr::FindChar( int sessionId )
{
	if( m_charList.empty() )
		return NULL;

	std::list<Character*>::iterator iter;
	iter = m_charList.begin();
	for( ; iter != m_charList.end(); ++iter )
	{
		if( (*iter)->GetSessionID() == sessionId )
			return (*iter);
	}

	return NULL;
}
