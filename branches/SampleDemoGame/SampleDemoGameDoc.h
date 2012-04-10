
// SampleDemoGameDoc.h : CSampleDemoGameDoc 클래스의 인터페이스
//


#pragma once


class CSampleDemoGameDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CSampleDemoGameDoc();
	DECLARE_DYNCREATE(CSampleDemoGameDoc)

// 특성입니다.
public:

// 작업입니다.
private:
	//Login 서버와 연결이 되어 있는지를 나타내는 flag
	BOOL		isConnectToLoginSrv;
	BOOL		isLogin;
	int			myindexID;

	//game서버와 연결이 되어 있는지를 나타내는 flag
	BOOL		isConnectToGameSrv;

public:
	void SetConnect( BOOL conn );
	BOOL GetConnect();

	void SetLogin( BOOL login );
	BOOL GetLogin();

	void SetIndexId( int index );
	int	GetIndexId();


	void SetConnectToG( BOOL conn );
	BOOL GetConnectToG();

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CSampleDemoGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};


