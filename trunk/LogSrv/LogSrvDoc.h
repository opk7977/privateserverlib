
// LogSrvDoc.h : CLogSrvDoc 클래스의 인터페이스
//


#pragma once

class LogDataBase;

class CLogSrvDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CLogSrvDoc();
	DECLARE_DYNCREATE(CLogSrvDoc)

// 특성입니다.
private:
	//데이터베이스 싱글톤
	LogDataBase*		m_database;
	
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CLogSrvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};


