#pragma once

#include "SWindow.h"

class winMgr : public SWindow
{
public:
	winMgr(void);
	~winMgr(void);

	virtual LRESULT WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};
