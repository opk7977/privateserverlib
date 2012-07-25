#pragma once

#include "SSessionObj.h"

class SLogger;

class WorldSession : public SSessionObj
{
public:
	SDECLARE_DYNAMIC(WorldSession)
	SDECLARE_DYNCREATE(WorldSession)

private:
	//======================================
	// singleTon
	//======================================
	static SLogger*				m_logger;

public:
	WorldSession(void);
	~WorldSession(void);

public:
	//����� �������
	void OnCreate();
	void OnDestroy();

	//��Ŷ IDüũ
	void PacketParsing( SPacket& packet );

private:
	//======================================
	// ��Ŷ �ؼ� & ���� �Լ�
	//======================================
	//WF_WORLDSRV_CONNECT_OK
	//�Լ� ����

	//FW_REQ_FIELD_SPACE / WF_FIELDINFO
	void ReqFieldInfo();

	//FW_NOTIFY_READY / WF_NOTIFY_START
	void RecvReady( SPacket &packet );

	//FW_NPC_CREATED / WF_NPC_CREATED
	void RecvNpcCreated( SPacket &packet );

	//FW_NPC_CHANGE_STATE / WF_NPC_CHANGE_STATE
	void RecvChangeState( SPacket &packet );

	//FW_NPC_MOVE_CONTROL / WF_NPC_MOVE_CONTROL
	void RecvMoveNpcControl( SPacket &packet );

};
