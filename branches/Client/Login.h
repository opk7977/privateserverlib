#pragma once


// CLogin ��ȭ �����Դϴ�.

class CLogin : public CDialog
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLogin();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
