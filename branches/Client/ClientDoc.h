
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
	BOOL isConnectToLogin;	//�α��μ����� ����Ǳ� ��ٸ� ����\

	BOOL isCheckID;			//���̵� üũ����� �޾Ҵ����� ���� ����
	int  CheckID;

	BOOL RecvCreateResult;
	int isCreateResult;

	BOOL isReturnLogin;		//�α��� ����� �޾ƿԴ����� üũ�� ����
	BOOL SessionID;			//���� ID
	CString strId[50];		//��id

	BOOL isConnectToLobby;	//�κ� ������ ����Ǳ� ��ٸ� ����


	

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


