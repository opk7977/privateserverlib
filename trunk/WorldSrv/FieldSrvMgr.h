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
	// �ʱ�ȭ & ����
	//======================================
	BOOL Init( int _count );
	void Release();

	//�ʵ� ���� ���
	inline int GetFieldCount() { return m_fieldCount; }

	//�ش� id�� �´� session�� ��Ī(?)
	BOOL SetSession( int _id, WorldSession* session );

	//id�� Ȯ���ϰ� field������ packet�� ����
	BOOL SendToPacket( int _id, SPacket &packet );

};

#define GetFieldSrvMgr FieldSrvMgr::GetInstance()
