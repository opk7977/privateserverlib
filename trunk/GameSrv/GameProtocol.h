#pragma once

#define CONNECT_LOG_SERVER

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
	// int			-		Team����
	GL_PLAYER_DISCONNECT,
};

enum LOBBY_SERVER
{
	//�κ񼭹����� ������ Ȯ��
	SC_LOBBY_CONNECT_OK = 200,
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
	// int			-		game(room)Num
	// int			-		playerCount
	//--------------------------------------
	// int			-		sessionID
	// int			-		rank_id
	//--------------------------------------
	DB_TO_GAME_LEVEL_UP_PLAYER,

	// DBSrv -> lobbySrv
	// ĳ���� ������ ���Ǿ� ���ŵǸ� �κ� ���ο� �����͸� �˸���
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

enum LOG_SERVER
{
	//========================================================
	// �α׼��� -> �ٸ�����
	// �α� ������ ���������� ���ӵǾ���
	//--------------------------------------------------------
	// ������ ����
	//========================================================
	LOG_SERVER_CONNECT_OK = 10000,

	//========================================================
	// �ٸ����� -> �α׼���
	// �α� ������ �Ѿ� ���� ��� ��Ŷ�� ������ ID�� ���´�
	//--------------------------------------------------------
	// int			-			ServerID
	// int			-			LogLevel
	// int			-			LogDataLenth
	// TCHAR		-			LogString
	//========================================================
	RECV_SERVER_LOG,

	//========================================================
	// �α׼��� -> �ٸ�����
	// �α� ���� �� ����
	//--------------------------------------------------------
	// ������ ����
	//========================================================
	LOG_SERVER_SHOTDOWN,
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
	// int			-		ip�ּ� size
	// char			-		ip
	// int			-		port
	//-------------------------------------> playerCount��ŭ �ݺ����� �� ����
	SC_GAME_CHARINFO_INGAME,

	// client -> server
	// ���� ������ �����ϴٴ� ��ȣ
	// ������ ����
	CS_GAME_GAME_READY_OK,

	// server -> client
	// ���ӿ� ��� player�� ���������Ƿ� ������ �����϶�� ���
	// ������ ����
	SC_GAME_START_GAME,

	// client -> server
	// ĳ������ ��ġ ������ �޴´�.
	// int			-		sessionID
	// float		-		posX
	// float		-		posY
	// float		-		posZ
	CS_GAME_CHARACTER_SYNC,

	// client -> server
	// ĳ������ ����
	// int			-		��弦?
	// int			-		�ǰݴ�� sessionID
	// int			-		���� ������
	// float		-		PosX
	// float		-		PosY
	// float		-		PosZ
	// float		-		NormalX
	// float		-		NormalY
	// float		-		NormalZ
	CS_GAME_ATTACK,

	// server -> client
	// ĳ������ ���ݼ����� ��ο��� ����� ������
	// int			-		��弦?
	// int			-		������SessionID
	// int			-		�ǰݴ��SessionID
	// float		-		PosX
	// float		-		PosY
	// float		-		PosZ
	// float		-		NormalX
	// float		-		NormalY
	// float		-		NormalZ
	SC_GAME_ATTACK,

	// server -> client
	// ���ݴ��� ĳ���Ϳ��� �� �¾Ҵٰ� �˷���
	// int			-		��弦?
	// int			-		������SessionID
	// int			-		���� ������
	SC_GAME_YOU_ATTACKED,

	// client -> server 
	// �ΰ�(ĳ����)�� �ƴ� ��ü�����Ϳ� ������ ������
	// int			-		objID
	// float		-		PosX
	// float		-		PosY
	// float		-		PosZ
	// float		-		NormalX
	// float		-		NormalY
	// float		-		NormalZ
	CS_GAME_TRY_ATTACK,

	// server -> client
	// �ΰ�(ĳ����)�� �ƴ� ��ü�����Ϳ� ������ �ߴٴ°��� �˸�
	// int			-		objID
	// float		-		PosX
	// float		-		PosY
	// float		-		PosZ
	// float		-		NormalX
	// float		-		NormalY
	// float		-		NormalZ
	SC_GAME_TRY_ATTACK,

	// client -> server
	// �׳� ���� ��
	CS_GAME_JUST_SHOOT,

	// server -> client
	// �׳� ���� ��
	// int			-		sessionID
	SC_GAME_JUST_SHOOT,

	// server -> client
	// ���ݴ��� ĳ���Ͱ� ������ ������ ��Ŷ
	// int			-		��弦?
	// int			-		������SessionID
	// int			-		�ǰݴ��SessionID
	SC_GAME_CHAR_DIE,

	// server -> client
	// ���ݴ��ؼ� ������ ������ ������ ��Ŷ
	// int			-		��弦?
	// int			-		������SessionID
	SC_GAME_YOU_DIE,

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
	// ��Ȱ�� �� �ִ� �ð��� �Ǿ� ��Ȱ�� ��û�Ѵ�
	// int			-		��Ȱ ��ġ index
	CS_GAME_ASK_REVIVAL,

	// server -> client
	// ĳ���Ͱ� ��Ȱ�Ѵٴ� ���
	// int			-		SessionID
	// int			-		��Ȱ ��ġ index
	SC_GAME_CHAR_REVIVAL,

	// client -> server
	// int			-		All(0)/ Team(1)
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
	// �����ð����� hp�� �÷� �ö� hp�� �޴´�
	// int			-		������ hp
	SC_GAME_REMAIN_HP,

	// client -> server
	// ���� play
	// int			-		����index
	CS_GAME_RADIO_PLAY,

	// server -> client
	// ���� play
	// int			-		sessionID
	// int			-		����index
	SC_GAME_RADIO_PLAY,

	// server -> client
	// ������ �ð� ����
	// ������ ����( or int	- ���� ī��Ʈ )
	SC_GAME_TIME_COUNTDOWN,

	// server -> client
	// Ÿ�� �ƿ�!/ ��������� ����
	// ����
	SC_TIME_OUT,

// 	// server -> client
// 	// ���� ����
// 	// int			-		������ ����( �� �¸�/ Ÿ�ӿ��� )
// 	// int			-		�¸� ��
// 	// int			-		������ ��Kill��( ������ Death�� )
// 	// int			-		������ ��Kill��( ������ Death�� )
// 	// int			-		������ win ��
// 	// int			-		������ win ��
// 	// int			-		���� ��� ��
// 	SC_GAME_END,

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

	// server -> client
	// ������ �ٽ� �����Ѵٴ� ��Ŷ�� ������
	SC_GAME_RESTART,

// 	// client -> server
// 	// ������ ������ ������ �̵��ϱ� ���� ���� ��û
// 	// ������ ����
// 	CS_GAME_GOTO_LOBBY,

	// server -> client
	// player�� �κ�� �̵�
	// int			-		ip�������� ũ��
	// char			-		ip
	// int			-		port
	SC_GAME_GOTO_LOBBY,

// 	// server -> client
// 	// player�� �κ�� �̵��ߴٰ� �˸�
// 	// int			-		SessionID
// 	SC_GAME_CHAR_GOTO_LOBBY,

	// server -> client
	// player�� ������ ���� ������ �������� ����鿡�� �˸�
	// int			-		SessionID
	SC_GAME_CHAR_DISCONNECT,
};