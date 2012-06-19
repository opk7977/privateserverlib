#pragma once

//#define CONNECT_LOG_SERVER

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
};

enum LOG_SERVER
{
	//========================================================
	// 로그서버 -> 다른서버
	// 로그 서버로 정상적으로 접속되었다
	//--------------------------------------------------------
	// 데이터 없음
	//========================================================
	LOG_SERVER_CONNECT_OK = 10000,

	//========================================================
	// 다른서버 -> 로그서버
	// 로그 서버로 넘어 오는 모든 패킷이 동일한 ID를 갖는다
	//--------------------------------------------------------
	// int			-			ServerID
	// int			-			LogLevel
	// int			-			LogDataLenth
	// TCHAR		-			LogString
	//========================================================
	RECV_SERVER_LOG,

	//========================================================
	// 로그서버 -> 다른서버
	// 로그 서버 문 닫음
	//--------------------------------------------------------
	// 데이터 없음
	//========================================================
	LOG_SERVER_SHOTDOWN,
};

enum LOGIN_SERVER
{
	// server -> client
	// 로그인 서버에 연결이 정상적으로 되었다는 확인 패킷
	// 데이터 없음
	SC_LOGIN_CONNECT_OK = 10,
};

enum LOBBY_SERVER
{
	// server -> client
	// 로비 서버에 연결이 정상적으로 되었다는 확인 패킷
	// 데이터 없음
	SC_LOBBY_CONNECT_OK = 200,
};

enum GAME_SERVER
{
	// server -> client
	// 게임서버와의 연결이 정상적으로 되었다는 신호
	SC_GAME_CONNECT_OK = 300,
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
	// int			-		game(room)Num
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	//--------------------------------------
	DB_TO_GAME_LEVEL_UP_PLAYER,

	// DBSrv -> lobbySrv
	// 캐릭터 정보가 계산되어 갱신되면 로비에 새로운 데이터를 알린다
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
