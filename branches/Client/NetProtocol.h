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
	SC_LOBBY_ROOMPLAYER_COUNTDOWN,

	// client -> server
	// ready상태를 변경하고 상태를 서버로 보냄
	CS_ROOM_CHAR_READY,

	// server -> client
	// 바뀐 ready상태를 방의 유저들에게 알림
	// int			-		SessionID
	// int			-		ready 상태
	SC_ROOM_CHAR_READY,

	// client -> server
	// 팀 변경에 대한 상태를 알림
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
	// float		-		DirZ
	// int			-		DirInt
	CS_GAME_MOVE_CHAR,

	// server -> client
	// 캐릭터의 상태가 변함( 이동시작/멈춤 등 )
	// int			-		sessionID
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirZ
	// int			-		DirInt
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
	SC_GAME_CHAR_DIE,

	// client -> server
	// 위치 및 상태 동기화를 위한 패킷
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
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	SC_GAME_SYNC,

	// client -> server
	// 캐릭터 상태가 변함
	// int			-		dirInt
	CS_GAME_CHANGE_STATE,

	// server -> client
	// 캐릭터가 전환된 상태를 모두에게 알린다.
	// int			-		sessionId
	// int			-		dirInt
	SC_GAME_CHANGE_STATE,

	// client -> server
	// 캐릭터 회전
	// float		-		dirX
	// float		-		dirY
	// float		-		dirZ
	CS_GAME_ROTATION,

	// server -> client
	// 캐릭터 회전을 모두에게 알린다
	// int			-		sessionId
	// float		-		dirX
	// float		-		dirY
	// float		-		dirZ
	SC_GAME_ROTATION,

	// client -> server
	// int			-		stringSize
	// TCHAR		-		chattingString
	CS_GAME_CHATTING,

	// server -> client
	// int			-		stringSize
	// TCHAR		-		chattingString
	SC_GAME_CHATTING,

	// client -> server
	// 공격팀의 player가 폭탄을 설치했음
	// 데이터 없음
	CS_GAME_INSTALL_BOOM,

	// server -> client
	// 폭탄 설치를 모두에게 알림
	// int			-		SessionID
	SC_GAME_INSTALL_BOOM,

	// server -> client
	// 폭탄의 카운트 다운
	// 데이터 없음( or int  - 남은 카운트 )
	SC_GAME_BOOM_COUNTDOWN,

	// client -> server
	// 수비팀이 폭탄을 제거함
	// 데이터 없음
	CS_GAME_UNINSTALL_BOOM,

	// server -> client
	// 폭탄이 제거되었음을 알림
	// int			-		sesseionID
	SC_GAME_UNINSTALL_BOOM,

	// server -> client
	// 게임중 시간 어택
	// 데이터 없음( or int	- 남은 카운트 )
	SC_GAME_TIME_COUNTDOWN,

	// server -> client
	// 게임 종료
	// int			-		종료의 종류( 팀 승리/ 타임오버 )
	// int			-		승리 팀
	// int			-		공격팀 Kill수( 수비팀 Death수 )
	// int			-		수비팀 Kill수( 공격팀 Death수 )
	SC_GAME_END,

	// client -> server
	// 아이템 설치
	// int			-		item종류
	// float		-		posX
	// float		-		poaY
	// float		-		posZ
	CS_GAME_INSTALL_ITEM,

	// server -> client
	// 아이템의 설치를 모두에게 알림
	// int			-		설치자의 Team
	// int			-		item종류
	// int			-		item ID
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	SC_GAME_INSTALL_ITEM,

	// client -> server
	// 아이템이 동작함
	// int			-		item에 공격당한 player의 sessionId
	// int			-		item ID
	// int			-		데미지
	SC_GAME_RUN_ITEM,

	// client -> server
	// 게임이 끝나고 방으로 이동하기 위해 정보 요청
	// 데이터 없음
	CS_GAME_GOTO_LOBBY,

	// server -> client
	// player를 로비로 이동
	// int			-		ip데이터의 크기
	// char			-		ip
	// int			-		port
	SC_GAME_GOTO_LOBBY,

	// server -> client
	// player가 로비로 이동했다고 알림
	// int			-		SessionID
	SC_GAME_CHAR_GOTO_LOBBY,

	// server -> client
	// player가 게임을 끄고 접속을 끊었을때 사람들에게 알림
	// int			-		SessionID
	SC_GAME_CHAR_DISCONNECT,
};