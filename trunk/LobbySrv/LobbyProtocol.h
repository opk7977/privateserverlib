#pragma once

enum SERVER_CODE
{
	// ���� ���� �� ���� ��ü���� ������
	// �������� ó���� ���� ���Ѱ��
	// ��������� �� ���� �� �ִ�.
	SERVER_ERROR			= -10,
	// ������ �����
	SUCCESSED_ACK			= 1,

	//--------------------------------------------------------------

	// IDüũ ����� ID�� �ߺ��ɶ�
	OVERLAPPED_ID			= -1,

	// �α��� ����� �ش� ID�� �̹� �α��� �߿� ������
	PRE_LOGIN_ID			= -5,
	// �α��� ����� ID�� ����
	NONEXISTENT_ID			= -1,
	// �α��� ����� PW�� Ʋ��
	WRONG_PW				= 0,

	//--------------------------------------------------------------

	// ���̻� �α��� �� �� �����ϴ�
	CHARAVER_SPACE_IS_FULL	= -20,
	
	//--------------------------------------------------------------
	
	// �游��� ����( ���̻� ���� ���� �� ���� )
	ROOMCOUNT_IS_FULL		= -1,
	
	// �� ����� ���� �������̸�
	ROOM_IS_PLAY_NOW		= -1,

	// �� ����� ���� �ο� �ʰ�
	ROOM_PLAYERCOUNT_FULL	= 0,

	//--------------------------------------------------------------

	// ���� ������ ����
	// �ð� �ʰ�
	GAME_END_TIMEATTECT		= 1,
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
	// int			-		roomNum
	GL_START_OK,

	// gameSrv -> lobbySrv
	// ������ ����ǰ� ���� �ٽ� �Ϲ� ���·� �ٲ� �ش�
	// int			-		roomNum
	GL_GAME_END,

	// gameSrv -> lobbySrv
	// ���Ӽ������� ������ ������� player�� ���� ó��
	// int			-		roomNum
	// int			-		SessionId
	// int			-		Team����
	GL_PLAYER_DISCONNECT,
};

enum DB_SERVER
{
	// DBSrv -> otherSrv
	// ������ ����� �Ǿ��ٴ� ��ȣ
	// ������ ����
	DB_TO_OTHER_CONNECT_OK = 1000,

	// loginSrv -> DBSrv
	// id�ߺ� üũ
	// int			-		�ӽ� index
	// int			-		id������ ũ��
	// TCHAR		-		id
	LOGIN_TO_DB_CHECK_ID,

	// DBSrv -> loginSrv
	// id�ߺ�üũ ����� ������.
	// int			-		�ӽ� index
	// BOOL			-		result(��밡��:TRUE/���Ұ�:FALSE)
	DB_TO_LOGIN_CHECK_ID_RESULT,

	// loginSrv -> DBSrv
	// ���������� ��û�Ѵ�
	// int			-		�ӽ� index
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		pw�������� ũ��
	// TCHAR		-		pw
	// int			-		email�������� ũ��
	// TCHAR		-		email
	LOGIN_TO_DB_CREATE_ACCOUNT,

	// DBSrv -> loginSrv
	// ���������� ���� ����� ������
	// BOOL			-		result( ����:TRUE/ ����:FALSE )
	DB_TO_LOGIN_CREATE_ACCOUNT_RESULT,

	// loginSrv -> DBSrv
	// �α���
	// int			-		�ӽ� index
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		pw�������� ũ��
	// TCHAR		-		pw
	LOGIN_TO_DB_TRYLOGIN,

	// DBSrv -> lobbySrv
	// �α����� ��ٸ� lobby������ �� user�� data�� ������
	// int			-		�ӽ� index
	// int			-		sessionId
	// int			-		id������ ũ��
	// TCHAR		-		id
	// int			-		Rank_ID
	// int			-		Rank_Point
	// int			-		AccumulatedKillPoint
	// int			-		AccumulatedDeathPoint
	DB_TO_LOBBY_CHARACTER_LOGIN,

	// lobbySrv -> DBSrv
	// �κ� ĳ���� ������ �غ� �Ǿ��ٴ� ��ȣ�� �κ� ��񿡰� ����
	// int			-		�ӽ� index
	// int			-		sessionID
	// int			-		���
	//						( ���������� �غ� �Ϸ� : 1
	//						  ĳ���Ͱ� ���̻� ���� �� �� ���� : -10 )
	LOBBY_TO_DB_CHARINSERT_READY_RESULT,

	// DBSrv -> loginSrv
	// �α��� ����� ������
	// int			-		�ӽ� index
	// int			-		result( �̷̹α�����: PRE_LOGIN_ID(-5)
	//							  / ���� ���̵� : NONEXISTENT_ID(-1)
	//							  / ��� Ʋ��   : WRONG_PW(0)
	//							  / ����        : n(�ڽ���sessionID) )
	DB_TO_LOGIN_LOGINRESULT,

	// DBSrv -> otherSrv
	// �ߺ��� id�� �α��� �˴ϴ�. ������ ���ӵ� user�� ������ �����ϴ�.
	// int			-		sessionID
	DB_TO_OTHER_DROP_PLAYER,

	// gameSrv -> DBSrv
	// ������ ������ ĳ���͵��� ������ �����Ѵ�
	// int			-		game(room)Num
	// int			-		Playercount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_point	// ������ point
	// int			-		kill		// ������ killcount
	// int			-		death		// ������ deathcount
	//--------------------------------------> playerCount��ŭ �ݺ�
	GAME_TO_DB_UPDATE_USERDATA,

	// DBSrv -> GameSrv
	// ĳ���͵��� ������ �����ϰ� ���ŵ� ĳ������ ������ ���� ������ �޴´�
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	//--------------------------------------
	DB_TO_GAME_LEVEL_UP_PLAYER,

	// DBSrv -> lobbySrv
	// ĳ���� ������ ���Ǿ� ���ŵǸ� �κ� ���ο� �����͸� �˸���
	// int			-		game(room)Num
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	// int			-		rank_point	// ������ point
	// int			-		kill		// ������ killcount
	// int			-		death		// ������ deathcount
	//--------------------------------------> playerCount��ŭ �ݺ�
	DB_TO_LOBBY_UPDATE_USERDATA,

	// otherSrv -> DBSrv
	// ĳ���Ͱ� ������ �������ϴ�(������ ����)
	// int			-		sessionId
	OTHER_TO_DB_DISCONNECT_CHARACTER,
};

enum GAME_SERVER
{
	// server -> client
	// ���Ӽ������� ������ ���������� �Ǿ��ٴ� ��ȣ
	SC_GAME_CONNECT_OK = 300,
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
	// int			-		Room(0:�κ�/ n:���ȣ)
	// int			-		Playstate
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
	// int			-		stageMap
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
	//--------------------------------------> retult�� �����̸� �Ʒ� �����Ͱ� �´�
	// int			-		������ ������ ����
	// TCHAR		-		������
	// int			-		stageMap
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
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
	// int			-		result( -1: ����/ 1: ���� )
	SC_ROOM_RESULT_INSERT,

	// server -> client
	// �濡 ���� ��û�� �� �޾� ������ ����� �����ִ� ��Ŷ
	// int			-		result
	//--------------------------------------> retult�� �����̸� �Ʒ� �����Ͱ� �´�
	// int			-		������ ������ ����
	// TCHAR		-		������
	// int			-		stageMap
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
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
	// �濡�� ���������� ������������ �޴´�
	// ������ ����
	SC_ROOM_OUT_RESULT,

	// server -> client
	// �濡 ����鿡�� ������ ���� �������� �˸�
	// int			-		SessionId
	SC_ROOM_CHAR_OUT,

	// server -> client
	// �κ� �ִ� ����鿡�� �濡 ��� ���� �پ��ٴ� ���� �˸�
	// int			-		roomNum
	// int			-		sessionId
	SC_LOBBY_ROOM_PLAYER_OUT,

	// client -> server
	// ready���¸� �����ϰ� ���¸� ������ ����
	CS_ROOM_CHAR_READY,

	// server -> client
	// �ٲ� ready���¸� ���� �����鿡�� �˸�
	// int			-		SessionID
	// int			-		ready ����
	SC_ROOM_CHAR_READY,

	// client -> server
	// ���� ����
	// int			-		stageMap
	CS_ROOM_MAP_CHANGE,

	// server -> client
	// ����� ���� �˸�
	// int			-		stageMap
	SC_ROOM_MAP_CHANGE,

	// server -> client
	// int			-		roomNum
	// int			-		stageMap
	SC_LOBBY_MAP_CHANGE,

	// client -> server
	// ������ Mode�� ����
	// int			-		mapNum
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
	CS_ROOM_MODE_CHANGE,

	// server -> client
	// ����� Mode�� ����
	// int			-		mapNum
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
	SC_ROOM_MODE_CHANGE,

	// server -> client
	// �κ� ���� ����� �濡 ���� ������ �˸�
	// int			-		roomnum
	// int			-		mapnum
	// int			-		gameMode
	// int			-		playTime
	// int			-		playCount
	SC_LOBBY_MODE_CHANGE,

	// client -> server
	// �� ���濡 ���� ���¸� �˸�
	CS_ROOM_TEAM_CHANGE,

	// server -> client
	// �ٲ� �� ������ ���� �����鿡�� �˸�
	// int			-		SessionId
	// int			-		Team
	SC_ROOM_TEAM_CHANGE,

	// client -> server
	// ��ü ä��
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	CS_LOBBYSRV_ALL_CHAT,

	// server -> client
	// ��ü ä��
	// int			-		0:�κ�/ n: ��
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	SC_LOBBYSRV_ALL_CHAT,

	// client -> server
	// �Ӹ� ����
	// int			-		���� ID�ǵ����� ũ��
	// TCHAR		-		������ ID
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	CS_LOBBYSRV_TARGET_CHAT,

	// server -> client
	// �Ӹ� ����
	// int			-		0: �κ�/ n: ���ȣ
	// int			-		������ ������ ũ��
	// TCHAR		-		����
	SC_LOBBYSRV_TARGET_CHAT,

	// server -> client
	// ���忡�� ���� start��ư�� Ȱ��ȭ �ϵ��� �˷���
	// ������ ����
	SC_ROOM_START_VISIBLE,

	// server -> client
	// ���忡�� start��ư�� ��Ȱ��ȭ �ϵ��� �˷���
	// ������ ����
	SC_ROOM_START_INVISIBLE,

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
	// �κ� �ִ� player�鿡�� ������ ���� ���� ��ȣ�� �˸���.
	// int			-		���ȣ
	SC_LOBBY_GAME_END,

	// server -> client
	// player(��)�� �κ񿡼� ������ ������ �˷� �ش�.
	// int			-		SessionID;
	SC_LOBBY_PLAYER_DISCONNECT,
};