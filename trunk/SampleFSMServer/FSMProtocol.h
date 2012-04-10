#ifndef __FSM_PROTOCOL__
#define __FSM_PROTOCOL__


enum SampleFSM{
	//--------------------------------------
	// server -> client
	// Ŭ���̾�Ʈ�� ���������� ������ Ŭ���̾�Ʈ���� ������ Ȯ�� ��Ŷ
	// ���� ���ƴٴϰ� �ִ� npc�� ������ �����ش�.
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
	// npc�� ���°� �������� �׿� �����Ͽ� ��Ŷ�� ������
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

//npc ĳ���� ���� ����ü
struct NPCObj
{
	int		charId;				//char�� Id
	int		charState;			//char�� ����
	int		charSpeed;
	int		charX;				//char�� ���� X��ġ
	int		charY;				//char�� ���� Y��ġ
	int		goalX;				//char�� ��ǥ X��ġ
	int		goalY;				//char�� ��ǥ Y��ġ
};



#endif
