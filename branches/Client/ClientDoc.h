
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
	BOOL isConnectToLogin;	//�α��μ����� ����Ǳ� ��ٸ� ����
	BOOL isCheckID;			//���̵� üũ����� �޾Ҵ����� ���� ����
	int  CheckID;
	BOOL isReturnLogin;		//�α��� ����� �޾ƿԴ����� üũ�� ����
	BOOL isConnectToLobby;	//�κ� ������ ����Ǳ� ��ٸ� ����

	BOOL SessionID;			//���� ID
	CString strId[50];		//��id

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


