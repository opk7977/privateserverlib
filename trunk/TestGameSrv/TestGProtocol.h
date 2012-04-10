#pragma once

enum TestGame
{
	//--------------------------------------
	// server -> client
	// ����Ȯ�� 
	// ���� ����
	//--------------------------------------
	SC_TGAME_CONNECT_OK = 0,

	//--------------------------------------
	// client -> server
	// �ش� ���� ������ �Բ� ���� �����͸� ������
	// int				-			charSessionID
	// int				-			RoomNum
	// int				-			PlayerCount
	//--------------------------------------
	CS_TGAME_SENDINFO,

	//--------------------------------------
	// server -> client
	// ĳ������ ��ġ�� �˷� ��
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
	// �ش� ���� ���μ����� �ο���ŭ ������ �Ϸ� �Ǿ� ������ �����϶�� ����� ����
	//--------------------------------------
	SC_TGAME_STARTGAME,

	//--------------------------------------
	// client -> server
	// ĳ���� ������Ŷ�� �޾� ĳ���� ������ �Ϸ� �ߴٴ� ��Ŷ
	// int				-			GameNo
	//--------------------------------------
	CS_TGAME_READYOK,

	//--------------------------------------
	// server -> client
	// Ŭ�󿡰� �ð��� ����� �˸���
	//--------------------------------------
	SC_TGAME_COUNTDWON_TIME,

	//--------------------------------------
	// client -> server
	// ȸ���� ���� ��Ŷ, �ڽ��� ���º�ȭ�� �˸���.
	// int				-			charSessionID
	// int				-			charState
	//--------------------------------------
	CS_TGAME_STATEROTATE,

	//--------------------------------------
	// server -> client
	// ȸ���� ���� ��Ŷ, ��ȭ�� player�� ������ ��ο��� �˸���.
	// int				-			charSessionID
	// int				-			charState
	//--------------------------------------
	SC_TGAME_STATEROTATE,

	//--------------------------------------
	// client -> server
	// �����ӿ� ���� ��Ŷ, �ڽ��� ���º�ȭ�� �˸���.
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
	// �����ӿ� ���� ��Ŷ, ��ȭ�� player�� ������ ��ο��� �˸���.
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
	// ���� ���� ��Ŷ
	//--------------------------------------
	SC_TGAME_ENDGAME,

	//--------------------------------------
	// server -> client
	// ������ ���Ḧ �ؼ� ĳ���͸� ���� ��
	// int				-			charSessionID
	//--------------------------------------
	SC_TGAME_DELETECHAR,

};

