#pragma once

#include "SServerObj.h"

class DBSession;
class SPacket;

class SrvMgr : public SServerObj
{
private:
	DBSession*			m_Srv;

public:
	SrvMgr(void);
	~SrvMgr(void);

	void Init();
	void Release();

	BOOL SetSession( DBSession* session );
	DBSession* GetSession() const;

	BOOL SendPacketToSrv( SPacket& packet );
};
