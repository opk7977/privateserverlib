#pragma once


// CAddLogTable ��ȭ �����Դϴ�.

class CAddLogTable : public CDialog
{
	DECLARE_DYNAMIC(CAddLogTable)

public:
	CAddLogTable(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddLogTable();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ADD_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// ������ LogTable�� �̸��� �����մϴ�
	CString m_editTableName;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
