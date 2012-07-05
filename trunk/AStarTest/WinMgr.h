#pragma once

#include "SWindow.h"

class MyData;

class WinMgr : public SWindow
{
private:
	MyData*		m_document;
public:
	WinMgr(void);
	~WinMgr(void);

	virtual LRESULT WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
};
