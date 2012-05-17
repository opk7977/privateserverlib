
// LogSrvDoc.h : CLogSrvDoc Ŭ������ �������̽�
//


#pragma once

class LogDataBase;
class Network;

class CLogSrvDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CLogSrvDoc();
	DECLARE_DYNCREATE(CLogSrvDoc)

// Ư���Դϴ�.
private:
	//�����ͺ��̽� �̱���
	LogDataBase*		m_database;
	Network*			m_network;

	// log�� ���� Table
	TCHAR				m_logTable[20];

	// ����IP
	char				m_srvIP[15];
	 
	// ���� port��ȣ
	int					m_srvPort;

	//--------------------------------------
	// ���� flag
	//--------------------------------------
	BOOL				m_isDBConnect;
	BOOL				m_isSettingSrv;
	
public:

// �۾��Դϴ�.
public:
	inline void SetPort( int port ) { m_srvPort = port; }
	inline int GetPort() { return m_srvPort; }

	inline void SetIP( char* ip ) { CopyMemory( m_srvIP, ip, 15 ); }
	inline char* GetIP() { return m_srvIP; }

	inline void SetLogTbl( TCHAR* table ) { CopyMemory( m_logTable, table, 20 ); }
	inline TCHAR* GetUseTbl() { return m_logTable; }

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
public:
	afx_msg void OnAddTable();
	afx_msg void OnAddServer();
	afx_msg void OnDeleteTable();
	afx_msg void OnMenuLogsrvstart();
	afx_msg void OnMenuLogsrvsetting();
	afx_msg void OnMenuDisconnect();
};


