#pragma once
#include "afxwin.h"


// CDelTable ��ȭ �����Դϴ�.

class CDelTable : public CDialog
{
	DECLARE_DYNAMIC(CDelTable)

public:
	CDelTable(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDelTable();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DEL_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboDelLogTable;
	CString m_editDelTablePw;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
