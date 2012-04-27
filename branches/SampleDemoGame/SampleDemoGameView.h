
// SampleDemoGameView.h : CSampleDemoGameView Ŭ������ �������̽�
//


#pragma once

class CSampleDemoGameView : public CView
{
protected: // serialization������ ��������ϴ�.
	CSampleDemoGameView();
	DECLARE_DYNCREATE(CSampleDemoGameView)

// Ư���Դϴ�.
public:
	CSampleDemoGameDoc* GetDocument() const;

// �۾��Դϴ�.
private:
	//--------------------------------------
	// ���� ���۸�
	//--------------------------------------
	CDC			memoryDC;
	CBitmap		memoryBit;
	CBitmap*	pOldBit;

	//timer���� ���ǵ� fram
	int			frame;
	

public:
	//--------------------------------------
	// ���� ���۸� ���۰� ��
	//--------------------------------------
	void BeginScene();
	void EndScene();

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CSampleDemoGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

#ifndef _DEBUG  // SampleDemoGameView.cpp�� ����� ����
inline CSampleDemoGameDoc* CSampleDemoGameView::GetDocument() const
   { return reinterpret_cast<CSampleDemoGameDoc*>(m_pDocument); }
#endif

