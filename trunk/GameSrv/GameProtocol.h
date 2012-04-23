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

enum GAME_SERVER
{
	// server -> client
	// ���Ӽ������� ������ ���������� �Ǿ��ٴ� ��ȣ
	SC_GAME_CONNECT_OK = 300,

	// client -> server
	// ���Ӽ����� �ڽ��� ������ ����
	// int			-		sessionID
	// int			-		roomNum
	CS_GAME_INGAME,

	// server -> client
	// ������ player�� ���ӽ��� ��ǥ���� ������ ����
	// �̹� ������ �ִ� player���� ����
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
	//-------------------------------------> playerCount��ŭ �ݺ����� �� ����
	SC_GAME_CHARINFO_INGAME,

	// server -> client
	// ���ӿ� ��� player�� ���������Ƿ� ������ �����϶�� ���
	// ������ ����
	SC_GAME_START_GAME,

	// client -> server
	// ĳ������ ���°� ����( �̵�����/���� �� )
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	CS_GAME_MOVE_CHAR,

	// server -> client
	// ĳ������ ���°� ����( �̵�����/���� �� )
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
	// ĳ������ ����
	// int			-		��������
	// int			-		�ǰݴ�� sessionID
	// int			-		�ǰݺ���
	CS_GAME_ATTECT,

	// server -> client
	// ĳ������ ���ݼ����� ��ο��� ����� ������
	// 
	// int			-		������SessionID
	// int			-		��������
	// int			-		�ǰݴ��SessionID
	// int			-		�ǰݺ���
	// int			-		���� ������
	SC_GAME_ATTECT,

	// server -> client
	// ���ݴ��� ĳ���Ͱ� ������ ������ ��Ŷ
	// int			-		������SessionID
	// int			-		��������
	// int			-		�ǰݴ��SessionID
	// int			-		�ǰݺ���
	CS_GAME_CHAR_DIE,

	// client -> server
	// ��ġ �� ���� ����ȭ�� ���� ��Ŷ
	// int			-		charState
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	CS_GAME_SYNC,

	// server -> client
	// ��ġ �� ���� ����ȭ�� ���� ��Ŷ
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
	// �������� player�� ��ź�� ��ġ����
	// ������ ����
	CS_GAME_INSTALL_BOOM,

	// server -> client
	// ��ź ��ġ�� ��ο��� �˸�
	// int			-		SessionID
	SC_GAME_INSTALL_BOOM,


};