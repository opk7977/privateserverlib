
// ClientDoc.h : CClientDoc 클래스의 인터페이스
//


#pragma once


class CClientDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CClientDoc();
	DECLARE_DYNCREATE(CClientDoc)

// 특성입니다.
public:
	int isSceneState;		//지금의 상태(0:네트웍X, 1:로비, 2:방에 들어옴 )


	BOOL isConnectToLogin;	//로그인서버에 연결되길 기다릴 변수\

	int  CheckID;			//아이디 체크결과를 받았는지를 위한 변수

	int isCreateResult;

	int SessionID;			//세션 ID
	TCHAR strId[50];		//내id

	BOOL isConnectToLobby;	//로비 서버에 연결되길 기다릴 변수

	int myRoomNum;

// 작업입니다.
public:
	void Init();

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};


