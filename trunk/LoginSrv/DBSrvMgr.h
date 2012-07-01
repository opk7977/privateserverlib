#pragma once

#include "SSocket.h"

class LoginSession;
class SPacket;
class SLogger;

class DBSrvMgr : public SSingleton <DBSrvMgr>
{
private:
	friend class SSingleton<DBSrvMgr>;

private:
	//======================================
	// singleTon��ü
	//======================================
	SLogger*			m_logger;

private:
	SSocket				m_sock;
	SOCKADDR			m_sockAddr;

	LoginSession*		m_dbSrv;
	BOOL				m_isConnect;

private:
	DBSrvMgr(void);
	~DBSrvMgr(void);

public:
	BOOL Init();
	//����
	BOOL ConnectToDBSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( LoginSession* session );
	inline LoginSession* GetSession() const { return m_dbSrv; }

	inline BOOL IsConnect() { return m_isConnect; }

	BOOL SendToDBServer( SPacket& packet );
};

#define GetDBSrv DBSrvMgr::GetInstance()
