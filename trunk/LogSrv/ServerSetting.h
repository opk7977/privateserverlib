#pragma once
#include "afxwin.h"


// CServerSetting ��ȭ �����Դϴ�.

class CServerSetting : public CDialog
{
	DECLARE_DYNAMIC(CServerSetting)

public:
	CServerSetting(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CServerSetting();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SRVSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int			m_iIp1;
	int			m_iIp2;
	int			m_iIp3;
	int			m_iIp4;
	int			m_srvPort;
	CComboBox	m_comboUseTable;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
