#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"

//===============================================================
//  NAME	  : NInput
//  COMMENTS  : 입력 클래스 선언
//===============================================================
class NInput : public NSingleton< NInput >
{
	DECL_FRIEND( NSingleton< NInput >)

	//============================
	//	NInput Data
	//============================
	enum NI_KEYSTATE
	{
		KS_UP		 = 0x01,
		KS_RELEASED  = 0x02,
		KS_UPTOTAL	 = 0x03,
		KS_DOWN		 = 0x04,
		KS_PRESSED   = 0x08,
		KS_DOWNTOTAL = 0x0C,
		KS_CHECK	 = 0x80
	};

	HWND	m_hWnd;
	POINT	m_cursorPos;
	POINT	m_oldCursorPos;
	POINT	m_cursorMove;
	short	m_wheelDelta;
	BYTE	m_keyState[256];
	BYTE	m_fixedKeyState[256];

public:
	//============================
	//	Constructor & Destructor
	//============================
	NInput(void);
	~NInput(void);

	void Init(HWND hWnd);
	void Frame(void);
	void Release(void);
	void Clear(void);

	bool	IsKeydownFirst(ULONG vKey)	const 	{	return m_fixedKeyState[vKey] & KS_PRESSED ?		true : false;	}
	bool	IsKeydown(ULONG vKey)		const	{	return m_fixedKeyState[vKey] & KS_DOWN ?		true : false;	}
	bool	IsKeyupFirst(ULONG vKey)	const	{	return m_fixedKeyState[vKey] & KS_RELEASED ?	true : false;	}
	bool	IsKeyup(ULONG vKey)			const	{	return m_fixedKeyState[vKey] & KS_UP ?			true : false;	}

	int		GetCursorX()				const	{	return m_cursorPos.x;	}
	int		GetCursorY()				const	{	return m_cursorPos.y;	}
	POINT	GetCursorPos()				const	{	return m_cursorPos;		}
	int		GetCursorMoveX()			const	{	return m_cursorMove.x;	}
	int		GetCursorMoveY()			const	{	return m_cursorMove.y;	}
	POINT	GetCursorMove()				const	{	return m_cursorMove;	}
	bool	IsMoved()					const	{	return m_cursorMove.x || m_cursorMove.y;	}

	void	SetMouseWheel(short delta)			{	m_wheelDelta = delta;	}
	void	ResetMouseWheel()					{	m_wheelDelta = 0;		}
	short	GetWheel()					const	{	return m_wheelDelta;	}
	void	SetOldCursor(POINT& pt)				{	m_oldCursorPos = pt;	}
};

#define I_NInput	NInput::GetInstance()