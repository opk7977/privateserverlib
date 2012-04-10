#pragma once

#include "SWindow.h"

class WinMgr : public SWindow
{
public:
	WinMgr(void);
	~WinMgr(void);

	virtual LRESULT WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

};
