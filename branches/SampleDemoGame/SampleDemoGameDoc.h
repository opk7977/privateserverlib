
// SampleDemoGameDoc.h : CSampleDemoGameDoc Ŭ������ �������̽�
//


#pragma once


class CSampleDemoGameDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSampleDemoGameDoc();
	DECLARE_DYNCREATE(CSampleDemoGameDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
private:
	//Login ������ ������ �Ǿ� �ִ����� ��Ÿ���� flag
	BOOL		isConnectToLoginSrv;
	BOOL		isLogin;
	int			myindexID;

	//game������ ������ �Ǿ� �ִ����� ��Ÿ���� flag
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

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CSampleDemoGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


