
// LogSrvView.h : CLogSrvView 클래스의 인터페이스
//


#pragma once
#include "afxcmn.h"
#include "afxwin.h"


class CLogSrvView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CLogSrvView();
	DECLARE_DYNCREATE(CLogSrvView)

public:
	enum{ IDD = IDD_LOGSRV_FORM };

// 특성입니다.
public:
	CLogSrvDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CLogSrvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_TableCombo;
	CComboBox	m_ServerCombo;
	CComboBox	m_LogLevelCombo;
	CListCtrl	m_LogList;
	CButton		m_SendQuery;
};

#ifndef _DEBUG  // LogSrvView.cpp의 디버그 버전
inline CLogSrvDoc* CLogSrvView::GetDocument() const
   { return reinterpret_cast<CLogSrvDoc*>(m_pDocument); }
#endif

