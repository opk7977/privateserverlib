#pragma once

enum SERVER_CODE
{
	// 쿼리 오류 등 서버 자체적인 에러로
	// 정상적인 처리가 되지 못한경우
	// 결과값으로 들어가 있을 수 있다.
	SERVER_ERROR = -10,
	// 성공의 결과값
	SUCCESSED_ACK = 1,

	//--------------------------------------------------------------

	// ID체크 결과로 ID가 중복될때
	OVERLAPPED_ID = -1,

	// 로그인 결과로 해당 ID가 이미 로그인 중에 있을때
	PRE_LOGIN_ID = -5,
	// 로그인 결과로 ID가 없음
	NONEXISTENT_ID = -1,
	// 로그인 결과로 PW가 틀림
	WRONG_PW = 0,
	
	//--------------------------------------------------------------
	
	// 방만들기 실패( 더이상 방을 만들 수 없음 )
	ROOMCOUNT_IS_FULL = -1,
	
	// 방 입장시 방이 게임중이면
	ROOM_IS_PLAY_NOW = -1,
	// 방 입장시 방이 인원 초과
	ROOM_PLAYERCOUNT_FULL = 0,

	//--------------------------------------------------------------

	// 게임 종료의 종류
	// 시간 초과
	GAME_END_TIMEATTECT = 1,
	// 팀 전멸
	GAME_END_TEAM_ALL_DIE,
	// 팀 미션의 성공
	GAME_END_MISSION_SUCCESS,
};

enum SERVER_TO_SERVER
{
	// gameSrv -> lobbySrv
	// 게임서버가 로비서버로 자신이 게임서버라는 것을 알린다
	// 데이터 없음
	GL_CONNECT_SERVER = 100,

	// lobbySrv -> gameSrv
	// 방의 유저들이 게임을 시작하게 되어 게임서버의 준비상태를 명령
	// int			-		roomNum
	// int			-		playerCount
	//--------------------------------------------------------------
	// int			-		SessionId
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		Team
	//--------------------------------------------------------------playerCount만큼 반복으로 들어가 있음
	LG_START_GAME,

	// gameSrv -> lobbySrv
	// 게임 proc을 열수가 없거나 열기에 실패했다
	// int			-		roomNum
	GL_START_FAILD,

	// gameSrv -> lobbySrv
	// 게임 시작이 가능한 상태니 신호를 보내라는 패킷
	// int			-		roomNum
	GL_START_OK,

	// gameSrv -> lobbySrv
	// 게임이 종료되고 방을 다시 일반 상태로 바꿔 준다
	// int			-		roomNum
	GL_GAME_END,

	// gameSrv -> lobbySrv
	// 게임서버에서 접속을 끊어버린 player에 대한 처리
	// int			-		roomNum
	// int			-		SessionId
	GL_PLAYER_DISCONNECT,
};

enum LOBBY_SERVER
{
	// server -> client
	// 로비 서버에 연결이 정상적으로 되었다는 확인 패킷
	// 데이터 없음
	SC_LOBBY_CONNECT_OK = 200,

	// client -> server
	// 로비에 접속하면서 자신의 정보를 보내며 다른 정보를 요청하는 패킷
	// int			-		SessionId
	// int			-		RoomNum( 없다면 0을 보낸다 )
	CS_LOBBY_INSERT_LOBBY,

	// server -> client
	// 접속한 유저에게 혹은 접속해 있는 유저에게 캐릭터 정보를 보내는 패킷
	// int			-		playerCount
	//--------------------------------------
	// int			-		SessionId
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		Room(0:로비/ n:방번호)
	// int			-		Playstate
	//--------------------------------------> playerCount만큼 반복으로 들어가 있음
	SC_LOBBY_OTHER_CHARINFO,

	// server -> client
	// 접속한 유저에게 혹은 접속해 있는 유저에게 방에 대한 정보를 보내는 패킷
	// int			-		roomCount
	//--------------------------------------
	// int			-		RoomId
	// int			-		playerCountInRoom
	// int			-		RoomTitle데이터의 크기
	// TCHAR		-		RoomTitle
	// int			-		stateOfRoom ( ex)0:일반, 1(양수):게임중 )
	// int			-		stageMap
	//--------------------------------------> roomCount만큼 반복으로 들어가 있음
	SC_LOBBY_ROOMINFO,

	// client -> server
	// 방 만들기를 요청한다.
	// int			-		roomTitle데이터의 크기
	// TCHAR		-		roomTitle
	CS_LOBBY_CREATE_ROOM,

	// server -> client
	// 방만들기에 대한 결과를 보낸다
	// int			-		result ( -1:실패/ n(양수) 만들어진 방의 ID )
	// int			-		Team	//방생성 실패시 데이터 없다
	SC_ROOM_RESULT_CREATE,

	// server -> client
	// 접속한 사람들에게 방이 만들어 졌다는 패킷을 보낸다.
	// int			-		roomNum
	// int			-		roomTitle데이터 크기
	// int			-		roomTitle
	SC_LOBBY_OPEN_ROOM,

	// server -> client
	// 접속한 사람들에게 방이 닫혔다는 패킷을 보낸다
	// int			-		roomNum
	SC_LOBBY_CLOSE_ROOM,

	// client -> server
	// 방으로 들어가는 요청을 보내는 함수
	// int			-		roomNum
	CS_LOBBY_INSERT_ROOM,

	// server -> client
	// 방에 들어가는 요청이 잘 받아 졌는지 결과를 보내주는 패킷
	// int			-		result
	// int			-		Team	//못들어갔으면 데이터 없다
	SC_ROOM_RESULT_INSERT,

	// server -> client
	// 로비의 사람들에게 누군가 방으로 들어갔는 패킷을 보낸다.
	// int			-		SessionID
	// int			-		roomId
	SC_LOBBY_INSERT_ROOM,

	// server -> client
	// 방에 있는 사람들의 정보를 받음
	// int			-		playerCount
	//--------------------------------------
	// int			-		SessionId
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		Team
	// int			-		Ready
	//--------------------------------------> playerCount만큼 반복
	SC_ROOM_OTHER_CHARINFO,

	// server -> client
	// 방장이 나가고 다음 방장이 누구인지 보낸다.
	// int			-		SessionId
	SC_ROOM_LEADER,

	// client -> server
	// 방에서 나가 로비를 가는것을 알림
	// 데이터 없음
	CS_ROOM_OUT_ROOM,

	// server -> client
	// 방에 사람들에게 누군가 방을 나갔가고 알림
	// int			-		SessionId
	SC_ROOM_CHAR_OUT,

	// server -> client
	// 로비에 있는 사람들에게 방에 사람 수가 줄었다는 것을 알림
	// int			-		roomNum
	// int			-		sessionId
	SC_LOBBY_ROOM_PLAYER_OUT,

	// client -> server
	// ready상태를 변경하고 상태를 서버로 보냄
	CS_ROOM_CHAR_READY,

	// server -> client
	// 바뀐 ready상태를 방의 유저들에게 알림
	// int			-		SessionID
	// int			-		ready 상태
	SC_ROOM_CHAR_READY,

	// client -> server
	// 맵을 변경
	// int			-		mapNum
	CS_ROOM_MAP_CHANGE,

	// server -> client
	// 변경된 맵을 전함
	// int			-		mapNum
	SC_ROOM_MAP_CHANGE,

	// server -> client
	// 로비에 맵이 변경된 방에 대한 정보를 알림
	// int			-		roomnum
	// int			-		mapnum
	SC_LOBBY_MAP_CHANGE,

	// client -> server
	// 팀 변경에 대한 상태를 알림
	CS_ROOM_TEAM_CHANGE,

	// server -> client
	// 바뀐 팀 정보를 방의 유저들에게 알림
	// int			-		SessionId
	// int			-		Team
	SC_ROOM_TEAM_CHANGE,

	// client -> server
	// 전체 채팅
	// int			-		문장의 데이터 크기
	// TCHAR		-		문장
	CS_LOBBYSRV_ALL_CHAT,

	// server -> client
	// 전체 채팅
	// int			-		0:로비/ n: 방
	// int			-		문장의 데이터 크기
	// TCHAR		-		문장
	SC_LOBBYSRV_ALL_CHAT,

	// client -> server
	// 귓말 전송
	// int			-		상대방 ID의데이터 크기
	// TCHAR		-		상대방의 ID
	// int			-		문장의 데이터 크기
	// TCHAR		-		문장
	CS_LOBBYSRV_TARGET_CHAT,

	// server -> client
	// 귓말 전송
	// int			-		0: 로비/ n: 방번호
	// int			-		문장의 데이터 크기
	// TCHAR		-		문장
	SC_LOBBYSRV_TARGET_CHAT,

	// client -> server
	// 방장이 게임을 시작하려고 알리는 패킷
	// 데어터 없음
	CS_ROOM_START,

	// server -> client
	// 게임을 시작할 수 있는지에 대한 결과
	// BOOL			-		결과값
	SC_ROOM_START_RESULT,

	// server -> client
	// 게임 시작을 클라에게 알린다.
	// int			-		ip데이터크기
	// TCHAR		-		ip
	// int			-		port번호
	SC_ROOM_GAME_START,

	// server -> client
	// 로비에 있는 player들에게 게임이 시작된 방의 번호를 알린다
	// int			-		방번호
	SC_LOBBY_GAME_START,

	// server -> client
	// 로비에 있는 player들에게 게임이 끝난 방의 번호를 알린다.
	// int			-		방번호
	SC_LOBBY_GAME_END,

	// server -> client
	// player(내)가 로비에서 접속을 끊으면 알려 준다.
	// int			-		SessionID;
	SC_LOBBY_PLAYER_DISCONNECT,

	// server -> client
	// player가 방(혹은게임)에서 접속을 끊으면 알려 준다
	// int			-		roomNum
	// int			-		sessionID
	SC_ROOM_PLAYER_DISCONNECT,
};