#pragma once


// CJoin 대화 상자입니다.

class CJoin : public CDialog
{
	DECLARE_DYNAMIC(CJoin)

public:
	CJoin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CJoin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_JOIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_csId;
	CString m_csPw;
	CString m_csMail;
	afx_msg void OnBnClickedCheckid();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
