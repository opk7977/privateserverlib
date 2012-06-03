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
	// int			-		roomNo
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

enum LOBBY_SERVER
{
	//로비서버와의 연결을 확인
	SC_LOBBY_CONNECT_OK = 200,
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
	// int			-		ip주소 size
	// char			-		ip
	// int			-		port
	//-------------------------------------> playerCount만큼 반복으로 들어가 있음
	SC_GAME_CHARINFO_INGAME,

	// client -> server
	// 게임 시작이 가능하다는 신호
	// 데이터 없음
	CS_GAME_GAME_READY_OK,

	// server -> client
	// 게임에 모든 player가 접속했으므로 게임을 시작하라는 명령
	// 데이터 없음
	SC_GAME_START_GAME,

	// client -> server
	// 캐릭터의 공격
	// int			-		무기종류
	// int			-		피격대상 sessionID
	// int			-		감소 에너지
	CS_GAME_ATTACK,

	// server -> client
	// 캐릭터의 공격성공시 모두에게 결과를 보낸다
	// int			-		공격자SessionID
	// int			-		무기종류
	// int			-		피격대상SessionID
	SC_GAME_ATTACK,

	// server -> client
	// 공격당한 캐릭터에게 너 맞았다고 알려줌
	// int			-		공격자SessionID
	// int			-		무기종류
	// int			-		남은 에너지
	SC_GAME_YOU_ATTACKED,

	// client -> server 
	// 인간(캐릭터)가 아닌 물체같은것에 공격을 했을때
	// int			-		objID
	// float		-		PosX
	// float		-		PosY
	// float		-		PosZ
	// float		-		NormalX
	// float		-		NormalY
	// float		-		NormalZ
	CS_GAME_TRY_ATTACK,

	// server -> client
	// 인간(캐릭터)가 아닌 물체같은것에 공격을 했다는것을 알림
	// int			-		objID
	// float		-		PosX
	// float		-		PosY
	// float		-		PosZ
	// float		-		NormalX
	// float		-		NormalY
	// float		-		NormalZ
	SC_GAME_TRY_ATTACK,

	// server -> client
	// 공격당한 캐릭터가 죽으면 보내는 패킷
	// int			-		공격자SessionID
	// int			-		무기종류
	// int			-		피격대상SessionID
	SC_GAME_CHAR_DIE,

	// server -> client
	// 공격당해서 죽으면 나에게 보내는 패킷
	// int			-		공격자SessionID
	// int			-		무기종류
	SC_GAME_YOU_DIE,

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
	// 부활할 수 있는 시간이 되어 부활을 요청한다
	// int			-		부활 위치 index
	CS_GAME_ASK_REVIVAL,

	// server -> client
	// 캐릭터가 부활한다는 명령
	// int			-		SessionID
	// int			-		부활 위치 index
	SC_GAME_CHAR_REVIVAL,

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
	// 타임 아웃!/ 현재게임의 종료
	// 데이터 없음
	SC_TIME_OUT,

	// server -> client
	// 게임 종료
	// int			-		종료의 종류( 팀 승리/ 타임오버 )
	// int			-		승리 팀
	// int			-		공격팀 총Kill수( 수비팀 Death수 )
	// int			-		수비팀 총Kill수( 공격팀 Death수 )
	// int			-		공격팀 win 수
	// int			-		수비팀 win 수
	// int			-		게임 비긴 수
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