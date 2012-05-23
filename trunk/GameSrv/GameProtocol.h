#pragma once

enum SERVER_CODE
{
	// ���� ���� �� ���� ��ü���� ������
	// �������� ó���� ���� ���Ѱ��
	// ��������� �� ���� �� �ִ�.
	SERVER_ERROR = -10,
	// ������ �����
	SUCCESSED_ACK = 1,

	//--------------------------------------------------------------

	// IDüũ ����� ID�� �ߺ��ɶ�
	OVERLAPPED_ID = -1,

	// �α��� ����� �ش� ID�� �̹� �α��� �߿� ������
	PRE_LOGIN_ID = -5,
	// �α��� ����� ID�� ����
	NONEXISTENT_ID = -1,
	// �α��� ����� PW�� Ʋ��
	WRONG_PW = 0,

	//--------------------------------------------------------------

	// �游��� ����( ���̻� ���� ���� �� ���� )
	ROOMCOUNT_IS_FULL = -1,

	// �� ����� ���� �������̸�
	ROOM_IS_PLAY_NOW = -1,
	// �� ����� ���� �ο� �ʰ�
	ROOM_PLAYERCOUNT_FULL = 0,

	//--------------------------------------------------------------

	// ���� ������ ����
	// �ð� �ʰ�
	GAME_END_TIMEATTECT = 1,
	// �� ����
	GAME_END_TEAM_ALL_DIE,
	// �� �̼��� ����
	GAME_END_MISSION_SUCCESS,
};

enum SERVER_TO_SERVER
{
	// gameSrv -> lobbySrv
	// ���Ӽ����� �κ񼭹��� �ڽ��� ���Ӽ������ ���� �˸���
	// ������ ����
	GL_CONNECT_SERVER = 100,

	// lobbySrv -> gameSrv
	// ���� �������� ������ �����ϰ� �Ǿ� ���Ӽ����� �غ���¸� ���
	// int			-		roomNum
	// int			-		playerCount
	//--------------------------------------------------------------
	// int			-		SessionId
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		Team
	//--------------------------------------------------------------playerCount��ŭ �ݺ����� �� ����
	LG_START_GAME,

	// gameSrv -> lobbySrv
	// ���� proc�� ������ ���ų� ���⿡ �����ߴ�
	// int			-		roomNum
	GL_START_FAILD,

	// gameSrv -> lobbySrv
	// ���� ������ ������ ���´� ��ȣ�� ������� ��Ŷ
	// int			-		roomNo
	GL_START_OK,

	// gameSrv -> lobbySrv
	// ������ ����ǰ� ���� �ٽ� �Ϲ� ���·� �ٲ� �ش�
	// int			-		roomNum
	GL_GAME_END,

	// gameSrv -> lobbySrv
	// ���Ӽ������� ������ ������� player�� ���� ó��
	// int			-		roomNum
	// int			-		SessionId
	GL_PLAYER_DISCONNECT,
};

enum LOBBY_SERVER
{
	//�κ񼭹����� ������ Ȯ��
	SC_LOBBY_CONNECT_OK = 200,
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
	// float		-		DirZ
	// int			-		DirInt
	CS_GAME_MOVE_CHAR,

	// server -> client
	// ĳ������ ���°� ����( �̵�����/���� �� )
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
	SC_GAME_CHAR_DIE,

	// client -> server
	// ��ġ �� ���� ����ȭ�� ���� ��Ŷ
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
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	// float		-		DirX
	// float		-		DirY
	// float		-		DirZ
	SC_GAME_SYNC,

	// client -> server
	// ĳ���� ���°� ����
	// int			-		dirInt
	CS_GAME_CHANGE_STATE,

	// server -> client
	// ĳ���Ͱ� ��ȯ�� ���¸� ��ο��� �˸���.
	// int			-		sessionId
	// int			-		dirInt
	SC_GAME_CHANGE_STATE,

	// client -> server
	// ĳ���� ȸ��
	// float		-		dirX
	// float		-		dirY
	// float		-		dirZ
	CS_GAME_ROTATION,

	// server -> client
	// ĳ���� ȸ���� ��ο��� �˸���
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
	// �������� player�� ��ź�� ��ġ����
	// ������ ����
	CS_GAME_INSTALL_BOOM,

	// server -> client
	// ��ź ��ġ�� ��ο��� �˸�
	// int			-		SessionID
	SC_GAME_INSTALL_BOOM,

	// server -> client
	// ��ź�� ī��Ʈ �ٿ�
	// ������ ����( or int  - ���� ī��Ʈ )
	SC_GAME_BOOM_COUNTDOWN,

	// client -> server
	// �������� ��ź�� ������
	// ������ ����
	CS_GAME_UNINSTALL_BOOM,

	// server -> client
	// ��ź�� ���ŵǾ����� �˸�
	// int			-		sesseionID
	SC_GAME_UNINSTALL_BOOM,

	// server -> client
	// ������ �ð� ����
	// ������ ����( or int	- ���� ī��Ʈ )
	SC_GAME_TIME_COUNTDOWN,

	// server -> client
	// ���� ����
	// int			-		������ ����( �� �¸�/ Ÿ�ӿ��� )
	// int			-		�¸� ��
	// int			-		������ Kill��( ������ Death�� )
	// int			-		������ Kill��( ������ Death�� )
	SC_GAME_END,

	// client -> server
	// ������ ��ġ
	// int			-		item����
	// float		-		posX
	// float		-		poaY
	// float		-		posZ
	CS_GAME_INSTALL_ITEM,

	// server -> client
	// �������� ��ġ�� ��ο��� �˸�
	// int			-		��ġ���� Team
	// int			-		item����
	// int			-		item ID
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	SC_GAME_INSTALL_ITEM,

	// client -> server
	// �������� ������
	// int			-		item�� ���ݴ��� player�� sessionId
	// int			-		item ID
	// int			-		������
	SC_GAME_RUN_ITEM,

	// client -> server
	// ������ ������ ������ �̵��ϱ� ���� ���� ��û
	// ������ ����
	CS_GAME_GOTO_LOBBY,

	// server -> client
	// player�� �κ�� �̵�
	// int			-		ip�������� ũ��
	// char			-		ip
	// int			-		port
	SC_GAME_GOTO_LOBBY,

	// server -> client
	// player�� �κ�� �̵��ߴٰ� �˸�
	// int			-		SessionID
	SC_GAME_CHAR_GOTO_LOBBY,

	// server -> client
	// player�� ������ ���� ������ �������� ����鿡�� �˸�
	// int			-		SessionID
	SC_GAME_CHAR_DISCONNECT,
};