#pragma once

enum LOGIN_SERVER
{
	// server -> client
	// 연결이 정상적으로 됬으면 연결 됬다는 신호를 보내준다.
	// 자료 없음
	SC_LOGIN_CONNECT_OK = 0,

	// client -> server
	// 연결이 잘 됬으면 로그인 패킷을 보낸다
	// int			-		id 데이터 크기
	// TCHAR		-		id
	// int			-		pw 데이터 크기
	// TCHAR		-		pw
	CS_LOGIN_LOGIN,

	// server -> client
	// id가 존재하지 않는다는 패킷
	// 자료 없음
	SC_LOGIN_NONID,

	// server -> client
	// pw가 틀렸다는 패킷
	// 자료 없음
	SC_LOGIN_NOTPW,

	// server -> client
	// 로그인 성공 함수, game서버의 ip와 port번호를 넘긴다
	// int			-		자신의 indexID
	// int			-		ipAddr의 데이터 크기
	// char			-		ipAddr
	// int			-		port번호
	SC_LOGIN_SUCCESS,
};

enum GAME_SERVER
{
	// server -> client
	// 게임 서버에 정상적으로 접속되었음을 알림
	// 자료 없음
	SC_GAME_CONNECT_OK = 100,

	// client -> server
	// 서버에 연결이 되었으니 자신의 세션ID를 보내 캐릭터정보를 받아 와야 한다
	// int			-		SessionID
	// int			-		strID의 데이터 크기
	// TCHAR		-		strID
	CS_GAME_SEND_INDECX,

	// server -> client
	// 로그인서버에서 넘어온 정보가 없어 정상적으로 연결되지 못함
	// 자료 없음
	SC_GAME_CONNECT_FAILED,

	// server -> client
	// 접속된, 혹은 접속한 캐릭터에 대해 생성명령
	// int			-		생성해야 하는 캐릭터의 수
	// int			-		character index Id
	// int			-		strID의 데이터 크기
	// TCHAR		-		strID
	// int			-		charState
	// int			-		position X
	// int			-		position Y
	SC_GAME_CREATE_CHARACTER,

	// client -> server
	// 접속된 캐릭터에 대해 상태 변경 명령
	// int			-		character index Id
	// int			-		charState
	// int			-		position X
	// int			-		position Y
	CS_GAME_CHANGE_STATE,

	// server -> client
	// 접속된 캐릭터에 대해 상태 변경 명령
	// int			-		character index Id
	// int			-		charState
	// int			-		position X
	// int			-		position Y
	SC_GAME_CHANGE_STATE,

	// server -> client
	// 접속이 끊겨 삭제되어야 하는 캐릭터에 대해 아림
	// 접속된 캐릭터에 대해 상태 변경 명령
	// int			-		character index Id
	SC_GAME_DELETE_CHARACTER,
};