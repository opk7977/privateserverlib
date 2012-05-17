#pragma once
#include "afxwin.h"


// CServerSetting 대화 상자입니다.

class CServerSetting : public CDialog
{
	DECLARE_DYNAMIC(CServerSetting)

public:
	CServerSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CServerSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SRVSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
