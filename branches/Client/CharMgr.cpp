#include "StdAfx.h"
#include "CharMgr.h"

#include "ClientDoc.h"
#include "ClientView.h"

#include "MainFrm.h"

#include "Lobby.h"


CharMgr::CharMgr(void) : m_playerCount(0)
					   , m_itMe(NULL)
					   , m_pLobby(NULL)
{
}

CharMgr::~CharMgr(void)
{
	Release();
}

void CharMgr::AddChar( int sessionId, TCHAR* id )
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

	if( m_pLobby == NULL )
	{
		MessageBox( NULL, _T("CharMgr::AddChar()_m_pLobby가 없음"), _T("error"), MB_OK );
		return;
	}

	m_pLobby->AddPlayerInList( id );
	++m_playerCount;
}

void CharMgr::DelChar( int sessionId )
{
	Character* tmpChar = FindChar( sessionId );

	if( tmpChar == NULL )
		return;

	if( m_pLobby == NULL )
	{
		MessageBox( NULL, _T("CharMgr::AddChar()_m_pLobby가 없음"), _T("error"), MB_OK );
		return;
	}
	m_pLobby->DelPlayerInList( tmpChar->GetID() );

	delete tmpChar;
	m_charList.remove( tmpChar );
	--m_playerCount;
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

void CharMgr::ClearCharacter()
{
	if( m_charList.empty() )
		return;

	std::list<Character*>::iterator iter, preIter;
	iter = m_charList.begin();
	for( int i=0; i<m_playerCount; ++i )
	{
		preIter = iter++;

		m_pLobby->DelPlayerInList( (*preIter)->GetID() );

		delete *preIter;
		m_charList.erase( preIter );
	}
	m_itMe = 0;
}

void CharMgr::Release()
{
	if( m_charList.empty() )
		return;

	std::list<Character*>::iterator iter, preIter;
	iter = m_charList.begin();
	for( int i=0; i<m_playerCount; ++i )
	{
		preIter = iter++;

		delete *preIter;
		*preIter = 0;
	}
	m_charList.clear();
}
