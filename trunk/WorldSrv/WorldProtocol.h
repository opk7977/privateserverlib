
enum WORLD_SERVER
{
	// world -> field
	// �ʵ� ������ ���� ������ ������ ����
	// ������ ����
	WF_WORLDSRV_CONNECT_OK						= 10,

	// filed -> world
	// �ڽ��� ������ ��û�Ѵ�
	// ������ ����
	FW_REQ_FIELD_SPACE,

	// world -> field
	// ������ �Ҵ��� �ش�
	// int				-			�ش� �ʵ��� id
	// int				-			�ʵ� �̸�ũ��
	// TCHAR			-			�ʵ� �̸�
	// int				-			top
	// int				-			left
	// int				-			bottom
	// int				-			right
	// int				-			+���ðŸ�
	WF_FIELDINFO,

	// field -> world
	// ��� �غ� �������� �˸�
	// ������ ����
	FW_NOTIFY_READY,

	// workd -> field
	// ��� �ʵ� ������ �غ� �� ����
	// ���� ������ ������ ����
	// ������ ����
	WF_NOTIFY_START,

	// field -> world
	// ���Ͱ� ������
	// int				-			������ ���� ��
	//--------------------------------------> ���� �� ��ŭ �ݺ�
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	//--------------------------------------
	FW_NPC_CREATED,

	// world -> field
	// �ٸ� �������� ���Ͱ� ������ٰ� �˸�
	// int				-			������ ���� ��
	//--------------------------------------> ���� �� ��ŭ �ݺ�
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	//--------------------------------------
	WF_NPC_CREATED,

	// field -> world
	// ĳ������ ���°� �ٲ�
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	FW_NPC_CHANGE_STATE,

	// world -> field
	// ĳ������ ���º�ȭ�� �˸�
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	WF_NPC_CHANGE_STATE,

	// field -> world
	// ĳ������ �ֵ����� �ٲ�
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	FW_NPC_MOVE_CONTROL,

	// world -> field
	// ĳ������ ��ġ�� Ȯ���ϰ� �Ű��� �ʵ弭���� �ֵ����� �ѱ�
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	WF_NPC_MOVE_CONTROL,	
};