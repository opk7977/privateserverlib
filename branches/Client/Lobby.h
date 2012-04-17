#pragma once
#include "resource.h"
#include "afxwin.h"



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
	void AddPlayerInList( TCHAR* player );
	void DelPlayerInList( TCHAR* player );

public:
	virtual void OnInitialUpdate();
	afx_msg void OnNetConn();
	afx_msg void OnNetLogin();

	CStatic m_RoomTitle[4];
	CStatic m_RoomPlayerCount[4];
	CButton m_btnNewRoom[4];
	CButton m_btnEnterRoom[4];

	afx_msg void OnBnClickedRoom1New();
	afx_msg void OnBnClickedRoom2New();
	afx_msg void OnBnClickedRoom3New();
	afx_msg void OnBnClickedRoom4New();

	afx_msg void OnBnClickedRoom1Insert();
	afx_msg void OnBnClickedRoom2Insert();
	afx_msg void OnBnClickedRoom3Insert();
	afx_msg void OnBnClickedRoom4Insert();
};


