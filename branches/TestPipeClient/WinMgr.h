#pragma once

#include "SWindow.h"

class WinMgr : public SWindow
{
private:
	HANDLE m_hPipe;

public:
	WinMgr(void);
	~WinMgr(void);

	LRESULT WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void Release();
};
