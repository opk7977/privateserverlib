#pragma once

enum SERVER_TO_SERVER
{
	// gameSrv -> lobbySrv
	// 게임서버가 로비서버로 자신이 게임서버라는 것을 알린다
	// 데이터 없음
	GL_CONNECT_SERVER = 100,

	// lobbySrv -> gameSrv
	// 방의 유저들이 게임을 시작하게 되어 게임서버의 준비상태를 명령
	// int			-		roomNo
	// int			-		playerCount
	LG_START_GAME,

	// gameSrv -> lobbySrv
	// 게임서버에서 접속을 끊어버린 player에 대한 처리
	// int			-		roomNum
	// int			-		SessionId
	GL_PLAYER_DISCONNECT,
};

enum GAME_SERVER
{
	// server -> client
	// 게임서버와의 연결이 정상적으로 되었다는 신호
	SC_GAME_CONNECT_OK = 300,

	// client -> server
	// 게임서버의 자신의 정보를 보냄
	// int			-		sessionID
	// int			-		roomNum
	CS_GAME_INGAME,

	// server -> client
	// 접속한 player의 게임시작 좌표값을 포함한 정보
	// 이미 접속해 있는 player들의 정보
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	//-------------------------------------> playerCount만큼 반복으로 들어가 있음
	SC_GAME_CHARINFO_INGAME,

	// server -> client
	// 게임에 모든 player가 접속했으므로 게임을 시작하라는 명령
	// 데이터 없음
	SC_GAME_START_GAME,

	// client -> server
	// 캐릭터의 상태가 변함( 이동시작/멈춤 등 )
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	CS_GAME_MOVE_CHAR,

	// server -> client
	// 캐릭터의 상태가 변함( 이동시작/멈춤 등 )
	// int			-		sessionID
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	SC_GAME_MOVE_CHAR,

	// client -> server
	// 캐릭터의 공격
	// int			-		무기종류
	// int			-		피격대상 sessionID
	// int			-		피격부위
	CS_GAME_ATTECT,

	// server -> client
	// 캐릭터의 공격성공시 모두에게 결과를 보낸다
	// 
	// int			-		공격자SessionID
	// int			-		무기종료
	// int			-		피격대상SessionID
	// int			-		피격부위
	// int			-		감소 에너지
	SC_GAME_ATTECT,

	// server -> client
	// 공격당한 캐릭터가 죽으면 보내는 패킷
	// int			-		공격자SessionID
	// int			-		무기종류
	// int			-		피격대상SessionID
	// int			-		피격부위
	CS_GAME_CHAR_DIE,

	// client -> server
	// 위치 및 상태 동기화를 위한 패킷
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	CS_GAME_SYNC,

	// server -> client
	// 위치 및 상태 동기화를 위한 패킷
	// int			-		SessionID
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	SC_GAME_SYNC,

	// client -> server
	// 공격팀의 player가 폭탄을 설치했음
	// 데이터 없음
	CS_GAME_INSTALL_BOOM,

	// server -> client
	// 폭탄 설치를 모두에게 알림
	// int			-		SessionID
	SC_GAME_INSTALL_BOOM,


};