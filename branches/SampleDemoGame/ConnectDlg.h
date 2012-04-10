#pragma once
#include "afxwin.h"
#include "STime.h"

// CConnectDlg 대화 상자입니다.

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConnectDlg();

private:
	STime		time;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONNECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
