#pragma once


// CAddServer 대화 상자입니다.

class CAddServer : public CDialog
{
	DECLARE_DYNAMIC(CAddServer)

public:
	CAddServer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddServer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADD_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString	m_editServerName;
	int		m_iServerID;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
