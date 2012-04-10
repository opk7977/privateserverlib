#ifndef __WINDOWS_MANAGER__
#define __WINDOWS_MANAGER__

class CWinMgr
{
private:
	WNDCLASSEX	wcex;

public:
	CWinMgr(void);
	~CWinMgr(void);

	BOOL CreateWindows( HINSTANCE hInst, int nCmdShow );
};


#endif


