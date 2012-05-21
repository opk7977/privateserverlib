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

	// client -> server
	// 계정생성을 위해 id중복 확인을 위한 패킷
	// int			-		id데이터 크기
	// TCHAR		-		id
	CS_LOGIN_CHECK_ID,

	// server -> client
	// id중복 체크에 대한 결과값을 return 해 준다
	// -1(음수) : 중복/ 1(양수) : 사용가능
	// int			-		결과값
	SC_LOGIN_CHECK_ID_RESULT,

	// client -> server
	// 계정을 만드는데 필요한 정보를 받는다
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		password데이터의 크기
	// TCHAR		-		password
	// int			-		e-mail 데이터의 크기
	// TCHAR		-		e-mail
	CS_LOGIN_CREATE_ACCOUNT,

	// server -> client
	// 계정 새로 만들기에 대한 결과
	// -10(음수) : 실패/ 1(양수) : 성공
	// int			-		결과값
	SC_LOGIN_CREATE_RESULT,

	// client -> server
	// 생성된 계정으로 로그인 시도
	// int			-		id데이터의 크기
	// TCHAR		-		id
	// int			-		password데이터의 크기
	// TCHAR		-		password
	CS_LOGIN_LOGIN,

	// server -> client
	// 로그인 결과를 return한다
	// 결과가 -10: 오류, -5: 이미 로그인되어 있음, -1: id없음/ 0: 비번틀림/ n(양수): 성공(sessionID)
	// --------결과가 성공이면 추가할 data--------------------------
	// int			-		ip주소의 데이터 크기
	// char			-		ip주소
	// int			-		port번호
	SC_LOGIN_LOGIN_RESULT,

};