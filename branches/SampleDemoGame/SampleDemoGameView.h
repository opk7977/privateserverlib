
// SampleDemoGameView.h : CSampleDemoGameView 클래스의 인터페이스
//


#pragma once

class CSampleDemoGameView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSampleDemoGameView();
	DECLARE_DYNCREATE(CSampleDemoGameView)

// 특성입니다.
public:
	CSampleDemoGameDoc* GetDocument() const;

// 작업입니다.
private:
	//--------------------------------------
	// 더블 버퍼링
	//--------------------------------------
	CDC			memoryDC;
	CBitmap		memoryBit;
	CBitmap*	pOldBit;

	//timer에서 갱실될 fram
	int			frame;
	

public:
	//--------------------------------------
	// 더블 버퍼링 시작과 끝
	//--------------------------------------
	void BeginScene();
	void EndScene();

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CSampleDemoGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConnConnect();
	afx_msg void OnConnDisconn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // SampleDemoGameView.cpp의 디버그 버전
inline CSampleDemoGameDoc* CSampleDemoGameView::GetDocument() const
   { return reinterpret_cast<CSampleDemoGameDoc*>(m_pDocument); }
#endif

