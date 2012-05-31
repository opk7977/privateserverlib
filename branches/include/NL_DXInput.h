#pragma once

#include "NL_Base.h"
#include "NL_Singleton.h"

#define DIRECTINPUT_VERSION	0x0800

#include <dinput.h>

//===============================================================
//  NAME	  : NDXInput
//  COMMENTS  : DX 입력 클래스 선언
//===============================================================
class NDXInput : public NSingleton< NDXInput >
{
	DECL_FRIEND( NSingleton< NDXInput >)

	//============================
	//	NInput Data
	//============================
	HWND					m_hWnd;
	LPDIRECTINPUT8			m_pDInput;
	LPDIRECTINPUTDEVICE8	m_pMouse;
	DIMOUSESTATE			m_mouseState;

public:
	//============================
	//	Constructor & Destructor
	//============================
	NDXInput(void);
	~NDXInput(void);

	void Init(HWND hWnd);
	void Frame(void);
	void Reset(void);
	void Remove(void);
	void Release(void);

	int		GetCursorMoveX() const	{	return m_mouseState.lX;						}
	int		GetCursorMoveY() const	{	return m_mouseState.lY;						}
	int		GetWheel()		 const	{	return m_mouseState.lZ;						}
	bool	IsMoved()		 const	{	return m_mouseState.lX || m_mouseState.lY;	}
	HWND	GetHwnd()		 const	{	return m_hWnd;								}
};

#define I_NDXInput	NDXInput::GetInstance()