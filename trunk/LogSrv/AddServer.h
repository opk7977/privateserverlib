#pragma once


// CAddServer ��ȭ �����Դϴ�.

class CAddServer : public CDialog
{
	DECLARE_DYNAMIC(CAddServer)

public:
	CAddServer(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddServer();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ADD_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString	m_editServerName;
	int		m_iServerID;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
