#pragma once



// CRoom �� ���Դϴ�.

class CRoom : public CFormView
{
	DECLARE_DYNCREATE(CRoom)

protected:
	CRoom();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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


