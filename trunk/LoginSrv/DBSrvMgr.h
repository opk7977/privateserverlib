#pragma once

#include "SSocket.h"

class LoginSession;
class SPacket;

#ifdef _DEBUG
	class SLogger;
#endif

class DBSrvMgr : public SSingleton <DBSrvMgr>
{
private:
	friend class SSingleton<DBSrvMgr>;

private:
	//======================================
	// singleTon°´Ã¼
	//======================================
#ifdef _DEBUG
	SLogger*			m_logger;
#endif

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
	//¿¬°á
	BOOL ConnectToDBSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( LoginSession* session );
	inline LoginSession* GetSession() const { return m_dbSrv; }

	inline BOOL IsConnect() { return m_isConnect; }

	BOOL SendToDBServer( SPacket& packet );
};

#define GetDBSrv DBSrvMgr::GetInstance()

