
// ClientDoc.h : CClientDoc Ŭ������ �������̽�
//


#pragma once


class CClientDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CClientDoc();
	DECLARE_DYNCREATE(CClientDoc)

// Ư���Դϴ�.
public:
	int isSceneState;		//������ ����(0:��Ʈ��X, 1:�κ�, 2:�濡 ���� )


	BOOL isConnectToLogin;	//�α��μ����� ����Ǳ� ��ٸ� ����\

	int  CheckID;			//���̵� üũ����� �޾Ҵ����� ���� ����

	int isCreateResult;

	int SessionID;			//���� ID
	TCHAR strId[50];		//��id

	BOOL isConnectToLobby;	//�κ� ������ ����Ǳ� ��ٸ� ����

	int myRoomNum;

// �۾��Դϴ�.
public:
	void Init();

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


