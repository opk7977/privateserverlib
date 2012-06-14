#pragma once

enum SERVER_CODE
{
	// 쿼리 오류 등 서버 자체적인 에러로
	// 정상적인 처리가 되지 못한경우
	// 결과값으로 들어가 있을 수 있다.
	SERVER_ERROR			= -10,
	// 성공의 결과값
	SUCCESSED_ACK			= 1,

	//--------------------------------------------------------------

	// ID체크 결과로 ID가 중복될때
	OVERLAPPED_ID			= -1,

	// 로그인 결과로 해당 ID가 이미 로그인 중에 있을때
	PRE_LOGIN_ID			= -5,
	// 로그인 결과로 ID가 없음
	NONEXISTENT_ID			= -1,
	// 로그인 결과로 PW가 틀림
	WRONG_PW				= 0,

	//--------------------------------------------------------------

	// 더이상 로그인 할 수 없습니다
	CHARAVER_SPACE_IS_FULL	= -20,
	
	//--------------------------------------------------------------
	
	// 방만들기 실패( 더이상 방을 만들 수 없음 )
	ROOMCOUNT_IS_FULL		= -1,
	
	// 방 입장시 방이 게임중이면
	ROOM_IS_PLAY_NOW		= -1,

	// 방 입장시 방이 인원 초과
	ROOM_PLAYERCOUNT_FULL	= 0,

	//--------------------------------------------------------------

	// 게임 종료의 종류
	// 시간 초과
	GAME_END_TIMEATTECT		= 1,
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
	// int			-		Team정보
	GL_PLAYER_DISCONNECT,
};

enum DB_SERVER
{
	// DBSrv -> otherSrv
	// 연결이 제대로 되었다는 신호
	// 데이터 없음
	DB_TO_OTHER_CONNECT_OK = 1000,

	// loginSrv -> DBSrv
	// id중복 체크
	// int			-		임시 index
	// int			-		id데이터 크기
	// TCHAR		-		id
	LOGIN_TO_DB_CHECK_ID,

	// DBSrv -> loginSrv
	// id중복체크 결과를 보낸다.
	// int			-		임시 index
	// BOOL			-		result(사용가능:TRUE/사용불가:FALSE)
	DB_TO_LOGIN_CHECK_ID_RESULT,

	// loginSrv -> DBSrv
	// 계정생성을 요청한다
	// int			-		임시 index
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		pw데이터의 크기
	// TCHAR		-		pw
	// int			-		email데이터의 크기
	// TCHAR		-		email
	LOGIN_TO_DB_CREATE_ACCOUNT,

	// DBSrv -> loginSrv
	// 계정생성에 대한 결과를 보낸다
	// BOOL			-		result( 성공:TRUE/ 실패:FALSE )
	DB_TO_LOGIN_CREATE_ACCOUNT_RESULT,

	// loginSrv -> DBSrv
	// 로그인
	// int			-		임시 index
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		pw데이터의 크기
	// TCHAR		-		pw
	LOGIN_TO_DB_TRYLOGIN,

	// DBSrv -> lobbySrv
	// 로그인이 됬다면 lobby서버로 이 user의 data를 보낸다
	// int			-		임시 index
	// int			-		sessionId
	// int			-		id데이터 크기
	// TCHAR		-		id
	// int			-		Rank_ID
	// int			-		Rank_Point
	// int			-		AccumulatedKillPoint
	// int			-		AccumulatedDeathPoint
	DB_TO_LOBBY_CHARACTER_LOGIN,

	// lobbySrv -> DBSrv
	// 로비에 캐릭터 정보가 준비 되었다는 신호를 로비가 디비에게 전달
	// int			-		임시 index
	// int			-		sessionID
	// int			-		결과
	//						( 정상적으로 준비 완료 : 1
	//						  캐릭터가 더이상 접속 할 수 없음 : -10 )
	LOBBY_TO_DB_CHARINSERT_READY_RESULT,

	// DBSrv -> loginSrv
	// 로그인 결과를 보낸다
	// int			-		임시 index
	// int			-		result( 이미로그인중: PRE_LOGIN_ID(-5)
	//							  / 없는 아이디 : NONEXISTENT_ID(-1)
	//							  / 비번 틀림   : WRONG_PW(0)
	//							  / 성공        : n(자신의sessionID) )
	DB_TO_LOGIN_LOGINRESULT,

	// DBSrv -> otherSrv
	// 중복된 id로 로그인 됩니다. 기존에 접속된 user의 접속을 끊습니다.
	// int			-		sessionID
	DB_TO_OTHER_DROP_PLAYER,

	// gameSrv -> DBSrv
	// 게임이 끝나고 캐릭터들의 정보를 갱신한다
	// int			-		game(room)Num
	// int			-		Playercount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_point	// 증가될 point
	// int			-		kill		// 증가될 killcount
	// int			-		death		// 증가될 deathcount
	//--------------------------------------> playerCount만큼 반복
	GAME_TO_DB_UPDATE_USERDATA,

	// DBSrv -> GameSrv
	// 캐릭터들의 정보를 갱신하고 갱신된 캐릭터중 레벨이 없된 정보를 받는다
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	//--------------------------------------
	DB_TO_GAME_LEVEL_UP_PLAYER,

	// DBSrv -> lobbySrv
	// 캐릭터 정보가 계산되어 갱신되면 로비에 새로운 데이터를 알린다
	// int			-		game(room)Num
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	// int			-		rank_point	// 증가된 point
	// int			-		kill		// 증가된 killcount
	// int			-		death		// 증가된 deathcount
	//--------------------------------------> playerCount만큼 반복
	DB_TO_LOBBY_UPDATE_USERDATA,

	// otherSrv -> DBSrv
	// 캐릭터가 접속을 끊었습니다(게임을 종료)
	// int			-		sessionId
	OTHER_TO_DB_DISCONNECT_CHARACTER,
};

enum GAME_SERVER
{
	// server -> client
	// 게임서버와의 연결이 정상적으로 되었다는 신호
	SC_GAME_CONNECT_OK = 300,
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
	//--------------------------------------> retult가 성공이면 아래 데이터가 온다
	// int			-		방제목 데이터 길이
	// TCHAR		-		방제목
	// int			-		stageMap
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
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
	// int			-		result( -1: 실패/ 1: 성공 )
	SC_ROOM_RESULT_INSERT,

	// server -> client
	// 방에 들어가는 요청이 잘 받아 졌는지 결과를 보내주는 패킷
	// int			-		result
	//--------------------------------------> retult가 성공이면 아래 데이터가 온다
	// int			-		방제목 데이터 길이
	// TCHAR		-		방제목
	// int			-		stageMap
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
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
	// 방에서 정상적으로 나가졌는지를 받는다
	// 데이터 없음
	SC_ROOM_OUT_RESULT,

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
	// int			-		stageMap
	CS_ROOM_MAP_CHANGE,

	// server -> client
	// 변경된 맵을 알림
	// int			-		stageMap
	SC_ROOM_MAP_CHANGE,

	// server -> client
	// int			-		roomNum
	// int			-		stageMap
	SC_LOBBY_MAP_CHANGE,

	// client -> server
	// 게임의 Mode를 변경
	// int			-		mapNum
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
	CS_ROOM_MODE_CHANGE,

	// server -> client
	// 변경된 Mode을 전함
	// int			-		mapNum
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
	SC_ROOM_MODE_CHANGE,

	// server -> client
	// 로비에 맵이 변경된 방에 대한 정보를 알림
	// int			-		roomnum
	// int			-		mapnum
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
	SC_LOBBY_MODE_CHANGE,

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

	// server -> client
	// 방장에게 이제 start버튼을 활성화 하도록 알려줌
	// 데이터 없음
	SC_ROOM_START_VISIBLE,

	// server -> client
	// 방장에게 start버튼을 비활성화 하도록 알려줌
	// 데이터 없음
	SC_ROOM_START_INVISIBLE,

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
};