#pragma once


// CJoin ��ȭ �����Դϴ�.

class CJoin : public CDialog
{
	DECLARE_DYNAMIC(CJoin)

public:
	CJoin(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJoin();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_JOIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
