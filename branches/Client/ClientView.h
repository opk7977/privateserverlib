
// ClientView.h : CClientView Ŭ������ �������̽�
//


#pragma once


class CClientView : public CView
{
protected: // serialization������ ��������ϴ�.
	CClientView();
	DECLARE_DYNCREATE(CClientView)

// Ư���Դϴ�.
public:
	CClientDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNetConn();
	afx_msg void OnNetLogin();
};

#ifndef _DEBUG  // ClientView.cpp�� ����� ����
inline CClientDoc* CClientView::GetDocument() const
   { return reinterpret_cast<CClientDoc*>(m_pDocument); }
#endif

