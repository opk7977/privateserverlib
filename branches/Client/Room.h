#pragma once



// CRoom 폼 뷰입니다.

class CRoom : public CFormView
{
	DECLARE_DYNCREATE(CRoom)

protected:
	CRoom();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CRoom();

public:
	enum { IDD = IDD_ROOM };
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
	CString m_csTitle;

	CString m_csPlayer01_id;
	CString m_csPlayer02_id;
	CString m_csPlayer03_id;
	CString m_csPlayer04_id;
	CString m_csPlayer05_id;
	CString m_csPlayer06_id;
	CString m_csPlayer07_id;
	CString m_csPlayer08_id;

	CString m_csPlayer01_Team;
	CString m_csPlayer02_Team;
	CString m_csPlayer03_Team;
};


