
// LogSrvDoc.h : CLogSrvDoc Ŭ������ �������̽�
//


#pragma once

class LogDataBase;

class CLogSrvDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CLogSrvDoc();
	DECLARE_DYNCREATE(CLogSrvDoc)

// Ư���Դϴ�.
private:
	//�����ͺ��̽� �̱���
	LogDataBase*		m_database;
	
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CLogSrvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


