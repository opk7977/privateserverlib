#ifndef __WINDOW__
#define __WINDOW__

#include "SServerobj.h"

class SWindow : public SServerObj
{
private:
	WNDCLASSEX		m_wcex;

public:
	SWindow();
	virtual ~SWindow(void);

	BOOL CreateWindows( HINSTANCE hInst, TCHAR* className, TCHAR* appName, HWND &hWnd, int width, int height, int nCmdShow );
	BOOL ResizeWindows( HWND &hWnd, int width, int height );

	virtual LRESULT WindowMegProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	void SetMyWindowText( HWND &hWnd, TCHAR* title );
};

#endif