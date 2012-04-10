#pragma once
#include "afxwin.h"
#include "STime.h"

// CConnectDlg ��ȭ �����Դϴ�.

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConnectDlg();

private:
	STime		time;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CONNECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_iIp1;
	int m_iIp2;
	int m_iIp3;
	int m_iIp4;
	int m_iPort;
	CString m_strId;
	CString m_strPw;
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
