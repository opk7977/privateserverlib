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
	// -10(����) : ����/ 1(���) : ����
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
	// ����� -10: ����, -5: �̹� �α��εǾ� ����, -1: id����/ 0: ���Ʋ��/ n(���): ����(sessionID)
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
	// int			-		roomTitle�������� ũ��
	// TCHAR		-		roomTitle
	CS_LOBBY_CREATE_ROOM,

	// server -> client
	// �游��⿡ ���� ����� ������
	// int			-		result ( -1:����/ n(���) ������� ���� ID )
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
	CS_ROOM_CHAR_READY,

	// server -> client
	// �ٲ� ready���¸� ���� �����鿡�� �˸�
	// int			-		SessionID
	// int			-		ready ����
	SC_ROOM_CHAR_READY,

	// client -> server
	// �� ���濡 ���� ���¸� �˸�
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