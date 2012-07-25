#pragma once

#include "SServerObj.h"

class WorldSession;
class SPacket;

class FieldSrvMgr : public SSingleton <FieldSrvMgr>, public SServerObj
{
private:
	friend class SSingleton<FieldSrvMgr>;

private:
	ATL::CAtlMap<int, WorldSession*>		m_mapFieldSrv;
	int										m_fieldCount;

public:
	FieldSrvMgr(void);
	~FieldSrvMgr(void);

	//======================================
	// 초기화 & 해제
	//======================================
	BOOL Init( int _count );
	void Release();

	//필드 개수 얻기
	inline int GetFieldCount() { return m_fieldCount; }

	//해당 id에 맞는 session을 매칭(?)
	BOOL SetSession( int _id, WorldSession* session );

	//id를 확인하고 field서버로 packet을 전송
	BOOL SendToPacket( int _id, SPacket &packet );

};

#define GetFieldSrvMgr FieldSrvMgr::GetInstance()
