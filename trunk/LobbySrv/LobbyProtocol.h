#pragma once

enum SERVER_TO_SERVER
{
	// gameSrv -> lobbySrv
	// ���Ӽ����� �κ񼭹��� �ڽ��� ���Ӽ������ ���� �˸���
	// ������ ����
	GL_CONNECT_SERVER = 100,

	// lobbySrv -> gameSrv
	// ���� �������� ������ �����ϰ� �Ǿ� ���Ӽ����� �غ���¸� ���
	// int			-		roomNo
	// int			-		playerCount
	LG_START_GAME,

	// gameSrv -> lobbySrv
	// ���Ӽ������� ������ ������� player�� ���� ó��
	// int			-		roomNum
	// int			-		SessionId
	GL_PLAYER_DISCONNECT,
};

enum LOBBY_SERVER
{
	// server -> client
	// �κ� ������ ������ ���������� �Ǿ��ٴ� Ȯ�� ��Ŷ
	// ������ ����
	SC_LOBBY_CONNECT_OK = 200,

	// client -> server
	// �κ� �����ϸ鼭 �ڽ��� ������ ������ �ٸ� ������ ��û�ϴ� ��Ŷ
	// int			-		SessionId
	// int			-		RoomNum( ���ٸ� 0�� ������ )
	CS_LOBBY_INSERT_LOBBY,

	// server -> client
	// ������ �������� Ȥ�� ������ �ִ� �������� ĳ���� ������ ������ ��Ŷ
	// int			-		playerCount
	//--------------------------------------
	// int			-		SessionId
	// int			-		id�������� ũ��
	// TCHAR		-		id
	//--------------------------------------> playerCount��ŭ �ݺ����� �� ����
	SC_LOBBY_OTHER_CHARINFO,

	// server -> client
	// ������ �������� Ȥ�� ������ �ִ� �������� �濡 ���� ������ ������ ��Ŷ
	// int			-		roomCount
	//--------------------------------------
	// int			-		RoomId
	// int			-		playerCountInRoom
	// int			-		RoomTitle�������� ũ��
	// TCHAR		-		RoomTitle
	// int			-		stateOfRoom ( ex)0:�Ϲ�, 1(���):������ )
	//--------------------------------------> roomCount��ŭ �ݺ����� �� ����
	SC_LOBBY_ROOMINFO,

	// client -> server
	// �� ����⸦ ��û�Ѵ�.
	// int			-		roomId
	// int			-		roomTitle�������� ũ��
	// TCHAR		-		roomTitle
	CS_LOBBY_CREATE_ROOM,

	// server -> client
	// �游��⿡ ���� ����� ������
	// int			-		result
	// int			-		Team	//����� ���н� ������ ����
	SC_ROOM_RESULT_CREATE,

	// server -> client
	// ������ ����鿡�� ���� ����� ���ٴ� ��Ŷ�� ������.
	// int			-		roomNum
	// int			-		roomTitle������ ũ��
	// int			-		roomTitle
	SC_LOBBY_OPEN_ROOM,

	// server -> client
	// ������ ����鿡�� ���� �����ٴ� ��Ŷ�� ������
	// int			-		roomNum
	SC_LOBBY_CLOSE_ROOM,

	// client -> server
	// ������ ���� ��û�� ������ �Լ�
	// int			-		roomNum
	CS_LOBBY_INSERT_ROOM,

	// server -> client
	// �濡 ���� ��û�� �� �޾� ������ ����� �����ִ� ��Ŷ
	// int			-		result
	// int			-		Team	//�������� ������ ����
	SC_ROOM_RESULT_INSERT,

	// server -> client
	// �κ��� ����鿡�� ������ ������ ���� ��Ŷ�� ������.
	// int			-		SessionID
	// int			-		roomId
	SC_LOBBY_INSERT_ROOM,

	// server -> client
	// �濡 �ִ� ������� ������ ����
	// int			-		playerCount
	//--------------------------------------
	// int			-		SessionId
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		Team
	// int			-		Ready
	//--------------------------------------> playerCount��ŭ �ݺ�
	SC_ROOM_OTHER_CHARINFO,

	// server -> client
	// ������ ������ ���� ������ �������� ������.
	// int			-		SessionId
	SC_ROOM_LEADER,

	// client -> server
	// �濡�� ���� �κ� ���°��� �˸�
	// ������ ����
	CS_ROOM_OUT_ROOM,

	// server -> client
	// �濡 ����鿡�� ������ ���� �������� �˸�
	// int			-		SessionId
	SC_ROOM_CHAR_OUT,

	// server -> client
	// �κ� �ִ� ����鿡�� �濡 ��� ���� �پ��ٴ� ���� �˸�
	// int			-		roomNum
	SC_LOBBY_ROOMPLAYER_COUNTDOWN,

	// client -> server
	// ready���¸� �����ϰ� ���¸� ������ ����
	// int			-		ready���°�
	CS_ROOM_CHAR_READY,

	// server -> client
	// �ٲ� ready���¸� ���� �����鿡�� �˸�
	// int			-		SessionID
	// int			-		ready ����
	SC_ROOM_CHAR_READY,

	// client -> server
	// �� ���濡 ���� ���¸� �˸�
	// int			-		Team
	CS_ROOM_TEAM_CHANGE,

	// server -> client
	// �ٲ� �� ������ ���� �����鿡�� �˸�
	// int			-		SessionId
	// int			-		Team
	SC_ROOM_TEAM_CHANGE,

	// client -> server
	// ä���� ���� ������ ������ �˸�
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	CS_LOBBY_CHAT,

	// server -> client
	// ä�ù����� �˸�
	// int			-		RoomNum ( �κ��� 0 )
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	SC_LOBBY_CHAT,

	// client -> server
	// ������ ������ �����Ϸ��� �˸��� ��Ŷ
	// ������ ����
	CS_ROOM_START,

	// server -> client
	// ������ ������ �� �ִ����� ���� ���
	// BOOL			-		�����
	SC_ROOM_START_RESULT,

	// server -> client
	// ���� ������ Ŭ�󿡰� �˸���.
	// int			-		ip������ũ��
	// TCHAR		-		ip
	// int			-		port��ȣ
	SC_ROOM_GAME_START,

	// server -> client
	// �κ� �ִ� player�鿡�� ������ ���۵� ���� ��ȣ�� �˸���
	// int			-		���ȣ
	SC_LOBBY_GAME_START,

	// server -> client
	// player�� ������ ������ �˷� �ش�.
	// int			-		SessionID;
	SC_LOBBY_PLAYER_DISCONNECT,

};