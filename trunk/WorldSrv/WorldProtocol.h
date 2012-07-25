
enum WORLD_SERVER
{
	// world -> field
	// 필드 서버가 월드 서버에 접속이 성공
	// 데이터 없음
	WF_WORLDSRV_CONNECT_OK						= 10,

	// filed -> world
	// 자신의 영역을 요청한다
	// 데이터 없음
	FW_REQ_FIELD_SPACE,

	// world -> field
	// 영역을 할당해 준다
	// int				-			해당 필드의 id
	// int				-			필드 이름크기
	// TCHAR			-			필드 이름
	// int				-			top
	// int				-			left
	// int				-			bottom
	// int				-			right
	// int				-			+가시거리
	WF_FIELDINFO,

	// field -> world
	// 모든 준비가 끝났음을 알림
	// 데이터 없음
	FW_NOTIFY_READY,

	// workd -> field
	// 모든 필드 서버가 준비가 다 됬음
	// 몬스터 생성이 가능한 상태
	// 데이터 없음
	WF_NOTIFY_START,

	// field -> world
	// 몬스터가 생성됨
	// int				-			생성된 몬스터 수
	//--------------------------------------> 몬스터 수 만큼 반복
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	//--------------------------------------
	FW_NPC_CREATED,

	// world -> field
	// 다른 서버에서 몬스터가 생성됬다고 알림
	// int				-			생성된 몬스터 수
	//--------------------------------------> 몬스터 수 만큼 반복
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	//--------------------------------------
	WF_NPC_CREATED,

	// field -> world
	// 캐릭터의 상태가 바뀜
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	FW_NPC_CHANGE_STATE,

	// world -> field
	// 캐릭터의 상태변화를 알림
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	WF_NPC_CHANGE_STATE,

	// field -> world
	// 캐릭터의 주도권이 바뀜
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	FW_NPC_MOVE_CONTROL,

	// world -> field
	// 캐릭터의 위치를 확인하고 옮겨진 필드서버로 주도권을 넘김
	// int				-			npcID
	// int				-			npcState
	// int				-			npcPosX
	// int				-			npcPosY
	WF_NPC_MOVE_CONTROL,	
};