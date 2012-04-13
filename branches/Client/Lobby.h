#pragma once



// CLobby 폼 뷰입니다.

class CLobby : public CFormView
{
	DECLARE_DYNCREATE(CLobby)

protected:
	CLobby();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNetConn();
	afx_msg void OnNetLogin();
};


