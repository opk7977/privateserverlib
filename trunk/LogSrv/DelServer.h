#pragma once
#include "afxwin.h"


// CDelServer 대화 상자입니다.

class CDelServer : public CDialog
{
	DECLARE_DYNAMIC(CDelServer)

public:
	CDelServer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDelServer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEL_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_comboServer;
	CString		m_editDelPw;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
