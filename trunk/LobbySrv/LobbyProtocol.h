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
	//--------------------------------------> roomCount만큼 반복으로 들어가 있음
	SC_LOBBY_ROOMINFO,

	// client -> server
	// 방 만들기를 요청한다.
	// int			-		roomId
	// int			-		roomTitle데이터의 크기
	// TCHAR		-		roomTitle
	CS_LOBBY_CREATE_ROOM,

	// server -> client
	// 방만들기에 대한 결과를 보낸다
	// int			-		result
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
	SC_LOBBY_ROOMPLAYER_COUNTDOWN,

	// client -> server
	// ready상태를 변경하고 상태를 서버로 보냄
	// int			-		ready상태값
	CS_ROOM_CHAR_READY,

	// server -> client
	// 바뀐 ready상태를 방의 유저들에게 알림
	// int			-		SessionID
	// int			-		ready 상태
	SC_ROOM_CHAR_READY,

	// client -> server
	// 팀 변경에 대한 상태를 알림
	// int			-		Team
	CS_ROOM_TEAM_CHANGE,

	// server -> client
	// 바뀐 팀 정보를 방의 유저들에게 알림
	// int			-		SessionId
	// int			-		Team
	SC_ROOM_TEAM_CHANGE,

	// client -> server
	// 채팅을 위해 서버로 쓴글을 알림
	// int			-		문장의 데이터 크기
	// TCHAR		-		문장
	CS_LOBBY_CHAT,

	// server -> client
	// 채팅문장을 알림
	// int			-		RoomNum ( 로비라면 0 )
	// int			-		문장의 데이터 크기
	// TCHAR		-		문장
	SC_LOBBY_CHAT,

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
	// player가 접속을 끊으면 알려 준다.
	// int			-		SessionID;
	SC_LOBBY_PLAYER_DISCONNECT,

};