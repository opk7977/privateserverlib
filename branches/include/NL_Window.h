#pragma once

#include "NL_Base.h"

//===============================================================
//  NAME	  : NWindow
//  COMMENTS  : 윈도우 클래스
//===============================================================
class NWindow
{
protected:
	//-------------------------------
	//	NWindow Data
	//-------------------------------
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	BOOL		m_bActive;
	DWORD		m_clntWidth;
	DWORD		m_clntHeight;

public:
	//-------------------------------
	//	Constructor & Destructor
	//-------------------------------
	NWindow(void);
	virtual ~NWindow(void);
	
	//-------------------------------
	//	Function
	//-------------------------------
	HWND		CreateWin(	HINSTANCE		hInst,
							const TCHAR*	titleName = _T("Neolux"),
							DWORD			width = 800,
							DWORD			height = 600,
							const TCHAR*	clsName = _T("Neolux App") );
	void		InitWin( HWND hWnd );
	LRESULT		WindowMsgProc(	HWND	hWnd,
								UINT	msg,
								WPARAM	wParam,
								LPARAM	lParam	);
	virtual	LRESULT EngineMsgProc(	HWND	hWnd,
									UINT	msg,
									WPARAM	wParam,
									LPARAM	lParam	)	{	return -1;	}
	virtual int Run(void);
	void	Close(void)		{	::CloseWindow( m_hWnd );	}
	void	Destroy(void)	{	::DestroyWindow( m_hWnd );	}

	//-------------------------------
	//	Getter
	//-------------------------------
	HINSTANCE	GetInstance(void)	const	{	return m_hInst;			}
	HWND		GetHwnd(void)		const	{	return m_hWnd;			}
	BOOL		GetActive(void)		const	{	return m_bActive;		}
	DWORD		GetClntWidth(void)	const	{	return m_clntWidth;		}
	DWORD		GetClntHeight(void)	const	{	return m_clntHeight;	}

	//-------------------------------
	//	Setter
	//-------------------------------
	void	SetActive(BOOL bActive)			{	m_bActive = bActive;	}
};