#pragma once

#include "SWindow.h"

class WinMgr : public SWindow
{
private:
	HINSTANCE m_hInst;

public:
	WinMgr(void);
	~WinMgr(void);

	void SetInstance( HINSTANCE& hInstance );
	virtual LRESULT WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};
