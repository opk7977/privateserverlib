#pragma once

#include "SSocket.h"

class GameSession;
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

	GameSession*		m_dbSrv;

private:
	DBSrvMgr(void);
	~DBSrvMgr(void);

public:
	BOOL Init();
	//¿¬°á
	BOOL ConnectToDBSrv( char* ipAddr, int port );
	void DisConnect();

	void SetSession( GameSession* session );
	inline GameSession* GetSession() const { return m_dbSrv; }

	inline BOOL IsConnect() { return (m_dbSrv != NULL); }

	BOOL SendToDBServer( SPacket& packet );
};

#define GetDBSrv DBSrvMgr::GetInstance()

