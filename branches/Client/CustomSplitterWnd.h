#pragma once

class CCustomSplitterWnd : public CSplitterWnd
{
// Construction
public:
	CCustomSplitterWnd();

// Implementation
public:
	virtual ~CCustomSplitterWnd();

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};