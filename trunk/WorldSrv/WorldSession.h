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
	//연결과 연결끊기
	void OnCreate();
	void OnDestroy();

	//패킷 ID체크
	void PacketParsing( SPacket& packet );

private:
	//======================================
	// 패킷 해석 & 전송 함수
	//======================================
	//WF_WORLDSRV_CONNECT_OK
	//함수 없음

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
