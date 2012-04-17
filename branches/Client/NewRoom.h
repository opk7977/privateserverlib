#pragma once

class CRoom;

// CNewRoom ��ȭ �����Դϴ�.

class CNewRoom : public CDialog
{
	DECLARE_DYNAMIC(CNewRoom)

public:
	CNewRoom(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewRoom();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CREATE_ROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	int		m_roomNum;

public:
	CRoom*	m_pRoom;

public:
	void SetRoomNum( int n );

public:
	CString m_RoomTitle;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
