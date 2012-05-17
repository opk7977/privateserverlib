#pragma once
#include "afxwin.h"


// CDelTable 대화 상자입니다.

class CDelTable : public CDialog
{
	DECLARE_DYNAMIC(CDelTable)

public:
	CDelTable(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDelTable();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEL_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboDelLogTable;
	CString m_editDelTablePw;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
