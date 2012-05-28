#include "NL_Input.h"

//===============================================================
//  NAME	  : NInput
//  COMMENTS  : 입력 클래스 구현
//===============================================================
//============================
//	Constructor & Destructor
//============================
NInput::NInput( void )
{
	Clear();
}

NInput::~NInput( void )
{
	Release();
}

void NInput::Clear( void )
{
	m_hWnd = NULL;
	
	m_cursorPos.x = 0;
	m_cursorPos.y = 0;
	m_oldCursorPos.x = 0;
	m_oldCursorPos.y = 0;
	m_cursorMove.x = 0;
	m_cursorMove.y = 0;
	m_wheelDelta = 0;
	memset( m_keyState, 0, sizeof(BYTE) * 256 );
	memset( m_fixedKeyState, 0, sizeof(BYTE) * 256 );
}

void NInput::Init(HWND hWnd)
{
	Release();

	m_hWnd = hWnd;

	::GetCursorPos(&m_oldCursorPos);
	::ScreenToClient(m_hWnd, &m_oldCursorPos);
}

void NInput::Frame(void)
{
	::GetCursorPos(&m_cursorPos);
	::ScreenToClient(m_hWnd, &m_cursorPos);

	m_cursorMove.x = m_cursorPos.x - m_oldCursorPos.x;
	m_cursorMove.y = m_cursorPos.y - m_oldCursorPos.y;
	m_oldCursorPos = m_cursorPos;

	if( GetKeyboardState(m_keyState) )
	{
		for(int i = 0; i < 256; ++i)
		{
			if( !(KS_CHECK & m_keyState[i]) )
			{
				if( KS_DOWN & m_fixedKeyState[i] )
					m_fixedKeyState[i] = KS_UPTOTAL;
				else
					m_fixedKeyState[i] = KS_UP;
			}
			else
			{
				if( KS_UP & m_fixedKeyState[i] )
					m_fixedKeyState[i] = KS_DOWNTOTAL;
				else
					m_fixedKeyState[i] = KS_DOWN;
			}
		}
	}
}

void NInput::Release( void )
{
	Clear();
}