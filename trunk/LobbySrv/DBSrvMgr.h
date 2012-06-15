#pragma once

#include "SSocket.h"

class LobbySession;
class SPacket;

class SLogger;

class DBSrvMgr : public SSingleton <DBSrvMgr>
{
private:
	friend class SSingleton<DBSrvMgr>;

private:
	//======================================
	// singleTon°´Ã¼
	//======================================
	SLogger*			m_logger;

private:
	SSocket				m_sock;
	SOCKADDR			m_sockAddr;

	LobbySession*		m_dbSrv;
	BOOL				m_isConnect;

private:
	DBSrvMgr(void);
	~DBSrvMgr(void);

public:
	BOOL Init();
	//¿¬°á
	BOOL ConnectToDBSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( LobbySession* session );
	inline LobbySession* GetSession() const { return m_dbSrv; }

	inline BOOL IsConnect() { return m_isConnect; }

	BOOL SendToDBServer( SPacket& packet );
};

#define GetDBSrv DBSrvMgr::GetInstance()

