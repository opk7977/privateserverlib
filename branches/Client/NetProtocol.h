#pragma once

enum LOGIN_SERVER
{
	// server -> client
	// �α��� ������ ������ ���������� �Ǿ��ٴ� Ȯ�� ��Ŷ
	// ������ ����
	SC_LOGIN_CONNECT_OK = 10,

	// client -> server
	// ���������� ���� id�ߺ� Ȯ���� ���� ��Ŷ
	// int			-		id������ ũ��
	// TCHAR		-		id
	CS_LOGIN_CHECK_ID,

	// server -> client
	// id�ߺ� üũ�� ���� ������� return �� �ش�
	// -1(����) : �ߺ�/ 1(���) : ��밡��
	// int			-		�����
	SC_LOGIN_CHECK_ID_RESULT,

	// client -> server
	// ������ ����µ� �ʿ��� ������ �޴´�
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		password�������� ũ��
	// TCHAR		-		password
	// int			-		e-mail �������� ũ��
	// TCHAR		-		e-mail
	CS_LOGIN_CREATE_ACCOUNT,

	// server -> client
	// ���� ���� ����⿡ ���� ���
	// -1(����) : ����/ 1(���) : ����
	// int			-		�����
	SC_LOGIN_CREATE_RESULT,

	// client -> server
	// ������ �������� �α��� �õ�
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		password�������� ũ��
	// TCHAR		-		password
	CS_LOGIN_LOGIN,

	// server -> client
	// �α��� ����� return�Ѵ�
	// ����� -1(����): id����/ 0: ���Ʋ��/ n(���): ����(sessionID)
	// --------����� �����̸� �߰��� data--------------------------
	// int			-		ip�ּ��� ������ ũ��
	// char			-		ip�ּ�
	// int			-		port��ȣ
	SC_LOGIN_LOGIN_RESULT,
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
	// int			-		id�� ������ ũ��
	// TCHAR		-		id
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
	// �濡 �ִ� ����鿡�� �濡 ���� ����� ���� ������ ������.
	// int			-		SessionID
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		Team
	SC_ROOM_CHAR_INSERT,

	// server -> client
	// �濡 �ִ� ������� ������ ����
	// int			-		playerCount
	//--------------------------------------
	// int			-		SessionId
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		Team
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

	// client -> server
	// ready���¸� �����ϰ� ���¸� ������ ����
	// int			-		ready���°�
	CS_ROOM_READY,

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
	// ���� �������� ���� �κ񼭹����� ������ �����ϱ� ���� ������ ��ȣ
	// ������ ����
	//CS_ROOM_PLAY,

	// client -> server
	// ä���� ���� ������ ������ �˸�
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	CS_ROOM_CHAT,

	// server -> client
	// ä�ù����� �˸�
	// int			-		SessionID
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	SC_ROOM_CHAT,

	// server -> client
	// player�� ������ ������ �˷� �ش�.
	// int			-		SessionID;
	SC_LOBBY_PLAYER_DISCONNECT,

};
