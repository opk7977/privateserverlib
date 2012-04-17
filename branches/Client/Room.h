#pragma once
#include "Resource.h"
#include "afxwin.h"

class RoomPlayer
{
private:
	int		roomsortNo;
	int		sessionID;
	TCHAR	myID[30];
	int		myTeam;
	int		myReady;

public:
	void Init()
	{
		sessionID = myTeam = myReady = 0;
		ZeroMemory( myID, 30 );
	}

	void SetRoomSortNo( int i ) { roomsortNo = i; }
	int GetRoomSortNo() { return roomsortNo; }

	void SetSessionID( int _id ) { sessionID = _id; }
	int GetSessionID() { return sessionID; }
	
	void SetMyID( TCHAR* _id ) { _tcsncpy_s( myID, 30, _id, _tclen( _id ) ); }
	TCHAR* GetMyID() { return myID; }

	void SetTeam( int team ) { myTeam = team; }
	int GetTeam() { return myTeam; }

	void SetReady( int ready ) { myReady = ready; }
	int GetReady() { return myReady; }
};



// CRoom �� ���Դϴ�.

class CRoom : public CFormView
{
	DECLARE_DYNCREATE(CRoom)

protected:
	CRoom();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CRoom();

public:
	enum { IDD = IDD_INROOM };
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
	//���ȣ
	int							m_roomNo;

	//ĳ���� list
	std::map<int, RoomPlayer*>	m_mapPlayer;

	//ĳ���� ��
	int							m_playerCount;

	//��
	int							m_AttTeamCount;
	int							m_DefTeamCount;

	//��
	RoomPlayer*					m_itMe;
	//����
	RoomPlayer*					m_leader;

public:
	void CreateCharSpace();
	void Release();

	void OpenRoom( int roomNo, TCHAR* title );
	void CloseRoom();

	void AddPlayer( int sessionId, TCHAR* ID, int team, int ready );
	void DelPlayer( int SessionId );

	void ChangeReady( int sessionId, int ready );
	void ChangeTeam( int sessionId, int team );

public:
	CStatic m_csTitle;

	CStatic m_player_ID[8];
	CStatic m_player_Team[8];
	CStatic m_player_Ready[8];

	CButton m_btn_changeTeam;
	CButton m_btn_Ready;
	CButton m_btn_outRoom;
	CButton m_btn_chatSend;

	CString m_chatEdit;
	CEdit m_control_chatEdit;
};


