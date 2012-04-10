#ifndef __FSM_PROTOCOL__
#define __FSM_PROTOCOL__


enum SampleFSM{
	//--------------------------------------
	// server -> client
	// 클라이언트가 접속했을때 서버가 클라이언트에게 보내는 확인 패킷
	// 현재 돌아다니고 있는 npc의 정보를 보내준다.
	// int			-			npcCount
	//--------------------------------------
	// int			-			npcID
	// int			-			npcState
	// int			-			npcSpeed
	// int			-			npcX
	// int			-			npcY
	// int			-			goalX;
	// int			-			goalY;
	//				.
	//				.
	//				.
	//--------------------------------------
	SERVER_CONNECT_OK = 0,

	//--------------------------------------
	// server -> client
	// npc의 상태가 변했을때 그에 관련하여 패킷을 보낸다
	// int			-			npcID
	// DWORD		-			npcState
	// int			-			npcX
	// int			-			npcY
	// int			-			goalX;
	// int			-			goalY;
	//--------------------------------------
	SERVER_CHANGE_CHARACTER,

};



//////////////////////////////////////////////////////////////////////////

//npc 캐릭터 정보 구조체
struct NPCObj
{
	int		charId;				//char의 Id
	int		charState;			//char의 상태
	int		charSpeed;
	int		charX;				//char의 현재 X위치
	int		charY;				//char의 현재 Y위치
	int		goalX;				//char의 목표 X위치
	int		goalY;				//char의 목표 Y위치
};



#endif
