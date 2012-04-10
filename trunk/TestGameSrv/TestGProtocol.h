#pragma once

enum TestGame
{
	//--------------------------------------
	// server -> client
	// 접속확인 
	// 정보 없음
	//--------------------------------------
	SC_TGAME_CONNECT_OK = 0,

	//--------------------------------------
	// client -> server
	// 해당 방의 정보와 함께 접속 데이터를 보내옴
	// int				-			charSessionID
	// int				-			RoomNum
	// int				-			PlayerCount
	//--------------------------------------
	CS_TGAME_SENDINFO,

	//--------------------------------------
	// server -> client
	// 캐릭터의 위치를 알려 줌
	// int				-			characterCount
	//--------------------------------------
	// int				-			charSessionID
	// int				-			charState
	// float			-			charDirX
	// float			-			charDirY
	// float			-			charDirZ
	// float			-			charPosX
	// float			-			charPosY
	// float			-			charPosZ
	//--------------------------------------
	SC_TGAME_CREATECHAR,

	//--------------------------------------
	// server -> client
	// 해당 게임 프로세스의 인원만큼 접속이 완료 되어 게임을 시작하라는 명령을 내림
	//--------------------------------------
	SC_TGAME_STARTGAME,

	//--------------------------------------
	// client -> server
	// 캐릭터 생성패킷을 받아 캐릭터 생성을 완료 했다는 패킷
	// int				-			GameNo
	//--------------------------------------
	CS_TGAME_READYOK,

	//--------------------------------------
	// server -> client
	// 클라에게 시간의 경과를 알린다
	//--------------------------------------
	SC_TGAME_COUNTDWON_TIME,

	//--------------------------------------
	// client -> server
	// 회전에 대한 패킷, 자신의 상태변화를 알린다.
	// int				-			charSessionID
	// int				-			charState
	//--------------------------------------
	CS_TGAME_STATEROTATE,

	//--------------------------------------
	// server -> client
	// 회전에 대한 패킷, 변화한 player의 정보를 모두에게 알린다.
	// int				-			charSessionID
	// int				-			charState
	//--------------------------------------
	SC_TGAME_STATEROTATE,

	//--------------------------------------
	// client -> server
	// 움직임에 대한 패킷, 자신의 상태변화를 알린다.
	// int				-			charSessionID
	// int				-			charState
	// float			-			charDirX
	// float			-			charDirY
	// float			-			charDirZ
	// float			-			charPosX
	// float			-			charPosY
	// float			-			charPosZ
	//--------------------------------------
	CS_TGAME_STATECHANGE,

	//--------------------------------------
	// server -> client
	// 움직임에 대한 패킷, 변화한 player의 정보를 모두에게 알린다.
	// int				-			charSessionID
	// int				-			charState
	// float			-			charDirX
	// float			-			charDirY
	// float			-			charDirZ
	// float			-			charPosX
	// float			-			charPosY
	// float			-			charPosZ
	//--------------------------------------
	SC_TGAME_STATECHANGE,

	//--------------------------------------
	// server -> client
	// 게임 종료 패킷
	//--------------------------------------
	SC_TGAME_ENDGAME,

	//--------------------------------------
	// server -> client
	// 누군가 종료를 해서 캐릭터를 삭제 함
	// int				-			charSessionID
	//--------------------------------------
	SC_TGAME_DELETECHAR,

};

