#pragma once
#include "afxwin.h"


// CDelServer ��ȭ �����Դϴ�.

class CDelServer : public CDialog
{
	DECLARE_DYNAMIC(CDelServer)

public:
	CDelServer(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDelServer();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DEL_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_comboServer;
	CString		m_editDelPw;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
