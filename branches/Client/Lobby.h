#pragma once



// CLobby �� ���Դϴ�.

class CLobby : public CFormView
{
	DECLARE_DYNCREATE(CLobby)

protected:
	CLobby();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CLobby();

public:
	enum { IDD = IDD_LOBBY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNetConn();
	afx_msg void OnNetLogin();
};


