
// LogSrvDoc.h : CLogSrvDoc 클래스의 인터페이스
//


#pragma once

class LogDataBase;
class Network;

class CLogSrvDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CLogSrvDoc();
	DECLARE_DYNCREATE(CLogSrvDoc)

// 특성입니다.
private:
	//데이터베이스 싱글톤
	LogDataBase*		m_database;
	Network*			m_network;

	// log가 쌓일 Table
	TCHAR				m_logTable[20];

	// 서버IP
	char				m_srvIP[15];
	 
	// 서버 port번호
	int					m_srvPort;

	//--------------------------------------
	// 연결 flag
	//--------------------------------------
	BOOL				m_isDBConnect;
	BOOL				m_isSettingSrv;
	
public:

// 작업입니다.
public:
	inline void SetPort( int port ) { m_srvPort = port; }
	inline int GetPort() { return m_srvPort; }

	inline void SetIP( char* ip ) { CopyMemory( m_srvIP, ip, 15 ); }
	inline char* GetIP() { return m_srvIP; }

	inline void SetLogTbl( TCHAR* table ) { CopyMemory( m_logTable, table, 20 ); }
	inline TCHAR* GetUseTbl() { return m_logTable; }

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
public:
	afx_msg void OnAddTable();
	afx_msg void OnAddServer();
	afx_msg void OnDeleteTable();
	afx_msg void OnMenuLogsrvstart();
	afx_msg void OnMenuLogsrvsetting();
	afx_msg void OnMenuDisconnect();
};


