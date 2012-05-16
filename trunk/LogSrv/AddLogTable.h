#pragma once


// CAddLogTable 대화 상자입니다.

class CAddLogTable : public CDialog
{
	DECLARE_DYNAMIC(CAddLogTable)

public:
	CAddLogTable(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddLogTable();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADD_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 생성될 LogTable의 이름을 설정합니다
	CString m_editTableName;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
