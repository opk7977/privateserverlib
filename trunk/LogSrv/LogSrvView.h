
// LogSrvView.h : CLogSrvView Ŭ������ �������̽�
//


#pragma once
#include "afxcmn.h"
#include "afxwin.h"


class CLogSrvView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CLogSrvView();
	DECLARE_DYNCREATE(CLogSrvView)

public:
	enum{ IDD = IDD_LOGSRV_FORM };

// Ư���Դϴ�.
public:
	CLogSrvDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CLogSrvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_TableCombo;
	CComboBox	m_ServerCombo;
	CComboBox	m_LogLevelCombo;
	CListCtrl	m_LogList;
	CButton		m_SendQuery;
};

#ifndef _DEBUG  // LogSrvView.cpp�� ����� ����
inline CLogSrvDoc* CLogSrvView::GetDocument() const
   { return reinterpret_cast<CLogSrvDoc*>(m_pDocument); }
#endif

