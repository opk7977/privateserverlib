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
};


