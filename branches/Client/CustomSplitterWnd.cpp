#include "StdAfx.h"
#include "CustomSplitterWnd.h"

BEGIN_MESSAGE_MAP(CCustomSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CCustomSplitterWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCustomSplitterWnd::CCustomSplitterWnd(void)
{
	m_cxSplitter = m_cySplitter = 2;
	//m_cxBorderShare = m_cyBorderShare = 0;
	m_cxSplitterGap = m_cySplitterGap = 1;
	m_cxBorder = m_cyBorder = 2;
}

CCustomSplitterWnd::~CCustomSplitterWnd(void)
{
}

void CCustomSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CCustomSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
}
